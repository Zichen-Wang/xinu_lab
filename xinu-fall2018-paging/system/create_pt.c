/*
 * user: wang4113
 * data: 11/02/2018
 */

/* create_pt.c - create_pt */

#include <xinu.h>

/*----------------------------------------
 *  create_pt  -  Create a new page table
 *----------------------------------------
 */

pt_t * create_pt(pid32 pid)
{
    uint32  i;
    int     frame_num;
    pt_t    *pt;

    frame_num = findfframe(PAGE_DIRECTORY_TABLE);    /* Get a new frame for page table   */

    if (frame_num == SYSERR) {     /* Error    */
        kprintf("Error to create a new page table!\n");
        return (pt_t *)(SYSERR);
    }

    /* Set the inverted_page_table entry    */
    inverted_page_table[frame_num].fstate = F_USED_PT;
    inverted_page_table[frame_num].pid = pid;
    inverted_page_table[frame_num].reference_count = 0;

    inverted_page_table[frame_num].fnext = -1;


    hook_ptable_create(frame_num);

    /* Initialize the page table    */
    pt = (pt_t *)(NBPG * (frame_num + FRAME0)); /* base address of page table  */
    for (i = 0; i < PAGE_TABLE_ENTRIES; i++) {
        pt[i].pt_pres   = 0;
        pt[i].pt_write  = 1;
        pt[i].pt_user   = 0;
        pt[i].pt_pwt    = 0;
        pt[i].pt_pcd    = 0;
        pt[i].pt_acc    = 0;
        pt[i].pt_dirty  = 0;
        pt[i].pt_mbz    = 0;
        pt[i].pt_global = 0;
        pt[i].pt_avail  = 0;
        pt[i].pt_base   = 0;
    }

    return pt;
}