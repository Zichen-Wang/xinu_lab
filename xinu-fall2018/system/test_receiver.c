/*
 * User: wang4113
 * date: 10/18/2018
 */
/*  test_receiver.c - test_receiver */

#include <xinu.h>

/*-----------------------------------------------------------------
 *  test_receiver  -  A process for testing asynchronous IPC in lab4
 *-----------------------------------------------------------------
 */

void myrcv() {
    extern umsg32 msgbuf;
    pid32   pid;

    msgbuf = receive();  // copy message to user buffer
    pid = getpid();      /* Get current PID */
    kprintf("\n[%d ms]\tProcess %d received \"%d\".\n", clktimemilli, pid, msgbuf);
}

process test_receiver(void)
{
    if (reghandler(&myrcv) != OK) {
        kprintf("recv handler registration failed\n");
        return SYSERR;
    }

    while (TRUE) {
        ;
    }

    return OK;
}