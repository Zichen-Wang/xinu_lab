/*
 * User: wang4113
 * date: 09/25/2018
 */

/*  myTestProcA.c - myTestProcA */

#include <xinu.h>

/*----------------------------------------------------------
 *  myTestProcA  -  The test application of gross CPU usage
 *----------------------------------------------------------
 */

process myTestProcA(void)
{
    uint32 time_stamp = clktimemilli; /* Used in Lab2 3.2 */
    pid32   pid;
    pid = igetpid();            /* Used in Lab2 3.2 */
    int i;

    kprintf("\n");
    kprintf("Process ID: %d\n", pid);
    kprintf("Process Name: %s\n", proctab[pid].prname);
    kprintf("\n");

    /* A loop for test  */
    for (i = 0; i < 10000000; i++)
        ;

    kprintf("Process gross CPU usage before sleeping 200 ms:\n");
    kprintf("%d (%d - %d) ms\n",
            proctab[pid].pgrosscpu + currproctime, clktimemilli, time_stamp);

    sleepms(200);   /* Sleep for 200 ms to relinquish the CPU  */

    /* A loop for test  */
    for (i = 0; i < 10000000; i++)
        ;

    kprintf("Process gross CPU usage after sleeping 200 ms:\n");
    kprintf("%d (%d - %d - 200) ms\n\n",
            proctab[pid].pgrosscpu + currproctime, clktimemilli, time_stamp);

    return OK;
}
