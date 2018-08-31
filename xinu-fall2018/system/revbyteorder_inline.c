/* revbyteorder_inline.c - revbyteorder_inline */

#include <xinu.h>

long revbyteorder_inline(long x)
{
    asm volatile(
            "bswapl  %1\n\t"
            :
            :"r"(x)
            );
    
    return x;
}