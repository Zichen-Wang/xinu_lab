/* xuptime.c -  xuptime */

#include <xinu.h>

/*------------------------------------------------------------------------
 * xuptime  -  returns the uptime in minutes and seconds since XINU was bootstrapped on a backend.
 *------------------------------------------------------------------------
 */

syscall xuptime(xminsec *p) /* a pointer to a xminsec struct */
{

    intmask mask;   /* Saved interrupt mask		*/
    mask = disable();   /* Disable interrupts */

    p -> upmin = clktime / 60;
    p -> upsec = clktime % 60;

    restore(mask);

    return OK;
}