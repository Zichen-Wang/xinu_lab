/*
 * User: wang4113
 * date: 10/17/2018
*/

/* do_handler.c - do_shandler */

#incldue <xinu.h>

/*---------------------------------------
 * do_shandler -  ROP for signal handler
 *---------------------------------------
 */

void do_shandler()
{
    intmask	mask;			/* Saved interrupt mask		*/
    struct	procent *prptr;		/* Ptr to process's table entry	*/

    mask = disable();       /* Disable interrupts  */

    prptr = &proctab[currpid];

    if (((prptr -> prsig)[SIGRECV]).regyes == TRUE) {
        asm volatile ("sti");		/* Enable interrupts	*/
        (prptr -> prsig)[SIGRECV].fnt();	/* Call callback function for SIGRECV	*/
        asm volatile ("cli");		/* Disable interrupts	*/
    }

    if (((prptr -> prsig)[SIGTIME]).regyes == TRUE
     && ((prptr -> prsig)[SIGTIME]).optarg > 0
     && ((prptr -> prsig)[SIGTIME]).optarg <= clktimemilli) {
        /* when the scheduler decides to run this process,
         * the alarm time should be non-zero and equal to or lower than clktimemilli    */

        /* Clear the alarm  */
        ((prptr -> prsig)[SIGTIME]).optarg = 0;

        asm volatile ("sti");		/* Enable interrupts	*/
        (prptr -> prsig)[SIGTIME].fnt();	/* Call callback function for SIGTIME	*/
        asm volatile ("cli");		/* Disable interrupts	*/

    }

    restore(mask);          /* Restore mask    */


}
