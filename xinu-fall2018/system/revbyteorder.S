/* revbyteorder.S - revbytorder (x86) */

    .text
    .globl  revbyteorder
/*------------------------------------------------------------------------
 * revbytorder  -  reverses the byte order of argument x (x86 version)
 *------------------------------------------------------------------------
 */
revbyteorder:
    pushl   %ebp            # save the value of %ebp
    movl    %esp, %ebp      # %ebp now points to the top of the stack
    subl    $4, %esp

    movl    %eax, 8(%ebp)   # put the value of argument x to %eax

    movb    %al, -1(%ebp)
    movb    %ah, -2(%ebp)
    shr     $2, %eax
    movb    %al, -3(%ebp)
    movb    %ah, -4(%ebp)

    movb    -1(%ebp), %ah
    movb    -2(%ebp), %al
    shl     $2, %eax
    movb    -3(%ebp), %ah
    movb    -4(%ebp), %al

    movl    %ebp, %esp      # restore %esp to its old value
    popl    %ebp            # restore the old value of %ebp to its old value
    ret                     # return to the calling function
