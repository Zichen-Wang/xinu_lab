/* clkhandler.c - clkhandler */

#include <xinu.h>

/*------------------------------------------------------------------------
 * clkhandler - high level clock interrupt handler
 *------------------------------------------------------------------------
 */
void	clkhandler()
{
	static	uint32	count1000 = 1000;	/* Count to 1000 ms	*/

	/* Decrement the ms counter, and see if a second has passed */

	if((--count1000) <= 0) {

		/* One second has passed, so increment seconds count */

		clktime++;

		/* Reset the local ms counter for the next second */

		count1000 = 1000;
	}

	/*
	 * User: wang4113
	 * date: 09/19/2018
	 */
	/* Lab2 3.1: increment milliseconds count at each clock interrupt	*/
	clktimemilli++;

	/* Lab2 3.2: increment the CPU usage count at each clock interrupt	*/
	currproctime++;


	/* Handle sleeping processes if any exist */

	if(!isempty(sleepq)) {

		/* Decrement the delay for the first process on the	*/
		/*   sleep queue, and awaken if the count reaches zero	*/

		if((--queuetab[firstid(sleepq)].qkey) <= 0) {
			wakeup();
		}
	}

	/* Decrement the preemption counter, and reschedule when the */
	/*   remaining time reaches zero			     */

	if((--preempt) <= 0) {
		/*
		 * User: wang4113
		 * date: 10/07/2018
		 */
		/* Lab2 5.4: In R3 mode, if a process consumes all of its time slice the CPU,
         *   its priority is demoted to INITPRIO	*/
		if (XINUSCHED == 1) {
			if (currpid > 0)		/* The priority of prnull should remain zero	*/
				proctab[currpid].prprio = INITPRIO;
		}
		/* Lab3 3.2: In CFS mode, change its priority	*/
		else if (XINUSCHED == 2) {
			if (currpid > 0) {        /* The priority of prnull should remain zero	*/
				/* Now, the virtual CPU usage is proctab[currpid].pvirtcpu + currproctime	*/
				/* Update the priority accordingly	*/
				proctab[currpid].prprio = MAXPRIO - (proctab[currpid].pvirtcpu + currproctime);
			}
		}
		preempt = QUANTUM;
		resched();
	}
}
