/*
 * User: wang4113
 * date: 10/17/2018
*/

/* do_handler.S - do_handler (for x86) */

		.text
		.globl	do_handler

/*------------------------------------------------------------------------------
 * do_handler -  remember the original return address and call callback function
 *------------------------------------------------------------------------------
 */

do_handler:
        /* We do not need disable interrupts since it has been disabled when context switching  */

        pushl   %ebp            /* Push ebp onto stack		*/
        movl    %esp, %ebp      /* Record current SP in ebp	*/
                                /* We do not need save other registers
                                 * since EAX, ECX, EDX will be restored by the caller of ctxsw(),
                                 * and the rest will be saved by callee function and restored when callee returns
                                 */
        pushfl                  /* Push flags onto the stack	*/

        sti                     /* Interrupts enabled   */
        call    *8(%ebp)        /* Call callback function   */
        cli                     /* Interrupts disabled  */

        popfl                   /* Restore EFLAGS   */

        movl    %ebp, %esp      /* Restore stack pointer    */
        popl    %ebp            /* Restore stack base   */
        pushl   (%esp)          /* Push the original return address in stack and be ready for making a return    */
        ret                     /* Return to the original address   */
