/*
 * User: wang4113
 * date: 10/18/2018
 */
/*  test_sender.c - test_sender */

#include <xinu.h>

/*------------------------------------------------------
 *  test_sender  -  A process for testing Test IPC in lab4
 *------------------------------------------------------
 */

process sender(void)
{
    pid32   sender_pid, receiver_pid;
    umsg32	msg;
    int     i;

    sender_pid = getpid();

    while (TRUE) {

        msg = clktimemilli * sender_pid % 10000;
        receiver_pid = 3;

        if (send(3, msg) == OK) {
            /* Output the PID along with a time stamp	*/
            kprintf("\n[%d ms]\tProcess %d is sending a message \"%d\" to Process %d asynchronously.\n",
                    clktimemilli, sender_pid, msg, receiver_pid);
        }
        else {
            return SYSERR;
        }

        for (i = 0; i < 700000; i++);   /* Busy loop    */

        sleepms(1000);
    }
    return OK;
}
