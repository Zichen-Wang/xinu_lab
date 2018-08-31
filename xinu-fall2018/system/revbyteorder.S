/* revbyteorder.S - revbytorder (x86) */

    .text
    .globl  revbyteorder
/*------------------------------------------------------------------------
 * revbytorder  -  reverses the byte order of argument x (x86 version)
 *------------------------------------------------------------------------
 */
revbyteorder:
    pushl   %ebp            # save the value of %ebp
    movl    %esp, %ebp      # %ebp now points to the bottom of the stack

    movl    8(%ebp), %eax   # put the value of argument x to %eax

    bswap   %eax            # reverse the byte of %eax

    movl    %ebp, %esp      # restore %esp to its old value
    popl    %ebp            # restore the old value of %ebp to its old value
    ret                     # return to the calling function
