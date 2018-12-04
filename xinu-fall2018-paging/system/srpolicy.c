/*
 * user: wang4113
 * data: 11/23/2018
 */

/* srpolicy.c - set the page replacement policy */
#include <xinu.h>

syscall  srpolicy (uint16 spolicy)
{
    intmask	mask;			/* Saved interrupt mask		*/
    mask = disable();

    pgrpolicy = spolicy;

    if (spolicy == 0) {     /* The page replacement policy is FIFO */
        frameq_head = frameq_tail = -1;
    }
    else if (spolicy == 1) {    /* The page replacement policy is CLOCK */
        frame_clock_pt = -1;
    }

    restore(mask);
    return OK;
}