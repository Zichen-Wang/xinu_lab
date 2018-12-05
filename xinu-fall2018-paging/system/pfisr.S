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

        pushl   %ebx                    # Push fake arguments to avoid system low-level issues
        pushl   36(%esp)                # Push page fault error code

        call    pfhandler               # Call C function to handle the interrupt
        sti			                    # Restore interrupt status

        addl    $8, %esp                   # Discard parameters
        popal			                # Restore registers
        addl    $4, %esp                   # Discard eprror code

        iret                            # Return from interrupt
