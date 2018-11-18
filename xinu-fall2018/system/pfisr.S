/*
 * user: wang4113
 * data: 11/02/2018
 */

/* pfisr.S - pfisr (x86) */

/*------------------------------------------------------------------------
 * pfisr  -  Interrupt dispatcher for page fault (x86 version)
 *------------------------------------------------------------------------
 */

		.data
pferrorno:                              # Define the global variable pferrorno
        .long   0

		.text
		.globl	pfisr		            # Page fault interrupt dispatcher
pfisr:
        pushfl                          # Save EFLAGS
        cli                             # Disable further interrupts
        pushl   %ebx                    # Save the temp register EBX
        movl    4(%esp), %ebx
        movl    %ebx, pferrorno         # Page fault error number; Store error code in a global variable
        popl    %ebx                    # Restore temp register EBX

        pushal                          # Save registers
        call    pfhandler               # Call C function to handle the interrupt
        popal                           # Restore registers

        addl    $4, %esp                # Remove error code from stack
        popfl                           # Restore EFLAGS
        iret                            # Return from interrupt
