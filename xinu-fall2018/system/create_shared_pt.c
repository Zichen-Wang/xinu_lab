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
    uint32 frame_num;
    pt_t *pt_entry;

    for (t = 0; t < 5; t++) {
        frame_num = findfframe(FRAME_DIRECTORY_TABLE)    /* Get a new frame for page table   */

        if (frame_num == (uint32)(SYSERR)) {     /* Error    */
            stop("Error to create a new shared page table!\n");
        }

        inverted_page_table[frame_num].fstate = F_SHARED_PT;

        hook_ptable_create(frame_num);

        pt_entry = (pt_t *)(NBPG * (PAGE_TABLE_BASE + frame_num));
        kprintf("%d\n", (uint32)(pt_entry));

        /* Initialize the page table    */
        for (i = 0; i < PAGE_TABLE_ENTRIES; i++) {
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
                pt_entry -> pt_base	    = NBPG * (t * PAGE_TABLE_ENTRIES + i);
            else
                pt_entry -> pt_base     = NBPG * (DEVICE_FRAME_BASE + i);

            pt_entry++;
        }

        shared_page_table[t] = (char *)(NBPG * (PAGE_TABLE_BASE + frame_num));
    }

}