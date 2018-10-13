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
    /*
     * User: wang4113
     * date: 10/07/2018
     */

    qid16   curr;           /* Lab3 3.2: Runs through items in a queue   */
	uint32  min_pvirtcpu;   /* Lab3 3.2: Minimum virtual CPU usage      */

	if (isbadpid(pid)) {
		return SYSERR;
	}

	/* Set process state to indicate ready and add to ready list */

	prptr = &proctab[pid];
    /*
     * User: wang4113
     * date: 10/07/2018
     */
    /* Lab3 3.2 */
    /* In CFS mode, assign a blocked(sleep) process that becomes ready a virtual CPU usage value
     * that is the minimum CPU usage across all ready/current processes.
     */
    if (XINUSCHED == 2) {       /* Make sure it is in CFS mode  */
        if (prptr -> prstate == PR_SLEEP) {     /* Process is ready from sleep  */
            /* Find minimum CPU usage across all ready/current processes */

            /* Initialize min_pvirtcpu to MAXPRIO */
            min_pvirtcpu = MAXPRIO;

            /* Scan the ready list to find the process with minimum virtual CPU usage   */
            curr = firstid(readylist);
            while (curr != queuetail(readylist)) {
                if (curr > 0 && min_pvirtcpu > proctab[curr].pvirtcpu) {
                    min_pvirtcpu = proctab[curr].pvirtcpu;  /* Update the minimum virtual CPU usage */
                }
                curr = queuetab[curr].qnext;
            }

            /*	Compare to current process virtual CPU usage	*/
            int nowvirtcpu = proctab[currpid].pvirtcpu + currproctime;
            if (min_pvirtcpu > nowvirtcpu)
                min_pvirtcpu = nowvirtcpu;
            prptr -> pvirtcpu = min_pvirtcpu;           /* Set the virtual CPU usage of the new ready process    */
            prptr -> prprio = MAXPRIO - min_pvirtcpu;   /* Set the priority of the new ready process    */
        }

    }

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
