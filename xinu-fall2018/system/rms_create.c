/*
* User: wang4113
* date: 10/08/2018
*/

/* rms_create.h - rms_create */

#include <xinu.h>

local	int newpid();

/*--------------------------------------------------------------
 *  rms_create  -  Create a real-time process in Lab3 4.1 & 4.2
 *--------------------------------------------------------------
 */

pid32   create(
    void		*funcaddr,	    /* Address of the function	    */
    uint32	    ssize,		    /* Stack size in bytes		    */
    rmsparam_t	*ptrrmsparam,	/* A pointer to	struct rmsparam	*/
    char		*name,		    /* Name (for debugging)		    */
    uint32	    nargs,		    /* Number of args that follow	*/
    ...
    )
{
    uint32		savsp, *pushsp;
    intmask 	mask;    	/* Interrupt mask		*/
    pid32		pid;		/* Stores new process id	*/
    struct	    procent	*prptr;		/* Pointer to proc. table entry */
    int32		i;
    uint32		*a;		/* Points to list of args	*/
    uint32		*saddr;		/* Stack address		*/

    double      ad;     /* admission control test   */

    mask = disable();
    if (ssize < MINSTK)
        ssize = MINSTK;
    ssize = (uint32) roundmb(ssize);
    if ( (priority < 1) || ((pid=newpid()) == SYSERR) ||
         ((saddr = (uint32 *)getstk(ssize)) == (uint32 *)SYSERR) ) {
        restore(mask);
        return SYSERR;
    }

    if (ptrrmsparam -> rms_period > 1000 || ptrrmsparam -> rms_period < 20) {
        /* disallow periods greater than 1 second or smaller than 20 msec.  */
        restore(mask);
        return SYSERR;
    }

    ad = 0.0;       /* Initialize admission control to zero */
    /*  Count all real-time processes */
    for (i = 1; i < NPROC; i++) {
        if (proctab[nextpid].prstate != PR_FREE && proctab[i].prrms == 1) {
            ad += 1.0 * proctab[i].prct / proctab[i].prperiod;
        }
    }

    if (ad + 1.0 * ptrrmsparam -> rms_ct / ptrtmsparam -> rms_period > RMSBOUND) {
        /* admission control test failed    */
        restore(mask);
        return SYSERR;
    }

    prcount++;
    prptr = &proctab[pid];

    /* Initialize process table entry for new process */
    prptr->prstate = PR_SUSP;	/* Initial state is suspended	*/

    prptr -> prprio = 30000 - ptrrmsparam -> rms_period;    /* Lab3 4.3: set real-time process priority */

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

    /* Initialize the gross CPU usage of a process to zero	*/
    prptr -> pgrosscpu = 0;

    /* Initialize the waiting count and waiting time of a process to zero	*/
    prptr -> pwaittime = 0;
    prptr -> pwaitcount = 0;
    prptr -> pstartwait = 0;

    /* Lab3 4.1: real-time process	*/
    prptr -> prrms = 1;
    prptr -> prct = ptrrmsparam -> rms_ct;
    prptr -> prperiod = ptrrmsparam -> rms_period;

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
