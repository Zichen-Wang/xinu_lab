/*
 * user: wang4113
 * data: 11/02/2018
 */

/* setCR3.S - setCR3 (x86) */

/*----------------------------------------------
 * setCR3  -  set base address of page directory
 *----------------------------------------------
 */

        .text
        .globl  setCR3

setCR3:
        pushl   %ebp
        movl    %esp, %ebp

        pushfl  /* Save EFLAGS  */

        cli     /* Interrupt disabled   */
        movl    %cr3, %eax
        movl    %eax, 8(%ebp)
        movl    %eax, %cr3

        popfl   /* Restore EFLAGS   */

        movl    %ebp, %esp
        popl    %ebp
        ret
