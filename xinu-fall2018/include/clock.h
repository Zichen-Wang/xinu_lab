/* clock.h */

/* Intel 8254-2 clock chip constants */

#define	CLOCKBASE	0x40		/* I/O base port of clock chip	*/
#define	CLOCK0		CLOCKBASE
#define	CLKCNTL		(CLOCKBASE+3)	/* chip CSW I/O port		*/


#define CLKTICKS_PER_SEC  1000	/* clock timer resolution		*/

extern	uint32	clktime;	/* current time in secs since boot	*/

extern	qid16	sleepq;		/* queue for sleeping processes		*/
extern	uint32	preempt;	/* preemption counter			*/


/* time struct which keeps track of uptime in minutes and seconds   */

typedef struct xminsec {

    int upmin;
    int upsec;

} xminsec_t;

/*
 * User: wang4113
 * data: 09/19/2018
 */
extern  uint32  clktimemilli;   /* Lab2 3.1: current time in milli-secs since boot  */
extern  uint32  currproctime;   /* Lab2 3.2: current CPU usage in milli-secs        */
