/*
 * User: wang4113
 * date: 10/17/2018
*/

/* do_handler.S - do_handler (for x86) */

		.text
		.globl	do_handler

/*------------------------------------------------------------------------------
 * do_handler -  remember the orginial return address and call callback function
 *------------------------------------------------------------------------------
 */

do_handler:
        pushl   %ebp            /* Push ebp onto stack		*/
        movl    %esp, %ebp      /* Record current SP in ebp	*/

        pushfl                  /* Push flags onto the stack	*/

        sti                     /* Interrupts enabled   */
        call    *4(%ebp)        /* Call callback function   */
        cli                     /* Interrupts disabled  */

        popfl                   /* Restore EFLAGS   */

        movl    %ebp, %esp      /* Restore stack pointer    */
        popl    %ebp            /* Restore stack base   */
        pushl   4(%esp)         /* Push the original return address in stack and be ready for making a return    */
        ret                     /* Return to the original address   */
