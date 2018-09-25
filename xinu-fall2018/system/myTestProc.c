/*
 * User: wang4113
 * date: 09/19/2018
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

    /* A loop for test  */
    for (i = 0; i < 1000000; i++)
        ;

    //sleepms(200);

    kprintf("\n");
    kprintf("Process ID: %d\n", pid);
    kprintf("Process Name: %s\n", proctab[pid].prname);
    kprintf("Process Gross CPU Usage: %d ms\n", proctab[pid].pgrosscpu + currproctime);
    kprintf("The time difference between time_stamp and now is %d ms. ", clktimemilli - time_stamp);
    kprintf("It should be (Usage - 200) ms\n");
    kprintf("\n");

    return OK;
}
