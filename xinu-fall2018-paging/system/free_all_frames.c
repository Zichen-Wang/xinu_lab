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
    int     frame_pd_num, frame_pt_num;
    uint32  i;
    uint32  vp, a, p, q;

    int32   frameq_prev, frameq_curr;

    prptr = &proctab[pid];
    pd = prptr -> page_directory;

    if (prptr -> hsize > 0 && pgrpolicy == 0) {   /* The page replacement policy is FIFO */
        /* modify the frame queue    */
        frameq_prev = -1;
        frameq_curr = frameq_head;
        while (frameq_curr != -1) {
            if (pid == inverted_page_table[frameq_curr].pid) {
                if (frameq_prev != -1) {
                    inverted_page_table[frameq_prev].fnext = inverted_page_table[frameq_curr].fnext;
                }

                /* Free this frame */
                vp = inverted_page_table[frameq_curr].virt_page_num;
                a = vp * NBPG;
                p = a >> 22;
                q = (a >> 12) & 0x03FF;
                pt = (pt_t *)(NBPG * pd[p].pd_base);
                pt[q].pt_pres = 0;
                pt[q].pt_avail = 0;

                inverted_page_table[frameq_curr].fstate = F_FREE;
                inverted_page_table[frameq_curr].fnext = -1;

            }
            else {
                frameq_prev = frameq_curr;
            }
            frameq_curr = inverted_page_table[frameq_curr].fnext;
        }
    }

    /* Clear page tables and page directory  */
    for (i = 0; i < PAGE_DIRECTORY_ENTRIES; i++) {

        /* If the page table is present */
        if (pd[i].pd_pres == 1) {
            /* Find the frame number of this page table   */
            frame_pt_num = pd[i].pd_base - FRAME0;

            /* If it is not a shared page table   */
            if (inverted_page_table[frame_pt_num].fstate != F_SHARED_PT) {

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
