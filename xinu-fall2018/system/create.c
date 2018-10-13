/* create.c - create, newpid */

#include <xinu.h>

local	int newpid();

/*------------------------------------------------------------------------
 *  create  -  Create a process to start running a function on x86
 *------------------------------------------------------------------------
 */
pid32	create(
	  void		*funcaddr,	/* Address of the function	*/
	  uint32	ssize,		/* Stack size in bytes		*/
	  pri16		priority,	/* Process priority > 0		*/
	  char		*name,		/* Name (for debugging)		*/
	  uint32	nargs,		/* Number of args that follow	*/
	  ...
	)
{
	uint32		savsp, *pushsp;
	intmask 	mask;    	/* Interrupt mask		*/
	pid32		pid;		/* Stores new process id	*/
	struct	procent	*prptr;		/* Pointer to proc. table entry */
	int32		i;
	uint32		*a;		/* Points to list of args	*/
	uint32		*saddr;		/* Stack address		*/

	/*
	 * User: wang4113
	 * date: 10/07/2018
	 */
	qid16   curr;           /* Lab3 3.2: Runs through items in a queue   */
	uint32  max_pvirtcpu;   /* Lab3 3.2: Maximum virtual CPU usage      */

	mask = disable();
	if (ssize < MINSTK)
		ssize = MINSTK;
	ssize = (uint32) roundmb(ssize);
	if ( (priority < 1) || ((pid=newpid()) == SYSERR) ||
	     ((saddr = (uint32 *)getstk(ssize)) == (uint32 *)SYSERR) ) {
		restore(mask);
		return SYSERR;
	}

	prcount++;
	prptr = &proctab[pid];

	/* Initialize process table entry for new process */
	prptr->prstate = PR_SUSP;	/* Initial state is suspended	*/
	/*
	 * User: wang4113
	 * date: 10/08/2018
	 */
	if (XINUSCHED == 0) {        /* Lab2 5.3: If in legacy mode, the priority is equal to the argument 'priority' */
		if (priority >= 29000) {
			/* Lab3 4.3: To ensure that non-real-time process priorities do not exceed 29000,
 			* so that process creation with static priority greater than or equal to 29000 is not allowed	*/
			restore(mask);
			return (pid32) SYSERR;
		}
		prptr->prprio = priority;
	}
	else if (XINUSCHED == 1)	/* Lab2 5.3: If in R3 mode, the initial priority is equal to INITPRIO	*/
		prptr->prprio = INITPRIO;
	else if (XINUSCHED == 2) {
		/* Lab3 3.2: If in CFS mode, set virtual CPU usage first, and set its priority	*/
		/* Find maximum CPU usage across all ready/current processes */

		/* Initialize max_pvirtcpu to zero */
		max_pvirtcpu = 0;

		/* Scan the ready list to find the process with maximum virtual CPU usage   */
		curr = firstid(readylist);
		while (curr != queuetail(readylist)) {
			if (curr > 0 && max_pvirtcpu < proctab[curr].pvirtcpu) {
				max_pvirtcpu = proctab[curr].pvirtcpu;  /* Update the maximum virtual CPU usage */
			}
			curr = queuetab[curr].qnext;
		}

		/*	Compare to current process virtual CPU usage	*/
		int nowvirtcpu = proctab[currpid].pvirtcpu + currproctime;
		if (max_pvirtcpu < nowvirtcpu)
			max_pvirtcpu = nowvirtcpu;
		prptr -> pvirtcpu = max_pvirtcpu;			/* Set the virtual CPU usage of the new process    */
		prptr -> prprio = MAXPRIO - max_pvirtcpu;   /* Set the priority of the new process    */
	}


	prptr->prstkbase = (char *)saddr;
	prptr->prstklen = ssize;
	prptr->prname[PNMLEN-1] = NULLCH;
	for (i=0 ; i<PNMLEN-1 && (prptr->prname[i]=name[i])!=NULLCH; i++)
		;
	prptr->prsem = -1;
	prptr->prparent = (pid32)getpid();
	prptr->prhasmsg = FALSE;

	/* Set up stdin, stdout, and stderr descriptors for the shell	*/
	prptr->prdesc[0] = CONSOLE;
	prptr->prdesc[1] = CONSOLE;
	prptr->prdesc[2] = CONSOLE;

	/* Initialize stack as if the process was called		*/

	*saddr = STACKMAGIC;
	savsp = (uint32)saddr;

	/* Push arguments */
	a = (uint32 *)(&nargs + 1);	/* Start of args		*/
	a += nargs -1;			/* Last argument		*/
	for ( ; nargs > 0 ; nargs--)	/* Machine dependent; copy args	*/
		*--saddr = *a--;	/* onto created process's stack	*/
	*--saddr = (long)INITRET;	/* Push on return address	*/

	/* The following entries on the stack must match what ctxsw	*/
	/*   expects a saved process state to contain: ret address,	*/
	/*   ebp, interrupt mask, flags, registers, and an old SP	*/

	*--saddr = (long)funcaddr;	/* Make the stack look like it's*/
					/*   half-way through a call to	*/
					/*   ctxsw that "returns" to the*/
					/*   new process		*/
	*--saddr = savsp;		/* This will be register ebp	*/
					/*   for process exit		*/
	savsp = (uint32) saddr;		/* Start of frame for ctxsw	*/
	*--saddr = 0x00000200;		/* New process runs with	*/
					/*   interrupts enabled		*/

	/* Basically, the following emulates an x86 "pushal" instruction*/

	*--saddr = 0;			/* %eax */
	*--saddr = 0;			/* %ecx */
	*--saddr = 0;			/* %edx */
	*--saddr = 0;			/* %ebx */
	*--saddr = 0;			/* %esp; value filled in below	*/
	pushsp = saddr;			/* Remember this location	*/
	*--saddr = savsp;		/* %ebp (while finishing ctxsw)	*/
	*--saddr = 0;			/* %esi */
	*--saddr = 0;			/* %edi */
	*pushsp = (unsigned long) (prptr->prstkptr = (char *)saddr);

	/*
	 * User: wang4113
	 * date: 09/19/2018
	 */
	/* Lab2 3.2: Initialize the gross CPU usage of a process to zero	*/
	prptr -> pgrosscpu = 0;

	/* Lab2 3.3: Initialize the waiting count and waiting time of a process to zero	*/
	prptr -> pwaittime = 0;
	prptr -> pwaitcount = 0;
	prptr -> pstartwait = 0;


	/*
	 * User: wang4113
	 * date: 10/08/2018
	 */
	prptr -> prrms = 0;		/* Lab3 4.1: non-real-time process	*/

	restore(mask);
	return pid;
}

/*------------------------------------------------------------------------
 *  newpid  -  Obtain a new (free) process ID
 *------------------------------------------------------------------------
 */
local	pid32	newpid(void)
{
	uint32	i;			/* Iterate through all processes*/
	static	pid32 nextpid = 1;	/* Position in table to try or	*/
					/*   one beyond end of table	*/

	/* Check all NPROC slots */

	for (i = 0; i < NPROC; i++) {
		nextpid %= NPROC;	/* Wrap around to beginning */
		if (proctab[nextpid].prstate == PR_FREE) {
			return nextpid++;
		} else {
			nextpid++;
		}
	}
	return (pid32) SYSERR;
}
