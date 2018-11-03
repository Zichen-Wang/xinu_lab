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
		.globl	pfisr		            # page fault interrupt dispatcher
		.extern _pferrorno              # find the global variable pferrorno
pfisr:
        cli                             # Disable further interrupts
        movl    (%esp), $_pferrorno     # Store error code in a global variable

        pushal                          # Save registers
        call    pfhandler               # Call C function to handle the interrupt

        popal                           # Restore registers
        addl    $4, %esp                # Remove error code from stack
        sti                             # Restore interrupt status
        iret                            # Return from interrupt