/*
 * user: wang4113
 * data: 11/03/2018
 */

/* vfreemem.c - vfreemem */

#include <xinu.h>


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


    restore(mask);
    return OK;
}

