/* revbyteorder_asm.S - revbytorder_asm (x86) */

    .text
    .globl  revbyteorder_asm
/*-----------------------------------------------------------------------------
 * revbytorder_asm  -  Reverse the byte order of argument x (x86 assembly version);
                   the call is revbyteorder(long x)
 *-----------------------------------------------------------------------------
 */
revbyteorder:
    pushl   %ebp            # save the value of %ebp
    movl    %esp, %ebp      # %ebp now points to the bottom of the stack

    movl    8(%ebp), %eax   # put the value of argument x to %eax

    bswapl   %eax            # reverse the byte of %eax

    movl    %ebp, %esp      # restore %esp to its old value
    popl    %ebp            # restore the old value of %ebp to its old value
    ret                     # return to the calling function
