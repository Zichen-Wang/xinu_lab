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
        pushfl                          # Save EFLAGS
        cli                             # Disable further interrupts
        pushal                          # Save registers
        call    pfhandler               # Call C function to handle the interrupt
        popal                           # Restore registers

        popfl                           # Restore EFLAGS
        addl    $4, %esp                # Remove error code from stack
        iret                            # Return from interrupt
