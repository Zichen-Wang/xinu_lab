/* sleep.c - sleep sleepms */

#include <xinu.h>

#define	MAXSECONDS	2147483		/* Max seconds per 32-bit msec	*/

/*------------------------------------------------------------------------
 *  sleep  -  Delay the calling process n seconds
 *------------------------------------------------------------------------
 */
syscall	sleep(
	  int32	delay		/* Time to delay in seconds	*/
	)
{
	if ( (delay < 0) || (delay > MAXSECONDS) ) {
		return SYSERR;
	}
	sleepms(1000*delay);
	return OK;
}

/*------------------------------------------------------------------------
 *  sleepms  -  Delay the calling process n milliseconds
 *------------------------------------------------------------------------
 */
syscall	sleepms(
	  int32	delay			/* Time to delay in msec.	*/
	)
{
	intmask	mask;			/* Saved interrupt mask		*/

	if (delay < 0) {
		return SYSERR;
	}


	if (delay == 0) {
		yield();
		return OK;
	}

	/* Delay calling process */

	mask = disable();

	/*
	 * User: wang4113
	 * date: 09/19/2018
	 */
	/* Lab2 5.4: In R3 mode, if a process voluntarily relinquishes the CPU,
	 *   its priority is elevated to IOPRIO	*/
	if (XINUSCHED == 1 && currpid > 0) {	/* The priority of prnull process should remain zero	*/
		proctab[currpid].prprio = IOPRIO;
	}

	if (insertd(currpid, sleepq, delay) == SYSERR) {
		restore(mask);
		return SYSERR;
	}

	proctab[currpid].prstate = PR_SLEEP;
	resched();
	restore(mask);
	return OK;
}
