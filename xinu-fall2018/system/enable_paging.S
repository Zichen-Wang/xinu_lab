/*
 * user: wang4113
 * data: 11/02/2018
 */

/* enable_paging.S - enable_paging (x86) */

/*------------------------------------------------------------------------
 * enable_paging  -  set PG in CR0 register (x86 version)
 *------------------------------------------------------------------------
 */

        .text
        .globl  enable_paging

enable_paging:
        pushl   %ebp
        movl    %esp, %ebp

        pushfl  /* Save EFLAGS  */

        cli     /* Interrupt disabled   */
        movl    %cr0, %eax
        orl     $0x80000000, %eax   /* Set PG = 1    */
        movl    %eax, %cr0

        popfl   /* Restore EFLAGS   */

        movl    %ebp, %esp
        popl    %ebp
        ret