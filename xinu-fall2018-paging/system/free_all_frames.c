/*
 * user: wang4113
 * data: 11/02/2018
 */

/* free_all_frames.c - free_all_frames */

#include <xinu.h>

/*------------------------------------------------------------------
 *  free_all_frames  -  free all frames occupied by a killed process
 *------------------------------------------------------------------
 */

void free_all_frames(pid32 pid)
{
    struct	procent	*prptr;     /* Ptr to process table entry	*/
    pd_t    *pd;                /* Ptr to page directory	*/
    pt_t    *pt;                /* Ptr to page table	*/
    int     frame_pd_num, frame_pt_num, frame_virt_num;
    uint32  i, j;

    prptr = &proctab[pid];

    pd = prptr -> page_directory;

    for (i = 0; i < PAGE_DIRECTORY_ENTRIES; i++) {

        /* If the page table is present */
        if (pd[i].pd_pres == 1) {
            /* Find the frame number of this page table   */
            frame_pt_num = pd[i].pd_base - FRAME0;

            /* If it is not a shared page table   */
            if (inverted_page_table[frame_pt_num].fstate != F_SHARED_PT) {

                pt = (pt_t *)(NBPG * pd[i].pd_base);
                for (j = 0; j < PAGE_TABLE_ENTRIES; j++) {

                    /* If the virtual page is present  */
                    if (pt[j].pt_pres == 1) {
                        /* Find the frame number of the virtual page   */
                        frame_virt_num = pt[j].pt_base - FRAME0;

                        /* Free this frame */
                        inverted_page_table[frame_virt_num].fstate = F_FREE;
                        pt[j].pt_pres = 0;
                    }
                }

                /* Free this frame of page table */
                inverted_page_table[frame_pt_num].fstate = F_FREE;
                inverted_page_table[frame_pt_num].reference_count = 0;
                pd[i].pd_pres = 0;

                hook_ptable_delete(frame_pt_num);
            }
        }
    }

    /* Find the frame number of this page directory   */
    frame_pd_num = (uint32)(pd) / NBPG - FRAME0;
    inverted_page_table[frame_pd_num].fstate = F_FREE;  /* Free this frame  */

    hook_pdir_delete(frame_pd_num);
}
