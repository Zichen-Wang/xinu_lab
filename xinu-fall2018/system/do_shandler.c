/*
 * User: wang4113
 * date: 10/17/2018
*/

/* do_shandler.c - do_shandler */

#include <xinu.h>

/*---------------------------------------
 * do_shandler -  ROP for signal handler
 *---------------------------------------
 */

void do_shandler(uint32 arg)     /* `org' is prptr -> prstkptr + 48  */
{
    intmask mask;           /* Saved interrupt mask     */
    struct  procent *prptr;     /* Ptr to process's table entry */

    mask = disable();       /* Disable interrupts  */

    prptr = &proctab[currpid];

    kprintf("%d\n", arg);
    if (   (prptr -> prsig)[SIGRECV].regyes == TRUE
        && (arg & 1)) {    /* `arg' should be `01' or `11' */

        asm volatile ("sti");       /* Enable interrupts    */
        (prptr -> prsig)[SIGRECV].fnt();    /* Call callback function for SIGRECV   */
        asm volatile ("cli");       /* Disable interrupts   */
    }
    if (   (prptr -> prsig)[SIGTIME].regyes == TRUE
        && (arg & 2)
        && (prptr -> prsig)[SIGTIME].optarg > 0
        && (prptr -> prsig)[SIGTIME].optarg <= clktimemilli) {    /* `arg' should be `10' or `11' */

        /* Clear the alarm	*/
        //(prptr -> prsig)[SIGTIME].optarg = 0;

        asm volatile ("sti");       /* Enable interrupts    */
        (prptr -> prsig)[SIGTIME].fnt();    /* Call callback function for SIGTIME   */
        asm volatile ("cli");       /* Disable interrupts   */

    }

    restore(mask);          /* Restore mask    */
    asm volatile ("movl -4(%ebp), %ebx\n\t"     /* Restore the value of EBX   */
                  "movl %ebp, %esp\n\t"         /* Have the stack frame registers restored  */
                  "popl %ebp\n\t"
                  "pushl (%esp)\n\t"            /* Push the true return address */
                  "ret\n\t");                   /* Make a return    */
}
