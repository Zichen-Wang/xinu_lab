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

char * create_pt(pid32 pid)
{
    uint32 i;
    int frame_num;
    pt_t *pt_entry;

    frame_num = findfframe(PAGE_DIRECTORY_TABLE);    /* Get a new frame for page table   */

    if (frame_num == SYSERR) {     /* Error    */
        kprintf("Error to create a new page table!\n");
        return (char *)(SYSERR);
    }

    /* Set the inverted_page_table entry    */
    inverted_page_table[frame_num].fstate = F_PT;
    inverted_page_table[frame_num].pid = pid;
    inverted_page_table[frame_num].reference_count = 0;

    pt_entry = (pt_t *)(NBPG * (PAGE_TABLE_BASE + frame_num));  /* Base address of page table   */

    hook_ptable_create(frame_num);
    kprintf("pid %d: page table address 0x%08X\n", pid, (uint32)(pt_entry));

    /* Initialize the page table    */
    for (i = 0; i < PAGE_TABLE_ENTRIES; i++) {
        pt_entry -> pt_pres     = 0;
        pt_entry -> pt_write    = 1;
        pt_entry -> pt_user	    = 0;
        pt_entry -> pt_pwt	    = 0;
        pt_entry -> pt_pcd	    = 0;
        pt_entry -> pt_acc	    = 0;
        pt_entry -> pt_dirty    = 0;
        pt_entry -> pt_mbz	    = 0;
        pt_entry -> pt_global   = 0;
        pt_entry -> pt_avail    = 0;
        pt_entry -> pt_base	    = 0;

        pt_entry++;
    }

    return (char *)(NBPG * (PAGE_TABLE_BASE + frame_num));
}