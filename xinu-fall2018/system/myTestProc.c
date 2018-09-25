/*
 * User: wang4113
 * date: 09/25/2018
 */

/*  myTestProc.c - myTestProc */

#include <xinu.h>

/*-----------------------------------------------------------------------------------------
 *  myTestProc  -  The test application of gross CPU usage, waiting time and igetpid()
 *-----------------------------------------------------------------------------------------
 */

process myTestProc(void)
{
    uint32 time_stamp = clktimemilli; /* Used in Lab2 3.2 */
    pid32   pid;          /* Used in Lab2 4   */
    pid = igetpid();      /* Lab2 4: Get current process id by igetpid()  */
    int i;

    kprintf("\n");
    kprintf("Process ID: %d\n", pid);
    kprintf("Process Name: %s\n", proctab[pid].prname);
    kprintf("\n");

    /* A loop for test  */
    for (i = 0; i < 10000000; i++)
        ;

    kprintf("Process gross CPU usage before sleeping 200 ms: %d (%d - %d) ms\n",
            proctab[pid].pgrosscpu + currproctime, clktimemilli, time_stamp);

    sleepms(200);

    kprintf("Process average waiting time after sleeping 200 ms: %lf ms\n",
            1.0 * proctab[pid].pwaittime / proctab[pid].pwaitcount);

    /* A loop for test  */
    for (i = 0; i < 10000000; i++)
        ;

    kprintf("Process gross CPU usage after sleeping 200 ms: %d (%d - %d - 200) ms\n",
            proctab[pid].pgrosscpu + currproctime, clktimemilli, time_stamp);
    kprintf("\n");

    return OK;
}
