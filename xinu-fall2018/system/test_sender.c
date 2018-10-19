/*
 * User: wang4113
 * date: 10/18/2018
 */
/*  test_sender.c - test_sender */

#include <xinu.h>

/*-----------------------------------------------------------------
 *  test_sender  -  A process for testing asynchronous IPC in lab4
 *-----------------------------------------------------------------
 */

process test_sender(pid32 receiver_min, uint32 receiver_num)
{
    pid32   sender_pid, receiver_pid;
    umsg32	msg;
    int     msg_num, i;

    sender_pid = getpid();

    for (msg_num = 1; msg_num <= 20; msg_num++) {

        msg = clktimemilli * msg_num % 10000;
        receiver_pid = clktimemilli * msg_num % receiver_num + receiver_min;

        if (send(receiver_pid, msg) == OK) {
            /* Output the PID along with a time stamp	*/
            kprintf("\n[%d ms]\tProcess %d is sending a message \"%d\" to Process %d. [msg_num: %d]\n",
                    clktimemilli, sender_pid, msg, receiver_pid, msg_num);
        }
        else {
            kprintf("\n[%d ms]\tProcess %d is sending a message \"%d\" to Process %d. [msg_num: %d] [ERROR OCCUR] \n",
                    clktimemilli, sender_pid, msg, receiver_pid, msg_num);
            return SYSERR;
        }

        for (i = 0; i < 700000; i++);   /* Busy loop    */

        sleepms(1000);
    }
    return OK;
}