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

    prptr = &proctab[ppid]; /* Retrieve the process table of the parent process    */

    if (prptr -> prstate == PR_FREE) {  /* Check whether the parent process has terminated. */
        ppid = -1;
        /*  Question: What should we do if the parent process has terminated before the child process?  */
        /*  In x86, when a process terminates, its child process will change his parent to 'init'. */
        /*  However, I cannot find such operation in system/kill.c */
    }


    restore(mask);

    return ppid;

}
