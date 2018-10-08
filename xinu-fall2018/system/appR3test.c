/*
 * User: wang4113
 * date: 09/25/2018
 */

/*  appR3test.c - appR3test */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  appR3test  -  The test application of R3 scheduling
 *------------------------------------------------------------------------
 */


void appR3test(int proctype)
{
    int i, j;
    pid32 pid = igetpid();          /* Get the pid of current process   */
    int LOOP1, LOOP2;

    /*
     * User: wang4113
     * date: 10/07/2018
     */
    if (proctype == 0) {
        LOOP1 = 10;
        LOOP2 = 10000000;
    }
    else if (proctype == 1) {
        LOOP1 = 150;
        LOOP2 = 600000;
    }

    for (i = 0; i < LOOP1; i++) { // Affects overall run-time of app.
        for (j = 0; j < LOOP2; j++) {
            // LOOP2 dictates CPU cycles consumed by the inner loop.
        }
        if (proctype == 1) // I/O-bound process: IOSLEEP determines blocking to computation ratio.
            sleepms(IOSLEEP);
    }
    // Print PID, proctype, clktimemilli, gross CPU usage, and average time before terminating.

    kprintf("%d\t%d\t%d\t%d\t%f\n",
            pid, proctype, clktimemilli, proctab[pid].pgrosscpu + currproctime,
            1.0 * proctab[pid].pwaittime / proctab[pid].pwaitcount);

}
