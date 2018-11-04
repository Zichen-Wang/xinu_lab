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
    char    *pd;        /* current page directory   */

    pd_t    *pt;        /* Ptr to p'th page table   */

    pt_t    *pt_entry;  /* Ptr to entry of page table   */
    int     new_pt_addr, new_frame_num; /* New frame address    */

    a = get_faulted_addr();
    vp = a / NBPG;

    pd = proctab[currpid].page_directory;

    if (is_valid_addr(a, currpid) == FALSE) {
        kprintf("Invalid address 0x%08X\n", a);
        kill(currpid);
    }

    hook_pfault((char *)(a));

    p = a >> 22;
    q = (a / NBPG) & 0x000003FF;

    pt = (pd_t *)(pd + p * 4);

    if (pt -> pd_pres == 0) {   /* p'th page table does not exist   */
        pt -> pd_pres   = 1;
        pt -> pd_write  = 1;
        pt -> pd_user   = 0;
        pt -> pd_pwt    = 0;
        pt -> pd_pcd    = 0;
        pt -> pd_acc    = 0;
        pt -> pd_mbz    = 0;
        pt -> pd_fmb    = 0;
        pt -> pd_global = 0;
        pt -> pd_avail  = 0;

        new_pt_addr = (int)(create_pt(currpid));
        if (new_pt_addr == SYSERR) {    /* Cannot create a new frame for page table   */
            kill(currpid);
        }
        pt -> pd_base   = new_pt_addr / NBPG;
    }

    inverted_page_table[pt -> pd_base - FRAME0].reference_count++;

    new_frame_num = findfframe(PAGE_VIRTUAL_HEAP);


    /* Update the inverted_page_table for this new frame    */
    inverted_page_table[new_frame_num].fstate = F_USED_VIRT;
    inverted_page_table[new_frame_num].pid = currpid;
    inverted_page_table[new_frame_num].virt_page_num = vp;


    if (new_frame_num == SYSERR) {    /* Cannot create a new frame for virtual page   */
        kill(currpid);
    }

    pt_entry = (pt_t *)(NBPG * (pt -> pd_base) + q * 4);

    pt_entry -> pt_pres     = 1;
    pt_entry -> pt_write    = 1;
    pt_entry -> pt_user	    = 0;
    pt_entry -> pt_pwt	    = 0;
    pt_entry -> pt_pcd	    = 0;
    pt_entry -> pt_acc	    = 0;
    pt_entry -> pt_dirty    = 0;
    pt_entry -> pt_mbz	    = 0;
    pt_entry -> pt_global   = 0;
    pt_entry -> pt_avail    = 0;

    pt_entry -> pt_base	    = new_frame_num + FRAME0;




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
 * is_valid_addr - Check if a is a valid address
 *------------------------------------------------------------------------
 */

local   bool8   is_valid_addr(uint32 a, pid32 pid)
{
    if (NBPG * 4096 + proctab[pid].vmemlist.mlength >= a)
        return TRUE;

    return FALSE;
}