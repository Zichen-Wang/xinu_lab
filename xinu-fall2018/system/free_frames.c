/*
 * user: wang4113
 * data: 11/02/2018
 */

/* free_frames.c - free_frames */

#include <xinu.h>

/*-----------------------------------------------------------
 *  free_frames  -  free frames occupied by a killed process
 *-----------------------------------------------------------
 */

void free_frames(pid32 pid)
{
    struct	procent	*prptr;     /* Ptr to process table entry	*/
    pd_t    *pd_entry;          /* Ptr to page directory entry	*/
    pt_t    *pt_entry;          /* Ptr to page table entry	*/
    int     frame_pd_num, frame_pt_num, frame_virt_num;
    uint32  i, j;

    prptr = &proctab[pid];

    for (i = 0; i < PAGE_DIRECTORY_ENTRIES; i++) {

        pd_entry = (pd_t *)(prptr -> page_directory + i * 4);

        /* the page table is present */
        if (pd_entry -> pd_pres == 1) {
            pd_entry -> pd_pres = 0;
            /* Find the frame number of this page table   */
            frame_pt_num = (pd_entry -> pd_base) - FRAME0;

            /* it is not a shared page table   */
            if (inverted_page_table[frame_pt_num].fstate != F_SHARED_PT) {

                for (j = 0; j < PAGE_TABLE_ENTRIES; j++) {
                    pt_entry = (pt_t *)(NBPG * (pd_entry -> pd_base) + j * 4);

                    /* the true frame is present  */
                    if (pt_entry -> pt_pres == 1) {
                        pt_entry -> pt_pres = 0;
                        /* Find the frame number of the virtual page   */
                        frame_virt_num = (pt_entry -> pt_base) - FRAME0;

                        /* Free this frame for virtual memory */
                        inverted_page_table[frame_virt_num].fstate = F_FREE;
                    }
                }

                /* Free this frame of page table */
                inverted_page_table[frame_pt_num].fstate = F_FREE;

                hook_ptable_delete(frame_pt_num);
            }
        }
    }

    /* Find the frame number of this page directory   */
    frame_pd_num = (uint32)(prptr -> page_directory) / NBPG - FRAME0;
    inverted_page_table[frame_pd_num].fstate = F_FREE;  /* Free this frame  */
}
