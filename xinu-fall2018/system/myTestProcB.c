/*
 * User: wang4113
 * date: 09/25/2018
 */

/*  myTestProc.c - myTestProc */

#include <xinu.h>

/*--------------------------------------------------------
 *  myTestProc  -  The test application of waiting time
 *--------------------------------------------------------
 */

process myTestProcB(void)
{
    pid32   pid;
    pid = getpid();
    int i;



    /* A loop for test  */
    for (i = 0; i < 5000000; i++)
        ;

    //sleepms(50);

    for (i = 0; i < 5000000; i++)
        ;

    kprintf("\n");
    kprintf("Process ID: %d\n", pid);
    kprintf("Process Name: %s\n", proctab[pid].prname);
    kprintf("\n");

    kprintf("Process gross CPU usage: %d ms\n", proctab[pid].pgrosscpu + currproctime);
    kprintf("\n");

    kprintf("Process tot waiting time is %d ms\n", proctab[pid].pwaittime);
    kprintf("Process waiting count is %d\n", proctab[pid].pwaitcount);
    kprintf("Process average waiting time is %f ms\n", 1.0 * proctab[pid].pwaittime / proctab[pid].pwaitcount);
    kprintf("\n");

    return OK;
}
