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

    pd_t	    *pd;
    uint32      bs_counter;

    bsd_t       bs_map_id;
    uint16      allo_hsize, allo_offset;


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

    /*
     * user: wang4113
     * data: 11/04/2018
     */

    /* Create a new page directory */
    pd = create_pd(pid);

    if (pd == (pd_t *)(SYSERR)) {
        kprintf("Initialization of page directory for the new process failed!\n");
        restore(mask);
        return SYSERR;
    }

    /*
     * user: wang4113
     * data: 12/03/2018
     */

    /* Check whether we have enough backing stores  */
    bs_counter = 0;
    for (i = 0; i < MAX_BS_ENTRIES; i++) {
        if (backing_store_map[i].bs_state == BS_FREE) {
            bs_counter++;
        }
    }

    if (hsize_in_pages > bs_counter * 200) {
        kprintf("Not enough backing stores!\n");
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


    /* Assign shared page tables to page directory of the new process	*/

    prptr -> page_directory = pd;
    prptr -> hsize = hsize_in_pages;

    for (i = 0; i < 4; i++) {
        pd[i].pd_pres     = 1;
        pd[i].pd_write    = 1;
        pd[i].pd_user     = 0;
        pd[i].pd_pwt      = 0;
        pd[i].pd_pcd      = 0;
        pd[i].pd_acc      = 0;
        pd[i].pd_mbz      = 0;
        pd[i].pd_fmb      = 0;
        pd[i].pd_global   = 0;
        pd[i].pd_avail    = 0;

        pd[i].pd_base     = ((uint32) shared_page_table[i]) / NBPG;
    }

    pd[DEVICE_PD].pd_pres    = 1;
    pd[DEVICE_PD].pd_write   = 1;
    pd[DEVICE_PD].pd_user    = 0;
    pd[DEVICE_PD].pd_pwt	 = 0;
    pd[DEVICE_PD].pd_pcd	 = 0;
    pd[DEVICE_PD].pd_acc	 = 0;
    pd[DEVICE_PD].pd_mbz	 = 0;
    pd[DEVICE_PD].pd_fmb	 = 0;
    pd[DEVICE_PD].pd_global  = 0;
    pd[DEVICE_PD].pd_avail   = 0;

    pd[DEVICE_PD].pd_base    = ((uint32) shared_page_table[4] / NBPG);


    memptr = &(prptr -> vmemlist);

    memptr -> mnext = (struct memblk *)(NBPG * VHEAP_ST);
    memptr -> mlength = NBPG * hsize_in_pages;

    /* We cannot initialize the first block, since we are now in the context of parent  */
    /* Initializing it now will cause page fault in the context of parent   */
    /*
    memptr = memptr -> mnext;
    memptr -> mnext = NULL;
    memptr -> mlength = NBPG * hsize_in_pages;
    */
    /* We should delay this initialization until the first vgetmem()    */
    prptr -> vmem_init = FALSE;

    /*
     * user: wang4113
     * data: 12/03/2018
     */

    /* Allocate backing stores  */
    allo_offset = VHEAP_ST;         /* The offset when allocating backing stores    */
    while (hsize_in_pages > 0) {    /* The remaining unallocated pages  */

        if (hsize_in_pages <= MAX_PAGES_PER_BS) {
            allo_hsize = hsize_in_pages;
        }
        else {
            allo_hsize = MAX_PAGES_PER_BS;
        }

        hsize_in_pages -= allo_hsize;

        bs_map_id = allocate_bs(allo_hsize);    /* Allocate a backing store */

        if (bs_map_id == SYSERR) {
            kprintf("Cannot allocate a free backing store!\n");
            kill(pid);
            return SYSERR;
        }

        /* Initialize the backing store map */
        backing_store_map[bs_map_id].bs_state = BS_USED;
        backing_store_map[bs_map_id].pid = pid;
        backing_store_map[bs_map_id].virt_base_num = allo_offset;
        backing_store_map[bs_map_id].npages = allo_hsize;

        //kprintf("Allocate bs %d with base %d, npages %d\n", bs_map_id, allo_offset, allo_hsize);

        allo_offset += allo_hsize;      /* Update the offset    */

    }


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
