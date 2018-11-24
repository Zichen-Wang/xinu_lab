/*
 * user: wang4113
 * data: 11/23/2018
 */

/* set_pgrpolicy.c - set the page replacement policy */
#include <xinu.h>

syscall  set_pgrpolicy (uint16 spolicy)
{
    intmask	mask;			/* Saved interrupt mask		*/
    mask = disable();

    pgrpolicy = spolicy;

    restore(mask);
    return OK;
}