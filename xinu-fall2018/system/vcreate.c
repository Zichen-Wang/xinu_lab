/*
 * user: wang4113
 * data: 11/02/2018
 */

/* vcreate.c - vcreate, newpid */

#include <xinu.h>

local	int newpid();

#define	roundew(x)	( (x+3)& ~0x3)

/*------------------------------------------------------------------------------------
 *  vcreate  -  Create a process with virtual heap to start running a function on x86
 *------------------------------------------------------------------------------------
 */
pid32	vcreate(
        void		*funcaddr,	/* Address of the function	*/
        uint32	ssize,		/* Stack size in words		*/
        uint16  hsize_in_pages, /* The size of the heap (in number of pages) */
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

    pd_t	*pd_entry;		/* Ptr to page directory entry	*/
    struct	memblk	*memptr;	/* Ptr to memory block		*/

    mask = disable();
    if (ssize < MINSTK)
        ssize = MINSTK;
    ssize = (uint32) roundew(ssize);
    if (((saddr = (uint32 *)getstk(ssize)) ==
         (uint32 *)SYSERR ) ||
        (pid=newpid()) == SYSERR || priority < 1 ) {
        restore(mask);
        return SYSERR;
    }

    prcount++;
    prptr = &proctab[pid];

    /* Initialize process table entry for new process */
    prptr->prstate = PR_SUSP;	/* Initial state is suspended	*/
    prptr->prprio = priority;
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
        *--saddr = *a--;	/*   onto created process' stack*/
    *--saddr = (long)INITRET;	/* Push on return address	*/

    /* The following entries on the stack must match what ctxsw	*/
    /*   expects a saved process state to contain: ret address,	*/
    /*   ebp, interrupt mask, flags, registerss, and an old SP	*/

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
     * user: wang4113
     * data: 11/02/2018
     */

    /* Create a new page directory */
    prptr -> page_directory = create_pd(pid);

    /* Assign shared page tables to page directory of null process	*/
    for (i = 0; i < 4; i++) {
        pd_entry = (pd_t *)(prptr->page_directory + i * 4);

        pd_entry -> pd_pres		= 1;
        pd_entry -> pd_write	= 1;
        pd_entry -> pd_user		= 0;
        pd_entry -> pd_pwt		= 0;
        pd_entry -> pd_pcd		= 0;
        pd_entry -> pd_acc		= 0;
        pd_entry -> pd_mbz		= 0;
        pd_entry -> pd_fmb		= 0;
        pd_entry -> pd_global	= 0;
        pd_entry -> pd_avail	= 0;

        pd_entry -> pd_base		= ((uint32)shared_page_table[i]) / NBPG;
    }

    /* Assign shared page tables to page directory of null process	*/
    pd_entry = (pd_t *)(prptr -> page_directory + DEVICE_FRAME_BASE / PAGE_TABLE_ENTRIES * 4);

    pd_entry -> pd_pres		= 1;
    pd_entry -> pd_write	= 1;
    pd_entry -> pd_user		= 0;
    pd_entry -> pd_pwt		= 0;
    pd_entry -> pd_pcd		= 0;
    pd_entry -> pd_acc		= 0;
    pd_entry -> pd_mbz		= 0;
    pd_entry -> pd_fmb		= 0;
    pd_entry -> pd_global	= 0;
    pd_entry -> pd_avail	= 0;

    pd_entry -> pd_base		= ((uint32)shared_page_table[4]) / NBPG;


    memptr = &(prptr -> vmemlist);

    memptr -> mnext = (struct memblk *)(NBPG * 4096);
    memptr -> mlength = NBPG * hsize_in_pages;

    memptr = memptr -> mnext;

    kprintf("before\n");
    memptr -> mnext = NULL;         /* first page fault */
    kprintf("after\n");
    memptr -> mlength = NBPG * hsize_in_pages;



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
    return (pid32)(SYSERR);
}
