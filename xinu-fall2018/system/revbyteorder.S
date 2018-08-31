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

    movl    8(%ebp), %eax   # save the argument x to %eax

    addl    $10, %eax

#   movb    %al, %bl        # save the first (lowest) byte of x to %bl
#   movb    %ah, %bh        # save the second byte to %bh

#   shrl    $2, %eax        # shift right %eax 2 bytes

#   movb    %al, %cl        # save the third byte of x to %cl
#   movb    %ah, %ch        # save the fourth (highest) byte of x to %ch

#   movb    %bh, %al        # put %bl and %bh to the fourth and third byte of x
#   movb    %bl, %ah
#   shll    $2, %eax

#   movb    %ch, %al        # put %cl and %ch to the second and first byte of x
#   movb    %cl, %ah
                            # now %eax holds the return value

    movl    %ebp, %esp      # restore %esp to its old value
    popl    %ebp            # restore the old value of %ebp to its old value
    ret                     # return to the calling function
