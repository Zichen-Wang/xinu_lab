/* ctxsw.S - ctxsw (for x86) */

		.text
		.globl	ctxsw
/*
 * user: wang4113
 * data: 11/06/2018
 */
/*-------------------------------------------------------------------------
 * ctxsw -  X86 context switch; the call is ctxsw(&old_sp, &new_sp, new_pd)
 *-------------------------------------------------------------------------
 */
ctxsw:
		pushl	%ebp		/* Push ebp onto stack		*/
		movl	%esp,%ebp	/* Record current SP in ebp	*/
		pushfl			/* Push flags onto the stack	*/
		pushal			/* Push general regs. on stack	*/

		/* Save old segment registers here, if multiple allowed */

		movl	8(%ebp),%eax	/* Get mem location in which to	*/
					/*   save the old process's SP	*/
		movl	%esp,(%eax)	/* Save old process's SP	*/
		movl	12(%ebp),%eax	/* Get location from which to	*/
					/*   restore new process's SP	*/

		/*
         * user: wang4113
         * data: 11/06/2018
         */
		movl    16(%ebp), %ecx  /* Get new process's page directory */
		movl    %ecx, %cr3      /* Set PDBR register for new process    */

		/* The next instruction switches from the old process's	*/
		/*   stack to the new process's stack.			*/

		movl	(%eax),%esp	/* Pop up new process's SP	*/

		/* Restore new seg. registers here, if multiple allowed */

		popal			/* Restore general registers	*/
		movl	4(%esp),%ebp	/* Pick up ebp before restoring	*/
					/*   interrupts			*/
		popfl			/* Restore interrupt mask	*/
		add	$4,%esp		/* Skip saved value of ebp	*/
		ret			/* Return to new process	*/
