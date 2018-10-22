/*
 * User: wang4113
 * date: 10/22/2018
 */
/*  test_xalarm.c - test_xalarm */

#include <xinu.h>

/*-----------------------------------------------------------------
 *  test_xalarm  -  A process for testing SIGTIME
 *-----------------------------------------------------------------
 */

local void myrcv() {
    umsg32  msgbuf;     /* Using shared msgbuf by multiple receivers may cause race problem */
    pid32   pid;

    msgbuf = receive();  // copy message to user buffer
    pid = getpid();      /* Get current PID */
    kprintf("\n[%d ms]\tProcess %d received \"%d\".\n", clktimemilli, pid, msgbuf);
}

local int useralarm() {
    pid32   pid;

    pid = getpid();      /* Get current PID */
    kprintf("\n[SIGTIME] PID: %d\ttime: %d\n", pid, clktimemilli);
}

process test_xalarm(uint32 alarm_time)
{

    if (signalreg(SIGRECV, &myrcv) != OK) {
        kprintf("recv handler registration failed\n");
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