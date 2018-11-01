/*
 * User: wang4113
 * date: 10/08/2018
 */

/*  rms_app.c - rms_app */

#include <xinu.h>

/*------------------------------------------
 *  rms_app  -  A periodic real-time app
 *------------------------------------------
 */

void rms_app(uint32 x, uint32 y)    /* x specifies computation time requirement and y specifies period  */
{
    uint32  period_start;       /* remember the current time given by clktimemilli      */
    uint32  grosscpu_start;     /* track the gross CPU usage at the start of period     */
    uint32  period_number;      /* track the number of period   */
    uint32  comp_received;      /* track CPU time received by the process in the period    */

    pid32   pid = getpid();      /* get the pid of this process  */

    uint32  sleeptime;          /* time to sleep    */

    for (period_number = 1; period_number <= 100; period_number++) {     /* Outer loop   */

        period_start = clktimemilli;    /* Remember the current time    */
        grosscpu_start = proctab[currpid].pgrosscpu;    /* Remember the gross CPU usage at the start of period */
        comp_received = 0;          /* Initialize computation time to zero  */

        while (1) { /* Inner loop   */
            /* Update the computation time  */
            comp_received = proctab[currpid].pgrosscpu + currproctime - grosscpu_start;

            if (clktimemilli - period_start < y) {  /* The process is still in this period  */
                if (comp_received >= x) {           /* Check whether current computation time has reached x  */
                    sleeptime = y - (clktimemilli - period_start);  /* Calculate the sleep time */

                    kprintf("%d\t%d\t%d\t%d\t%d\t%d\n",
                            pid, x, y, period_number, clktimemilli, sleeptime);

                    sleepms(sleeptime);     /* The real-time process has finished   */
                    break;                                          /* Go to the next period    */
                }
            }
            else {                                  /* This period has expired  */
                if (comp_received < x) {           /* The real-time process has not finished in time  */

                    kprintf("%d\t%d\t%d\t%d\t%d\t%d\t[deadline violation]\n",
                            pid, x, y, period_number, clktimemilli, y - (clktimemilli - period_start));

                    return;     /* Terminate this real-time process */
                }
            }
        }
    }

}
