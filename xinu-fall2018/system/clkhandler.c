/* clkhandler.c - clkhandler */

#include <xinu.h>

/*------------------------------------------------------------------------
 * clkhandler - high level clock interrupt handler
 *------------------------------------------------------------------------
 */
void	clkhandler()
{
	static	uint32	count1000 = 1000;	/* Count to 1000 ms	*/
	pid32	i;

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

	/*
	 * User: wang4113
	 * date: 10/21/2018
	 */

	/* if the process registered a callback function for SIGXCPU,
	 * check whether the current process has reached the XCPU time 	*/
	if ((proctab[currpid].prsig)[SIGXCPU].regyes == TRUE
	 && proctab[currpid].pgrosscpu + currproctime == (proctab[currpid].prsig)[SIGXCPU].optarg) {

		asm volatile ("sti");		/* Enable interrupts	*/
		(proctab[currpid].prsig)[SIGXCPU].fnt();	/* Call callback function for SIGXCPU	*/
		asm volatile ("cli");		/* Disable interrupts	*/
	}

	/* Check processes whose alarm should ring	*/
	for (i = 0; i < NPROC; i++)
		if (proctab[i].prstate != PR_FREE
		 && (proctab[i].prsig)[SIGTIME].regyes == TRUE
		 && (proctab[i].prsig)[SIGTIME].optarg == clktimemilli) {
			/* We find a process whose alarm should ring	*/

			if (i == currpid) {
				/* The current process is the process that registered a handler for SIGTIME	*/

				/* Clear the alarm	*/
				(proctab[i].prsig)[SIGTIME].optarg = 0;

				asm volatile ("sti");		/* Enable interrupts	*/
				(proctab[currpid].prsig)[SIGTIME].fnt();	/* Call callback function for SIGTIME	*/
				asm volatile ("cli");		/* Disable interrupts	*/

				kprintf("%d\n", i);

			}
			else {

				/* The current process is NOT the process that registered a handler for SIGTIME	*/

				/* proctab[i].prstkptr + 48 and proctab[i].prstkptr + 44 are useless, so save the addresses there	*/

				/* modify proctab[i].prstkptr + 48 indicates that there is an asynchronous message	*/
				/* `00' means nothing; `01' means an asynchronous message; `10' means an alarm; `11' means both	*/
				if (!(*(int *)(proctab[i].prstkptr + 48) >= 0 && *(int *)(proctab[i].prstkptr + 48) < 4))
					*(int *)(proctab[i].prstkptr + 48) = 0;

				*(int *)(proctab[i].prstkptr + 48) |= 2;	/* add `10'	*/

				/* Save the original return address	into proctab[i].prstkptr + 44 */
				*(int *) (proctab[i].prstkptr + 44) = *(int *) (proctab[i].prstkptr + 40);

				/* modify the return address which is at proctab[i].prstkptr + 40 to do_shandler()	*/
				*(int *) (proctab[i].prstkptr + 40) = (uint32) do_shandler;

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
        if (proctab[currpid].prrms == FALSE) {
            preempt = QUANTUM;
        }
        else if (proctab[currpid].prrms == TRUE) {	/* Lab3 4.3: use RMSQUANTUM for real-time process	*/
            preempt = RMSQUANTUM;
        }
		resched();
	}
}
