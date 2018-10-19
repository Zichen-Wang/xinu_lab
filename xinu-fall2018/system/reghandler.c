/*
 * User: wang4113
 * date: 10/17/2018
*/
/* reghandler.c - reghandler */

#include <xinu.h>

/*-------------------------------------------------------------------------------
 * reghandler - used by an application to specify a user space callback function
 * that should be executed when a message is received
 *-------------------------------------------------------------------------------
 */

syscall reghandler(void (* func) (void))
{
    struct	procent	*prptr;		/* Pointer to proc. table entry */
    intmask mask;               /* Saved interrupt mask		*/
    mask = disable();           /* Disable interrupts   */

    prptr = &proctab[currpid];  /* Retrieve the process table of the current process    */
    if (func == NULL) {         /* If the pointer of callback function is NULL, return system error */
        return SYSERR;
    }

    prptr -> callback_func_addr = (uint32)func;  /* Register the callback function   */

    restore(mask);              /* Restore interrupts */
    return OK;
}
