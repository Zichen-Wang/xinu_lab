/* clkinit.c - clkinit (x86) */

#include <xinu.h>

uint32	clktime;		/* Seconds since boot			*/
uint32	ctr1000 = 0;		/* Milliseconds since boot		*/
qid16	sleepq;			/* Queue of sleeping processes		*/
uint32	preempt;		/* Preemption counter			*/

/*
 * User: wang4113
 * data: 09/19/2018
 */
uint32	clktimemilli;	/* Lab2 3.1: Milliseconds since boot	*/
uint32	currproctime;	/* Lab2 3.2: CPU usage when a process becomes current	*/

/*------------------------------------------------------------------------
 * clkinit  -  Initialize the clock and sleep queue at startup (x86)
 *------------------------------------------------------------------------
 */
void	clkinit(void)
{
	uint16	intv;		/* Clock rate in KHz			*/

	/* Allocate a queue to hold the delta list of sleeping processes*/

	sleepq = newqueue();

	/* Initialize the preemption count */

	preempt = QUANTUM;

	/* Initialize the time since boot to zero */

	clktime = 0;

	/*
	 * User: wang4113
	 * data: 09/19/2018
	 */
	/* Lab2 3.1: Initialize the time since boot to zero */
	clktimemilli = 0;

	/* Lab2 3.2: Initialize the CPU usage since boot to zero */
	currproctime = 0;

	/* Set interrupt vector for the clock to invoke clkdisp */

	set_evec(IRQBASE, (uint32)clkdisp);

	/* Set the hardware clock: timer 0, 16-bit counter, rate */
	/*   generator mode, and counter runs in binary		 */

	outb(CLKCNTL, 0x34);

	/* Set the clock rate to 1.190 Mhz; this is 1 ms interrupt rate */

	intv = 1193;	/* Using 1193 instead of 1190 to fix clock skew	*/

	/* Must write LSB first, then MSB */

	outb(CLOCK0, (char) (0xff & intv) );
	outb(CLOCK0, (char) (0xff & (intv>>8)));

	return;
}
