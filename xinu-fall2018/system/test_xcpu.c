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

void userhandler() {
    pid32   pid;

    pid = getpid();      /* Get current PID */
    kprintf("\n[%d ms]\tPID: %d\tSIGXCPU handler has been called.\n", clktimemilli, pid);
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