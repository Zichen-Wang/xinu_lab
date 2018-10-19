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

    if (nsig == SIGRECV) {

    }

    restore(mask);              /* Restore interrupts */
    return OK;
}