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
    pt_t *pt;

    for (t = 0; t < 5; t++) {
        frame_num = findfframe(PAGE_DIRECTORY_TABLE);    /* Get a new frame for page table   */

        if (frame_num == SYSERR) {     /* Error    */
            stop("Error to create a new shared page table!\n");
        }

        /* Set the inverted_page_table entry    */
        inverted_page_table[frame_num].fstate = F_SHARED_PT;
        inverted_page_table[frame_num].reference_count = PAGE_TABLE_ENTRIES;

        inverted_page_table[frame_num].fnext = -1;

        hook_ptable_create(frame_num);

        /* Initialize the page table    */
        pt = (pt_t *)(NBPG * (FRAME0 + frame_num)); /* base address of page table  */
        for (i = 0; i < PAGE_TABLE_ENTRIES; i++) {
            pt[i].pt_pres   = 1;
            pt[i].pt_write  = 1;
            pt[i].pt_user   = 0;
            pt[i].pt_pwt    = 0;
            pt[i].pt_pcd    = 0;
            pt[i].pt_acc    = 0;
            pt[i].pt_dirty  = 0;
            pt[i].pt_mbz    = 0;
            pt[i].pt_global = 0;
            pt[i].pt_avail  = 0;

            if (t < 4)
                pt[i].pt_base = t * PAGE_TABLE_ENTRIES + i;
            else
                pt[i].pt_base = DEVICE_PD * PAGE_TABLE_ENTRIES + i;

        }
        shared_page_table[t] = pt;
    }

}