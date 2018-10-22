/*
 * User: wang4113
 * date: 10/17/2018
*/
/* signalreg.c - signalreg */

#include <xinu.h>

/*-------------------------------------------------------------------------------
 * signalreg - used by an application to specify a user space callback function
 * that should be executed when a signal is raised
 *-------------------------------------------------------------------------------
 */

syscall signalreg(uint16 nsig, int (* func) (void), uint32 oarg)
{
    struct	procent	*prptr;		/* Pointer to proc. table entry */
    intmask mask;               /* Saved interrupt mask		*/

    mask = disable();           /* Disable interrupts   */

    prptr = &proctab[currpid];  /* Retrieve the process table of the current process    */

    if (func == NULL) {         /* If the pointer of callback function is NULL, return system error */
        restore(mask);          /* Restore interrupts */
        return SYSERR;
    }

    if (nsig == SIGRECV) {
        /* Register SIGRECV callback function   */

        ((prptr -> prsig)[SIGRECV]).regyes = TRUE;
        ((prptr -> prsig)[SIGRECV]).fnt = func;

    }
    else if (nsig == SIGXCPU) {
        /* Register SIGXCPU callback function   */

        if (oarg <= prptr -> pgrosscpu + currproctime) {
            /* The XCPU time should be larger than the gross CPU usage of current process    */
            restore(mask);          /* Restore interrupts */
            return SYSERR;
        }

        ((prptr -> prsig)[SIGXCPU]).regyes = TRUE;
        ((prptr -> prsig)[SIGXCPU]).fnt = func;
        ((prptr -> prsig)[SIGXCPU]).optarg = oarg;    /* Save the XCPU time   */

    }
    else if (nsig == SIGTIME) {
        /* Register SIGRECV callback function   */

        ((prptr -> prsig)[SIGTIME]).regyes = TRUE;
        ((prptr -> prsig)[SIGTIME]).fnt = func;
        ((prptr -> prsig)[SIGTIME]).optarg = 0;     /* alarm has not been set   */

    }

    restore(mask);              /* Restore interrupts */
    return OK;
}