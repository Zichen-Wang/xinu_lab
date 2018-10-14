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
		 * date: 10/13/2018
		 */
		/* Lab2 5.4: In R3 mode, if a process consumes all of its time slice the CPU,
         *   its priority is demoted to INITPRIO	*/
		if (XINUSCHED == 1) {
			if (currpid != NULLPROC && proctab[currpid].prrms == FALSE)
				/* The priority of prnull or real-time process should remain unchanged	*/
				proctab[currpid].prprio = INITPRIO;
		}
        /*
         * User: wang4113
         * date: 10/13/2018
         */
		//preempt = QUANTUM;
        if (ptnew -> prrms == FALSE) {
            preempt = QUANTUM;
        }
        else if (ptnew -> prrms == TRUE) {	/* Lab3 4.3: use RMSQUANTUM for real-time process	*/
            preempt = RMSQUANTUM;
        }
		resched();
	}
}
