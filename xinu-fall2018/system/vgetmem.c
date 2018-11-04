/*
 * user: wang4113
 * data: 11/03/2018
 */

/* vgetmem.c - vgetmem */

#include <xinu.h>

/*---------------------------------------------------------------------------
 *  vgetmem  -  Allocate virtual heap storage, returning lowest word address
 *---------------------------------------------------------------------------
 */

char  	*vgetmem(
        uint32	nbytes		/* Size of memory requested	*/
)
{
    intmask	mask;			/* Saved interrupt mask		*/
    struct	memblk	*prev, *curr, *leftover;
    struct  procent *prptr; /* Ptr to process table entry   */

    mask = disable();

    if (nbytes == 0) {
        restore(mask);
        return (char *)SYSERR;
    }

    nbytes = (uint32) roundmb(nbytes);	/* Use memblk multiples	*/

    prptr = &proctab[currpid];

    prev = &(prptr -> vmemlist);
    curr = (prptr -> vmemlist).mnext;


    if (prptr -> vmem_init == TRUE) {     /* The virtual memory should be initialized */
        prptr -> vmem_init = FALSE;

        curr -> mnext = NULL;                       /* first page fault */
        curr -> mlength = NBPG * (prptr -> hsize);

    }

    while (curr != NULL) {			/* Search free list	*/

        if (curr -> mlength == nbytes) {	/* Block is exact match	*/
            prev -> mnext = curr -> mnext;
            (prptr -> vmemlist).mlength -= nbytes;
            restore(mask);
            return (char *)(curr);

        }
        else if (curr -> mlength > nbytes) { /* Split big block	*/
            leftover = (struct memblk *)((uint32) curr +
                                         nbytes);
            kprintf("%d\n", ((pt_t *)((prptr -> page_directory)[(uint32)(curr) >> 22].pd_base * NBPG))[(uint32)(curr) / NBPG & 0x000003FF].pt_pres);
            prev -> mnext = leftover;
            leftover -> mnext = curr -> mnext;
            leftover -> mlength = curr -> mlength - nbytes;
            (prptr -> vmemlist).mlength -= nbytes;
            restore(mask);
            return (char *)(curr);
        }
        else {			/* Move to next block	*/
            prev = curr;
            curr = curr -> mnext;
        }
    }

    restore(mask);
    return (char *)SYSERR;
}
