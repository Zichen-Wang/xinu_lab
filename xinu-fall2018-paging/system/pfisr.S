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
        cli                             # Disable further interrupts

        pushal                          # Save registers
        call    pfhandler               # Call C function to handle the interrupt
        popal                           # Restore registers

        addl    $4, %esp                # Remove error code from stack
        sti
        iret                            # Return from interrupt
