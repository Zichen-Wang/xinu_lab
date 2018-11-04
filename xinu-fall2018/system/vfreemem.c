/*
 * user: wang4113
 * data: 11/03/2018
 */

/* vfreemem.c - vfreemem, free_frames */

#include <xinu.h>

local void free_frames(uint32, uint32);

/*----------------------------------------------------------------------------------------
 *  vfreemem  -  Free a virtual memory block, returning the block to the free virtual list
 *----------------------------------------------------------------------------------------
 */


syscall	vfreemem(
        char		*blkaddr,	/* Pointer to memory block	*/
        uint32	nbytes		/* Size of block in bytes	*/
)
{
    intmask	mask;			/* Saved interrupt mask		*/
    struct	memblk	*next, *prev, *block;
    uint32	top;
    struct  procent *prptr; /* Ptr to process table entry   */

    mask = disable();

    prptr = &proctab[currpid];

    if ((nbytes == 0) || ((uint32) blkaddr < (uint32) (NBPG * (FRAME0 + NFRAMES)))
        || ((uint32) blkaddr > (uint32) (NBPG * (FRAME0 + NFRAMES + prptr -> hsize)))) {
        restore(mask);
        return SYSERR;
    }

    nbytes = (uint32) roundmb(nbytes);	/* Use memblk multiples	*/
    block = (struct memblk *)blkaddr;

    prptr = &proctab[currpid];

    prev = &(prptr -> vmemlist);		/* Walk along free list	*/
    next = (prptr -> vmemlist).mnext;


    while ((next != NULL) && (next < block)) {
        prev = next;
        next = next -> mnext;
    }

    if (prev == &(prptr -> vmemlist)) {		/* Compute top of previous block*/
        top = (uint32) NULL;
    } else {
        top = (uint32) prev + prev -> mlength;
    }

    /* Ensure new block does not overlap previous or next blocks	*/

    if (((prev != &(prptr -> vmemlist)) && (uint32) block < top)
        || ((next != NULL)	&& (uint32) block + nbytes > (uint32)next)) {
        restore(mask);
        return SYSERR;
    }

    (prptr -> vmemlist).mlength += nbytes;

    /* Either coalesce with previous block or add to free list */

    if (top == (uint32) block) { 	/* Coalesce with previous block	*/
        prev -> mlength += nbytes;
        block = prev;
    } else {			/* Link into list as new node	*/
        block -> mnext = next;
        block -> mlength = nbytes;
        prev -> mnext = block;
    }

    /* Coalesce with next block if adjacent */

    if (((uint32) block + block -> mlength) == (uint32) next) {
        block -> mlength += next -> mlength;
        block -> mnext = next -> mnext;
    }

    free_frames((uint32)(blkaddr), nbytes);

    restore(mask);
    return OK;
}

/*--------------------------------------------------------------
 *  free_frames  -  Free frames occupied by this virtual memory
 *--------------------------------------------------------------
 */

local void free_frames(uint32 start_addr, uint32 length)
{
    uint32  end_addr = start_addr + length;
    uint32  i, j, ps, pe, qs, qe;
    /* The frames that we should free can be represented as
     * the whole ps'th table, the whole (ps + 1)'th table, ..., the whole (pe - 1)'th table
     * AND
     * page 0 in pe'th table, page 1 in pe'th table, ..., page (qe - 1) in pe'th table
     */
    pd_t    *pd;
    pt_t    *pt;
    uint32  frame_virt_num, frame_pt_num;

    pd = (pd_t *)(proctab[currpid].page_directory);

    ps = start_addr >> 22;
    pe = end_addr >> 22;

    /* The page tables from ps to (pe - 1) and their pages should be freed */
    for (i = ps; i < pe; i++) {

        /* the page table is present */
        if (pd[i].pd_pres == 1) {
            /* Find the frame number of this page table   */
            frame_pt_num = pd[i].pd_base - FRAME0;

            pt = (pt_t *)(NBPG * pd[i].pd_base);
            for (j = 0; j < PAGE_TABLE_ENTRIES; j++) {

                /* the virtual page is present  */
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

    /* The 0 to (qe - 1)'s pages in pe'th page table should be freed  */

    pt = (pt_t *)(NBPG * pd[pe].pd_base);
    frame_pt_num = pd[pe].pd_base - FRAME0;

    qs = 0;

    qe = (end_addr / NBPG) & 0x000003FF;

    for (j = qs; j < qe; j++) {

        /* the virtual page is present  */
        if (pt[j].pt_pres == 1) {
            /* Find the frame number of the virtual page   */
            frame_virt_num = pt[j].pt_base - FRAME0;

            /* Free this frame */
            inverted_page_table[frame_virt_num].fstate = F_FREE;
            pt[j].pt_pres = 0;

            /* The reference count of pe'th page table decreases one  */
            inverted_page_table[frame_pt_num].reference_count--;

            /* Decrease to zero */
            if (inverted_page_table[frame_pt_num].reference_count == 0) {

                /* Free pe'th page table */
                inverted_page_table[frame_pt_num].fstate = F_FREE;
                pd[pe].pd_pres = 0;

                hook_ptable_delete(frame_pt_num);

                break;
            }
        }

    }

}