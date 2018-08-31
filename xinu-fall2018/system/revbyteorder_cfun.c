/* revbyteorder_cfun.c - revbyteorder_cfun */

#include <xinu.h>
/*---------------------------------------------------------------------------------
 * revbytorder_cfun  -  reverses the byte order of argument x (C function version)
 *---------------------------------------------------------------------------------
 */

long revbyteorder_cfun(long x)
{
    long a, b, c, d;
    a = x & 255;
    b = x >> 8 & 255;
    c = x >> 16 & 255;
    d = x >> 24 & 255;

    return a << 24 | b << 16 | c << 8 | d;
}