/*
 * User: wang4113
 * date: 10/21/2018
*/
/* xalarm.c - xalarm */

#include <xinu.h>

/*--------------------------------------------
 * xalarm - specify the time delay for SIGTIME
 *--------------------------------------------
 */

syscall	xalarm(
        int32	delay		/* Time to delay in seconds	*/
)
{
    intmask	mask;			/* Saved interrupt mask		*/
    mask = disable();

    if (delay <= clktimemilli) {    /*  Time travel into the past is not allowed    */
        restore(mask);
        return SYSERR;
    }

    if ((proctab[currpid].prsig)[SIGTIME].regyes == FALSE) {
        /* Current process does not register SIGTIME callback handler   */

        restore(mask);
        return SYSERR;
    }

    (proctab[currpid].prsig)[SIGTIME].optarg = delay;

    restore(mask);

    return OK;
}