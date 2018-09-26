/* ready.c - ready */

#include <xinu.h>

qid16	readylist;			/* Index of ready list		*/

/*------------------------------------------------------------------------
 *  ready  -  Make a process eligible for CPU service
 *------------------------------------------------------------------------
 */
status	ready(
	  pid32		pid		/* ID of process to make ready	*/
	)
{
	register struct procent *prptr;

	if (isbadpid(pid)) {
		return SYSERR;
	}

	/* Set process state to indicate ready and add to ready list */

	prptr = &proctab[pid];
	prptr->prstate = PR_READY;

	/*
	 * User: wang4113
	 * date: 09/19/2018
	 */
	/* Lab2 3.3: Set pstartwait and pwaitcount of the old process	*/
	prptr -> pstartwait = clktimemilli;	/* Set start time of waiting by clktimemilli	*/
	prptr -> pwaitcount++;				/* Increase waiting count by 1	*/

	insert(pid, readylist, prptr->prprio);


	resched();

	return OK;
}
