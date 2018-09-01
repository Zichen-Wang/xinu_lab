/* revbyteorder_inline.c - revbyteorder_inline */

/*---------------------------------------------------------------------------------
 * revbytorder  -  reverses the byte order of argument x (in-line assembly version)
 *---------------------------------------------------------------------------------
 */

long revbyteorder_inline(long x)
{
    asm volatile(
            "bswapl  %0\n\t"
            :"+r"(x)
            :
            :
            );

    return x;
}
