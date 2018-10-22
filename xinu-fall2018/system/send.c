/* send.c - send */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  send  -  Pass a message to a process and start recipient if waiting
 *------------------------------------------------------------------------
 */
syscall	send(
	  pid32		pid,		/* ID of recipient process	*/
	  umsg32	msg		/* Contents of message		*/
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process's table entry	*/

	mask = disable();
	if (isbadpid(pid)) {
		restore(mask);
		return SYSERR;
	}

	prptr = &proctab[pid];
	if (prptr->prhasmsg) {
		restore(mask);
		return SYSERR;
	}
	prptr->prmsg = msg;		/* Deliver message		*/
	prptr->prhasmsg = TRUE;		/* Indicate message is waiting	*/

	/*
	 * User: wang4113
	 * date: 10/18/2018
	 */
	if (currpid != pid	/* Do not allow sending a message to the process itself under asynchronous IPC.	*/
		&& prptr -> prstate != PR_RECV
		&& prptr -> prstate != PR_RECTIM
		&& prptr -> callback_func != NULL) {
		/* If the receiver is non-block by a receive() and has registered a callback function,
		 * modify the stack of the receiver process.	*/


		/* Since the two arguments of ctxsw() are the addresses of old process pointer and new process pointer
		 * which are useless after context switch, so we can use the places of these two arguments to save
		 * the address of callback function and the original return address respectively.
		 */


		/* prptr -> prstkptr + 48 and prptr -> prstkptr + 44 are useless, so save the addresses there	*/

		/* Save the address of callback function into prptr -> prstkptr + 4	*/
		*(int *)(prptr -> prstkptr + 48) = (uint32)prptr -> callback_func;

		/* Save the original return address	into prptr -> prstkptr + 48 */
		*(int *)(prptr -> prstkptr + 44) = *(int *)(prptr -> prstkptr + 40);

		/* modify the return address which is at prptr -> prstkptr + 40 to do_handler()	*/
		*(int *)(prptr -> prstkptr + 40) = (uint32)do_handler;

	}

	if (currpid != pid	/* Do not allow sending a message to the process itself under asynchronous IPC.	*/
		&& prptr -> prstate != PR_RECV
		&& prptr -> prstate != PR_RECTIM
		&& ((prptr -> prsig)[SIGRECV]).regyes == TRUE) {
		/* If the receiver is non-block by a receive() and has registered a callback function of SIGRECV,
		 * modify the stack of the receiver process.	*/


		/* prptr -> prstkptr + 44 is useless, so save the original address there	*/

		/* Save the original return address	into prptr -> prstkptr + 44 */
		*(int *)(prptr -> prstkptr + 44) = *(int *)(prptr -> prstkptr + 40);

		/* modify the return address which is at prptr -> prstkptr + 40 to do_shandler()	*/
		*(int *)(prptr -> prstkptr + 40) = (uint32)do_handler;

		do_shandler();

	}

	/* If recipient waiting or in timed-wait make it ready */

	if (prptr->prstate == PR_RECV) {
		ready(pid);
	} else if (prptr->prstate == PR_RECTIM) {
		unsleep(pid);
		ready(pid);
	}
	restore(mask);		/* Restore interrupts */
	return OK;
}
