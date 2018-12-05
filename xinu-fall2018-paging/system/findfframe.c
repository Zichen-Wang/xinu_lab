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

local void write_back(uint32, uint32, pid32);

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

    uint32  saved_frameq_head, saved_frame_last_stopped;
    uint32  vp;     /* Virtual page number of the page to be replaced */
    uint32  a;      /* The first virtual address on page vp */
    uint32  p, q;   /* p is the high 10 bits of a; q is entry number in page table  */
    pid32   pid;    /* pid be the process ID of the process owning vp.  */

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
        if (spolicy == 0) {   /* The page replacement policy is FIFO */
            if (frameq_head == -1) {     /* The current frame queue is empty (Would never happen)    */
                return SYSERR;
            }

            saved_frameq_head = frameq_head;   /* Save the head frame of the frame queue  */
            frameq_head = inverted_page_table[frameq_head].fnext;

            if (frameq_head == -1) {     /* The new queue is empty   */
                frameq_tail = -1;
            }

            inverted_page_table[saved_frameq_head].fstate = F_FREE;
            vp = inverted_page_table[saved_frameq_head].virt_page_num;
            a = vp * NBPG;
            p = a >> 22;
            q = (a >> 12) & 0x03FF;
            pid = inverted_page_table[saved_frameq_head].pid;
            pd = proctab[pid].page_directory;
            pt = (pt_t *)(NBPG * pd[p].pd_base);


            if (pid == currpid) {   /* If the page being evicted belongs to the current process  */

                /*  Invalidate the TLB entry for the page vp    */
                asm volatile ("invlpg (%0)"
                            : /* No output register */
                            : "r" (a)
                            : "memory");

            }

            hook_pswap_out(vp, saved_frameq_head + FRAME0);

            /* If the dirty bit for page vp was set in its page table   */
            if (pt[q].pt_dirty == 1) {

                pt[q].pt_dirty = 0;
                write_back(saved_frameq_head, vp, pid);

            }

            pt[q].pt_pres = 0;     /* Mark the appropriate entry of pt as not present.    */

            /* Decrement the reference count of the frame occupied by pt */
            inverted_page_table[pd[p].pd_base - FRAME0].reference_count--;

            /* If the reference count has reached zero  */
            if (inverted_page_table[pd[p].pd_base - FRAME0].reference_count == 0) {
                pd[p].pd_pres = 0;     /* Mark the appropriate entry in pd as "not present."  */
                /* Free the frame holding that page table   */
                inverted_page_table[pd[p].pd_base - FRAME0].fstate = F_FREE;
            }


            return saved_frameq_head;
        }

        /*
         * user: wang4113
         * data: 12/02/2018
         */

        else if (spolicy == 1) {
            if (frame_last_stopped == -1) { /* Would never happen   */
                return SYSERR;
            }


            while (1) {

                //kprintf("frame pt: %d\n", frame_last_stopped + FRAME0);

                vp = inverted_page_table[frame_last_stopped].virt_page_num;
                a = vp * NBPG;
                p = a >> 22;
                q = (a >> 12) & 0x03FF;
                pid = inverted_page_table[frame_last_stopped].pid;
                pd = proctab[pid].page_directory;
                pt = (pt_t *)(NBPG * pd[p].pd_base);

                if (pt[q].pt_dirty == 1) {
                    inverted_page_table[frame_last_stopped].dirty = 1;
                    pt[q].pt_acc = 1;
                    pt[q].pt_dirty = 0;
                }
                else if (pt[q].pt_acc == 1) {
                    pt[q].pt_acc = 0;
                }
                else {
                    break;
                }

                frame_last_stopped++;

                if (frame_last_stopped == NFRAMES)
                    frame_last_stopped = NFRAMES_FOR_PAGE_TABLE;
            }

            saved_frame_last_stopped = frame_last_stopped;

            frame_last_stopped++;

            if (frame_last_stopped == NFRAMES)
                frame_last_stopped = NFRAMES_FOR_PAGE_TABLE;

            inverted_page_table[saved_frame_last_stopped].fstate = F_FREE;
            pid = inverted_page_table[saved_frame_last_stopped].pid;

            if (pid == currpid) {   /* If the page being evicted belongs to the current process  */

                /*  Invalidate the TLB entry for the page vp    */
                asm volatile ("invlpg (%0)"
                : /* No output register */
                : "r" (a)
                : "memory");

            }

            hook_pswap_out(vp, saved_frame_last_stopped + FRAME0);

            /* If the dirty bit for page vp was set in its page table   */
            if (inverted_page_table[saved_frame_last_stopped].dirty == 1) {

                inverted_page_table[saved_frame_last_stopped].dirty = 0;
                write_back(saved_frame_last_stopped, vp, pid);

            }

            pt[q].pt_pres = 0;     /* Mark the appropriate entry of pt as not present.    */

            /* Decrement the reference count of the frame occupied by pt */
            inverted_page_table[pd[p].pd_base - FRAME0].reference_count--;

            /* If the reference count has reached zero  */
            if (inverted_page_table[pd[p].pd_base - FRAME0].reference_count == 0) {
                pd[p].pd_pres = 0;     /* Mark the appropriate entry in pd as "not present."  */
                /* Free the frame holding that page table   */
                inverted_page_table[pd[p].pd_base - FRAME0].fstate = F_FREE;
            }

            return saved_frame_last_stopped;
        }

        return SYSERR;
    }

    return SYSERR;

}

local void write_back(uint32 frame_no, uint32 vp, pid32 pid)
{
    bsd_t   s;
    uint32  o;
    /* Using the backing store map, find the store and page offset within the store for pid and a   */
    s = find_bs_map(vp, pid);
    o = vp - backing_store_map[s].virt_base_num;
    if (s == SYSERR || o >= backing_store_map[s].npages) {   /* If the lookup fails  */
        kprintf("Backing store lookup failed for virtual page!\n", vp);
    }

    /* Write the page back to the backing store     */

    /*
    kprintf("Process ID %d is writing frame %d of pid %d to s: %d, o: %d\n",
            currpid, frame_no + FRAME0, pid, s, o);
    */
    if (write_bs((char *)(NBPG * (frame_no + FRAME0)), s, o) == SYSERR) {
        kprintf("Cannot write dirty page to the backing store %d!\n", s);
    }
}