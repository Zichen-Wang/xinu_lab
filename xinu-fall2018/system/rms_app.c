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
    uint32  period_start;       /* remember the current time given by clktimemilli     */
    uint32  grosscpu_start;     /* store gross CPU usage prgrosscpu                    */
    uint32  comp_received;      /* track CPU time received by the process in the period    */
    uint32  comp_temp;          /* remember the value of currproctime
                                 * which will otherwise be lost upon context-switch.    */
    uint32  period_number;      /* track the number of period   */
    pid32   pid = getpid();      /* get the pid of this process  */


    for (period_number = 1; period_number <= 40; period_number++) {     /* Outer loop   */

        grosscpu_start = proctab[pid].pgrosscpu;
        period_start = clktimemilli;    /* Remember the current time  */
        comp_received = 0;              /* Initialize CPU time received by the process in the period to zero    */
        comp_temp = currproctime;       /* Initialize the reminder of the value of currproctime */

        while (1) { /* Inner loop   */
            if (currproctime == 0) {  /* Context-switch has occurred  */
                comp_received += comp_temp;
                comp_temp = 0;
            }
            comp_temp = currproctime;   /* Update the running time of current process in case of preemption  */
            if (clktimemilli - period_start < y) {  /* The process is still in this period  */
                if (comp_received + currproctime >= x) {           /* Check whether current computation time has reached x  */
                    kprintf("\n%d\t%d\t%d\t%d\t%d\t%d\n",
                            pid, x, y, period_number, clktimemilli, y - (clktimemilli - period_start));
                    sleepms(y - (clktimemilli - period_start));     /* The real-time process has finished   */
                    break;                                          /* Go to the next period    */
                }
            }
            else {                                  /* This period has expired  */
                if (comp_received + currproctime < x) {           /* The real-time process has not finished in time  */
                    kprintf("%d\t%d\t%d\t%d\t%d\t%d\t[deadline violation]\n",
                            PID, x, y, period_number, clktimemilli, y - (clktimemilli - period_start));
                    return;     /* Terminate this real-time process */
                }
            }
        }
    }
}
