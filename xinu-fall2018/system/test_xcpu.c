/*
 * User: wang4113
 * date: 10/21/2018
 */
/*  test_xcpu.c - test_xcpu */

#include <xinu.h>

/*-----------------------------------------------------------------
 *  test_xcpu  -  A process for testing SIGXCPU
 *-----------------------------------------------------------------
 */

int userhandler() {
    pid32   pid;

    pid = getpid();      /* Get current PID */
    kprintf("\n[SIGXCPU] PID: %d\ttime: %d\tusage: %d\n", pid, clktimemilli, proctab[pid].pgrosscpu + currproctime);
}

process test_xcpu(uint32 cpu_time)
{
    if (signalreg(SIGXCPU, &userhandler, cpu_time) != OK) {
        kprintf("xcpu handler registration failed\n");
        return SYSERR;
    }

    while (TRUE) {
        ;
    }

    return OK;
}