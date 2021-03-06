/* revbyteorder_inline.c - revbyteorder_inline */

/*---------------------------------------------------------------------------------
 * revbytorder  -  Reverse the byte order of argument x (inline assembly version)
 *---------------------------------------------------------------------------------
 */

long revbyteorder_inline(long x)
{
    /*  use extended inline assembly format */
    asm volatile ("bswapl  %0\n\t"
                : "+r" (x));

    return x;
}
