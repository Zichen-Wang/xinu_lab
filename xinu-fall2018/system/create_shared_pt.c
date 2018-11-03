/*
 * user: wang4113
 * data: 11/02/2018
 */

/* create_shared_pt.c - create_shared_pt */

#include <xinu.h>

/*------------------------------------------------
 *  create_shared_pt  -  Create shared page tables
 *------------------------------------------------
 */

extern int32 stop(char *);

void create_shared_pt(void)
{
    uint32 t, i;
    int frame_num;
    pt_t *pt_entry;

    for (t = 0; t < 5; t++) {
        frame_num = findfframe(PAGE_DIRECTORY_TABLE);    /* Get a new frame for page table   */

        if (frame_num == SYSERR) {     /* Error    */
            stop("Error to create a new shared page table!\n");
        }

        /* Set the inverted_page_table entry    */
        inverted_page_table[frame_num].fstate = F_SHARED_PT;
        inverted_page_table[frame_num].reference_count = PAGE_TABLE_ENTRIES;

        hook_ptable_create(frame_num);

        /* Initialize the page table    */
        for (i = 0; i < PAGE_TABLE_ENTRIES; i++) {
            pt_entry = (pt_t *)(NBPG * (FRAME0 + frame_num) + i * 4);  /* address of page table entry  */

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

            if (t < 4)
                pt_entry -> pt_base = t * PAGE_TABLE_ENTRIES + i;
            else
                pt_entry -> pt_base = DEVICE_FRAME_BASE + i;

        }
        shared_page_table[t] = (char *)(NBPG * (FRAME0 + frame_num));
    }

}