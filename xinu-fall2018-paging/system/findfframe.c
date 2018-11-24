/*
 * user: wang4113
 * data: 11/01/2018
 */

/* findfframe.c - findfframe */
#include <xinu.h>

/*-------------------------------------
 *  findfframe  -  Find a free frame
 *-------------------------------------
 */


int findfframe(uint8 type)
/*
 * type == PAGE_DIRECTORY_TABLE means find a free frame for page directory or page table
 * type == PAGE_VIRTUAL_HEAP means find a free frame for virtual heap
 */
{
    uint32	i;			/* Iterate through all frames   */
    /* Position in array [0, NFRAMES_FOR_PAGE_TABLE - 1] for page directory of page table	*/
    static	uint32 next_frame_pdt = 0;

    /* Position in array [NFRAMES_FOR_PAGE_TABLE, NFRAMES - 1] for virtual memory	*/
    static  uint32 next_frame_virt = NFRAMES_FOR_PAGE_TABLE;

    uint32  old_frameq_head;
    uint32  vp;     /* Virtual page number of the page to be replaced */
    uint32  a;      /* The first virtual address on page vp */
    uint32  p, q;   /* p is the high 10 bits of a; q is entry number in page table  */
    pid32   pid;    /* pid be the process ID of the process owning vp.  */

    bsd_t   s;
    uint32  o;

    pd_t    *pd;
    pt_t    *pt;


    if (type == PAGE_DIRECTORY_TABLE) { /* find a frame for page directory or page table   */
        for (i = 0; i < NFRAMES_FOR_PAGE_TABLE; i++) {

            if (next_frame_pdt == NFRAMES_FOR_PAGE_TABLE) { /* Wrap around to beginning */
                next_frame_pdt = 0;
            }

            if (inverted_page_table[next_frame_pdt].fstate == F_FREE) {
                return next_frame_pdt++;
            }
            else {
                next_frame_pdt++;
            }
        }

        return SYSERR;
    }

    else if (type == PAGE_VIRTUAL_HEAP) { /* find a frame for for virtual heap   */

        for (i = 0; i < NFRAMES_FOR_VIRTUAL_HEAP; i++) {

            if (next_frame_virt == NFRAMES) { /* Wrap around to beginning */
                next_frame_virt = NFRAMES_FOR_PAGE_TABLE;
            }

            if (inverted_page_table[next_frame_virt].fstate == F_FREE) {
                return next_frame_virt++;
            }
            else {
                next_frame_virt++;
            }
        }

        /* Needs page replacement   */
        if (pgrpolicy == 0) {   /* The page replacement policy is FIFO */
            if (frameq_head == -1) {     /* The current frame queue is empty (Would never happen)    */
                return SYSERR;
            }
            old_frameq_head = frameq_head;   /* Save the head frame of the frame queue  */
            frameq_head = inverted_page_table[frameq_head].fnext;

            if (frameq_head == -1) {     /* The new queue is empty   */
                frameq_tail = -1;
            }

            inverted_page_table[old_frameq_head].fstate = F_FREE;
            vp = inverted_page_table[old_frameq_head].virt_page_num;
            a = vp * NBPG;
            p = a >> 22;
            q = (a / NBPG) & 0x000003FF;
            pid = inverted_page_table[old_frameq_head].pid;
            pd = proctab[pid].page_directory;
            pt = (pt_t *)(NBPG * pd[p].base);


            if (pid == currpid) {   /* If the page being evicted belongs to the current process  */

                /*  Invalidate the TLB entry for the page vp    */
                asm volatile ("invlpg (%0)"
                            : /* No output register */
                            : "r" (a));

            }

            /* If the dirty bit for page vp was set in its page table   */
            if (pt[q].pt_dirty == 1) {

                /* Using the backing store map, find the store and page offset within the store for pid and a   */
                s = proctab[pid].bs_map_id;
                o = vp - backing_store_map[s].virt_base_num;
                if (s == -1 || o >= backing_store_map[s].npage) {   /* If the lookup fails  */
                    kprintf("Backing store lookup failed for address [0x%08X]!\n", a);
                    kprintf("Process %d is being killed!\n", pid);
                    kill(pid);
                }

                /* Write the page back to the backing store     */
                if (write_bs(NBPG * (old_frameq_head + FRAME0), s, o) == SYSERR) {
                    kprintf("Cannot write dirty page to the backing store %d!\n", s);
                    kprintf("Process %d is being killed!\n", pid);
                    kill(pid);
                }

            }

            pt[q].pres = 0;     /* Mark the appropriate entry of pt as not present.    */

            /* Decrement the reference count of the frame occupied by pt */
            inverted_page_table[pd[p].pd_base - FRAME0].reference_count--;

            /* If the reference count has reached zero  */
            if (inverted_page_table[pd[p].pd_base - FRAME0].reference_count == 0) {
                pd[p].pd_pres = 0;     /* Mark the appropriate entry in pd as "not present."  */

                /* Free the frame holding that page table   */
                inverted_page_table[pd[p].pd_base - FRAME0].fstate = F_FREE;

            }

            return old_frameq_head;
        }

        return SYSERR;
    }

    return SYSERR;

}