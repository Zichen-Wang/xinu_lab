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

#define signalreg(x, y) signalreg(x, y, 0)

void signalreg() {
    pid32   pid;

    pid = getpid();      /* Get current PID */
    kprintf("\n[%d ms]\tPID: %d\tSIGXCPU handler has been called.\n", clktimemilli, pid);
}

process test_xcpu(void)
{
    if (signalreg(SIGXCPU, &myrcv, 500) != OK) {
        kprintf("xcpu handler registration failed\n");
        return SYSERR;
    }

    while (TRUE) {
        ;
    }

    return OK;
}