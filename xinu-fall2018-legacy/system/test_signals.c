/*
 * User: wang4113
 * date: 10/21/2018
 */
/*  test_signals.c - test_signals */

#include <xinu.h>

/*-----------------------------------------------------------------
 *  test_signals  -  A process for testing SIGXCPU
 *-----------------------------------------------------------------
 */

local void myrcv() {
    umsg32  msgbuf;     /* Using shared msgbuf by multiple receivers may cause race problem */
    pid32   pid;

    msgbuf = receive();  // copy message to user buffer
    pid = getpid();      /* Get current PID */
    kprintf("\n[%d ms]\tProcess %d received \"%d\".\n", clktimemilli, pid, msgbuf);
}

local int userhandler() {
    pid32   pid;

    pid = getpid();      /* Get current PID */
    kprintf("\n[SIGXCPU] PID: %d\ttime: %d\tusage: %d\n", pid, clktimemilli, proctab[pid].pgrosscpu + currproctime);
}

local int useralarm() {
    pid32   pid;

    pid = getpid();      /* Get current PID */
    kprintf("\n[SIGTIME] PID: %d\ttime: %d\n", pid, clktimemilli);
}

process test_signals(uint32 cpu_time, uint32 alarm_time)
{
    if (signalreg(SIGRECV, &myrcv, 0) != OK) {
        kprintf("recv handler registration failed\n");
        return SYSERR;
    }

    if (signalreg(SIGXCPU, &userhandler, cpu_time) != OK) {
        kprintf("xcpu handler registration failed\n");
        return SYSERR;
    }

    if (signalreg(SIGTIME, &useralarm, 0) != OK) {
        kprintf("alarm handler registration failed\n");
        return SYSERR;
    }

    if (xalarm(alarm_time) != OK) {
        kprintf("alarm set failed\n");
    }

    while (TRUE) {
        ;
    }

    return OK;
}