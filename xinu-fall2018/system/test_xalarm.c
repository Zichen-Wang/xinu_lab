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

int useralarm() {
    pid32   pid;

    pid = getpid();      /* Get current PID */
    kprintf("\n[SIGTIME] PID: %d\ttime: %d\n", pid, clktimemilli);
}

process test_xalarm(uint32 alarm_time)
{
    uint32 i;
    if (signalreg(SIGTIME, &useralarm, 0) != OK) {
        kprintf("alarm handler registration failed\n");
        return SYSERR;
    }
    kprintf("%d %d\n", alarm_time, clktimemilli);
    if (xalarm(alarm_time) != OK) {
        kprintf("alarm set failed\n");
    }

    for (i = 0; i < 10000000; i++);


    while (TRUE) {
        ;
    }

    return OK;
}