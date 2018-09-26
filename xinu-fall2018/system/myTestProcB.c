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

    sleepms(10);    /* Sleep for 10 ms to test tot waiting time */

    /* A loop for test  */
    for (i = 0; i < 5000000; i++)
        ;

    kprintf("\n");
    kprintf("Process ID: %d\n", pid);
    kprintf("Process Name: %s\n", proctab[pid].prname);

    kprintf("Process tot waiting time and waiting count:\n");
    kprintf("%d ms | %d\n", proctab[pid].pwaittime, proctab[pid].pwaitcount);

    kprintf("Process gross CPU usage:\n");
    kprintf("%d ms\n\n", proctab[pid].pgrosscpu + currproctime);

    return OK;
}
