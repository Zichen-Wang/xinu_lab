/*
 * User: wang4113
 * date: 09/20/2018
 */

#include "../include/xinu.h"

/*  appR3test.c - appR3test */

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

    kprintf("\n");
    kprintf("Process ID: %d\n", pid);
    kprintf("Process Type: %s\n", (proctype == 0) ? "CPU-bound" : "IO-bound");
    kprintf("Clktimemilli: %d ms\n", clktimemilli);
    kprintf("Gross CPU usage: %d ms\n", proctab[pid].pgrosscpu);
    kprintf("Average waiting time: %f ms\n", 1.0 * proctab[pid].pwaittime / proctab[pid].pwaitcount);
    kprintf("\n");

}
