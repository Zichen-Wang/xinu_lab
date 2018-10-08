/* resched.c - resched, resched_cntl */

#include <xinu.h>

struct	defer	Defer;

/*------------------------------------------------------------------------
 *  resched  -  Reschedule processor to highest priority eligible process
 *------------------------------------------------------------------------
 */
void	resched(void)		/* Assumes interrupts are disabled	*/
{
	struct procent *ptold;	/* Ptr to table entry for old process	*/
	struct procent *ptnew;	/* Ptr to table entry for new process	*/

	/* If rescheduling is deferred, record attempt and return */

	if (Defer.ndefers > 0) {
		Defer.attempt = TRUE;
		return;
	}

	/* Point to process table entry for the current (old) process */

	ptold = &proctab[currpid];

	if (ptold->prstate == PR_CURR) {  /* Process remains eligible */
		/*
		 * User: wang4113
		 * date: 10/17/2018
		 */
		/* Lab3 3.2 */
		/* In CFS mode, change the priority of current process before resched()    */
		if (XINUSCHED == 2) {
			if (currpid > 0) {        /* The priority of prnull should remain zero	*/
				/* Now, the virtual CPU usage is proctab[currpid].pvirtcpu + currproctime	*/
				/* Update the priority accordingly	*/
				ptold -> prprio = MAXPRIO - (proctab[currpid].pvirtcpu + currproctime);
			}
		}

		if (ptold->prprio > firstkey(readylist)) {
			return;
		}

		/* Old process will no longer remain current */



		ptold->prstate = PR_READY;
		/*
		 * User: wang4113
		 * date: 09/19/2018
		 */
		/* Lab2 3.3: Set pstartwait and pwaitcount of the old process	*/
		ptold -> pstartwait = clktimemilli;			/* Set start time of waiting by clktimemilli	*/
		ptold -> pwaitcount++;						/* Increase waiting count by 1	*/

        /*
		 * User: wang4113
		 * date: 10/07/2018
		 */
        /* Lab3 3.2: If in CFS mode We need to update pvirtcpu table field if the old process has been preempted.  */
		if (XINUSCHED == 2) {
			/* Lab3 3.2: Add currproctime to the pvirtcpu table field of the old process   */
			ptold -> pvirtcpu += currproctime;

			/* Update its priority  */
			if (currpid > 0)	/* The priority of prnull should remain zero	*/
				ptold -> prprio = MAXPRIO - ptold -> pvirtcpu;
		}

		insert(currpid, readylist, ptold->prprio);
	}

	/*
	 * User: wang4113
	 * date: 09/19/2018
	 */
	/* Lab2 3.2: Add currproctime to the pgrosscpu table field of the old process	*/
	ptold -> pgrosscpu += currproctime;
	currproctime = 0;	/* Reset to currproctime to zero	*/

    /* Lab3 3.2: We don't need update pvirtcpu table field
     * if the old process voluntarily relinquish the CPU. (I/O, sleep)    */


    /* Force context switch to highest priority ready process */

	currpid = dequeue(readylist);
	ptnew = &proctab[currpid];
	/*
	 * User: wang4113
	 * date: 09/19/2018
	 */
	/* Lab2 3.3: Add pwaittime to the new process	*/
	ptnew -> pwaittime += clktimemilli - (ptnew -> pstartwait);

	ptnew->prstate = PR_CURR;


	preempt = QUANTUM;		/* Reset time slice for process	*/
	ctxsw(&ptold->prstkptr, &ptnew->prstkptr);

	/* Old process returns here when resumed */

	return;
}

/*------------------------------------------------------------------------
 *  resched_cntl  -  Control whether rescheduling is deferred or allowed
 *------------------------------------------------------------------------
 */
status	resched_cntl(		/* Assumes interrupts are disabled	*/
	  int32	defer		/* Either DEFER_START or DEFER_STOP	*/
	)
{
	switch (defer) {

	    case DEFER_START:	/* Handle a deferral request */

		if (Defer.ndefers++ == 0) {
			Defer.attempt = FALSE;
		}
		return OK;

	    case DEFER_STOP:	/* Handle end of deferral */
		if (Defer.ndefers <= 0) {
			return SYSERR;
		}
		if ( (--Defer.ndefers == 0) && Defer.attempt ) {
			resched();
		}
		return OK;

	    default:
		return SYSERR;
	}
}
