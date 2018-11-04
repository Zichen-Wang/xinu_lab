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

    struct  procent *prptr;

    mask = disable();

    if (nbytes == 0) {
        restore(mask);
        return (char *)SYSERR;
    }

    nbytes = (uint32) roundmb(nbytes);	/* Use memblk multiples	*/

    prptr = &proctab[currpid];

    prev = &(prptr -> vmemlist);
    curr = (prptr -> vmemlist).mnext;

    if (curr == NULL) {     /* The process is not created by vcreate()  */
        restore(mask);
        return (char *)SYSERR;
    }

    if (prptr -> is_mem_initialized == FALSE) {     /* The virtual memory is not initialized */
        prptr -> is_mem_initialized = TRUE;

        curr -> mnext = NULL;                       /* first page fault */
        curr -> mlength = prev -> mlength;

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