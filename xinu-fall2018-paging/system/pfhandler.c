/*
 * user: wang4113
 * data: 11/02/2018
 */

/* pfhandler.c - pfhandler, get_faulted_addr, is_valid_addr */

#include <xinu.h>

local   uint32  get_faulted_addr(void);
local   bool8   is_valid_addr(uint32, pid32);

/*------------------------------------------------------------------------
 * pfhandler - high level page fault handler
 *------------------------------------------------------------------------
 */

void	pfhandler()
{
    uint32  a, p, q;    /* faulted address  */
    uint32  vp;         /* virtual pages    */
    pd_t    *pd;        /* current page directory   */

    pt_t    *pt;        /* ptr to p'th page table   */
    uint32  f;          /* free frame   */
    uint32  s, o;       /* backing store s, and offset o    */

    int     new_pt_addr, new_frame_num; /* New frame address    */

    a = get_faulted_addr();
    vp = a / NBPG;

    pd = (pd_t *)(proctab[currpid].page_directory);

    if (is_valid_addr(a, currpid) == FALSE) {
        kprintf("Process %d: Invalid address 0x%08X\n", currpid, a);
        kill(currpid);
    }

    hook_pfault((char *)(a));

    p = a >> 22;
    q = (a / NBPG) & 0x000003FF;

    if (pd[p].pd_pres == 0) {   /* p'th page table does not exist   */
        pd[p].pd_pres   = 1;
        pd[p].pd_write  = 1;
        pd[p].pd_user   = 0;
        pd[p].pd_pwt    = 0;
        pd[p].pd_pcd    = 0;
        pd[p].pd_acc    = 0;
        pd[p].pd_mbz    = 0;
        pd[p].pd_fmb    = 0;
        pd[p].pd_global = 0;
        pd[p].pd_avail  = 0;

        new_pt_addr = (int)(create_pt(currpid));
        if (new_pt_addr == SYSERR) {    /* Cannot create a new frame for page table   */
            kprintf("Process %d: Cannot create a new frame for page table!\n", currpid);
            kill(currpid);
        }
        pd[p].pd_base   = new_pt_addr / NBPG;
    }


    /* Using the backing store map, find the store s and page offset o which correspond to vp   */
    s = proctab[currpid].bs_map_id;
    o = vp - backing_store_map[s].virt_base_num;

    /* In the inverted page table, increment the reference count of the frame which holds pt.   */
    inverted_page_table[pd[p].pd_base - FRAME0].reference_count++;

    /* Obtain a free frame f */
    new_frame_num = findfframe(PAGE_VIRTUAL_HEAP);
    f = new_frame_num + FRAME0;

    if (new_frame_num == SYSERR) {    /* Cannot create a new frame for virtual page   */
        kprintf("Process %d: Cannot create a new frame for virtual page!\n", currpid);
        kill(currpid);
    }

    /* Update the inverted_page_table for this new frame    */
    inverted_page_table[new_frame_num].fstate = F_USED_PAGE;
    inverted_page_table[new_frame_num].pid = currpid;
    inverted_page_table[new_frame_num].virt_page_num = vp;

    if (pgrpolicy == 0) {   /* Page replacement policy is FIFO  */
        /* Insert this frame into frame queue tail   */
        if (frameq_tail == -1) { /* The current frame queue is empty   */
            frameq_head = frameq_tail = new_frame_num;
        }
        else {
            inverted_page_table[frameq_tail].fnext = new_frame_num;
            inverted_page_table[new_frame_num].fprev = frameq_tail;
            frameq_tail = new_frame_num;
            inverted_page_table[new_frame_num].fnext = -1;
        }
    }

    /* Copy the page o of store s to f  */
    kprintf("backing store!!!!!!%d %d [0x%08X]!!!!!!\n", s, o, NBPG * f);
    if (write_bs((char *)(NBPG * f), s, o) == SYSERR) {
        kprintf("Process %d: Cannot read a page from backing store!\n", currpid);
        kill(currpid);
    }


    /* Update pt to mark the appropriate entry as present, and set other relevant fields */

    pt = (pt_t *)(NBPG * (pd[p].pd_base));

    pt[q].pt_pres   = 1;
    pt[q].pt_write  = 1;
    pt[q].pt_user   = 0;
    pt[q].pt_pwt    = 0;
    pt[q].pt_pcd    = 0;
    pt[q].pt_acc    = 0;
    pt[q].pt_dirty  = 0;
    pt[q].pt_mbz    = 0;
    pt[q].pt_global = 0;
    pt[q].pt_avail  = 0;

    pt[q].pt_base   = f;

}

/*------------------------------------------------------------------------
 * get_faulted_addr - Get faulted address
 *------------------------------------------------------------------------
 */

local   uint32  get_faulted_addr(void)
{
    uint32  cr2;

    asm volatile ("movl %%cr2, %0\n\t"
                : "=r" (cr2) );

    return cr2;
}



/*------------------------------------------------------------------------
 * is_valid_addr - Check if a given address is a valid address
 *------------------------------------------------------------------------
 */

local   bool8   is_valid_addr(uint32 a, pid32 pid)
{
    /* We could never encounter an address which has already been in the shared table   */
    /* Just check if the virtual address is in the virtual heap address and does not exceed the size limit  */
    if (NBPG * VHEAP_ST <= a && a < NBPG * (VHEAP_ST + proctab[pid].hsize))
        return TRUE;

    return FALSE;
}