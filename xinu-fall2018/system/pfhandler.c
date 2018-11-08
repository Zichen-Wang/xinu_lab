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

    pt_t    *pt;        /* Ptr to p'th page table   */

    int     new_pt_addr, new_frame_num; /* New frame address    */

    a = get_faulted_addr();
    vp = a / NBPG;

    pd = (pd_t *)(proctab[currpid].page_directory);

    if (is_valid_addr(a, currpid) == FALSE) {
        kprintf("Invalid address 0x%08X\n", a);
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
            kill(currpid);
        }
        pd[p].pd_base   = new_pt_addr / NBPG;
    }

    /* Find a new free frame */
    new_frame_num = findfframe(PAGE_VIRTUAL_HEAP);

    if (new_frame_num == SYSERR) {    /* Cannot create a new frame for virtual page   */
        kill(currpid);
    }

    /* Update the inverted_page_table for this new frame    */
    inverted_page_table[new_frame_num].fstate = F_USED_PAGE;
    inverted_page_table[new_frame_num].pid = currpid;
    inverted_page_table[new_frame_num].virt_page_num = vp;



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

    pt[q].pt_base   = new_frame_num + FRAME0;

    inverted_page_table[pd[p].pd_base - FRAME0].reference_count++;

    /* To be continued to read this page from backing store if it is not read in the first time */




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
    if (NBPG * 4096 <= a && a < NBPG * (4096 + proctab[pid].hsize))
        return TRUE;

    return FALSE;
}