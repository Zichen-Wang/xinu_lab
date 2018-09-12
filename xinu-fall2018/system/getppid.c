/* getppid.c - getppid */

#include <xinu.h>

/*------------------------------------------------------------------------
 * getppid  -  Return the PID of the calling process's parent process.
 *------------------------------------------------------------------------
 */

syscall getppid(void)
{

    struct	procent	*prptr;		/* Pointer to proc. table entry */
    intmask mask;   /* Saved interrupt mask		*/
    pid32 ppid;


    mask = disable();   /* Disable interrupts   */

    prptr = &proctab[currpid];  /* Retrieve the process table of the current process    */
    ppid = prptr -> prparent;   /* Get the parent process PID   */

    /* Make sure that ppid is a valid number  */
    if (ppid < 0 || ppid > NPROC)
        return -1;

    prptr = &proctab[ppid]; /* Retrieve the process table of the parent process    */

    if (prptr -> prstate == PR_FREE) {  /* Check whether the parent process has terminated */
        ppid = -1;
    }


    restore(mask);

    return ppid;

}
