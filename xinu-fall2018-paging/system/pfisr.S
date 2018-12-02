/*
 * user: wang4113
 * data: 11/02/2018
 */

/* pfisr.S - pfisr (x86) */

/*------------------------------------------------------------------------
 * pfisr  -  Interrupt dispatcher for page fault (x86 version)
 *------------------------------------------------------------------------
 */

		.text
		.globl	pfisr		            # Page fault interrupt dispatcher
pfisr:

        pushal                          # Save registers
        movl %cr2, %ebx # Cannot push cr registers directly to the stack
        pushl %ebx
        movl %cr3, %ebx
        pushl %ebx
        cli                             # Disable further interrupts
        call    pfhandler               # Call C function to handle the interrupt
        sti			    # Restore interrupt status
        addl $8, %esp   # Discard parameters
        popal			# Restore registers
        addl $4, %esp   # Discard error code
        iret                            # Return from interrupt
