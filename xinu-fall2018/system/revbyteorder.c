/* revbyteorder.c - revbyteorder */

/*---------------------------------------------------------------------------------
 * revbytorder  -  Reverse the byte order of argument x (C function version)
 *---------------------------------------------------------------------------------
 */

long revbyteorder(long x)
{
    long a, b, c, d;    /* save four bytes of x in a, b, c and d        */
    a = x & 255;
    b = x >> 8 & 255;
    c = x >> 16 & 255;
    d = x >> 24 & 255;

    /* reverse the order of a, b, c, d and concatenate them into new x  */
    return a << 24 | b << 16 | c << 8 | d;
}
