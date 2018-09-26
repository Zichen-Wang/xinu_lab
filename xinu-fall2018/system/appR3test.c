/*
 * User: wang4113
 * date: 09/20/2018
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
    pid32 pid;
    for (i = 0; i < LOOP1; i++) { // Affects overall run-time of app.
        for (j = 0; j < LOOP2; j++) {
            // LOOP2 dictates CPU cycles consumed by the inner loop.
        }
        if (proctype == 1) // I/O-bound process: IOSLEEP determines blocking to computation ratio.
            sleepms(IOSLEEP);
    }
    // Print PID, proctype, clktimemilli, gross CPU usage, and average time before terminating.
    pid = igetpid();

    kprintf("\nProcess ID: %d\n", pid);
    kprintf("Process Type: %s\n", (proctype == 0) ? "CPU-bound" : "IO-bound");
    kprintf("Gross CPU usage: %d ms, average waiting time: %f ms\n\n",
            proctab[pid].pgrosscpu + currproctime, 1.0 * proctab[pid].pwaittime / proctab[pid].pwaitcount);

}
