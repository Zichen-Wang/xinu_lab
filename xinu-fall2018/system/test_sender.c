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

    for (msg_num = 0; msg_num < 10 * receiver_num; msg_num++) {

        msg = clktimemilli * msg_num % 10000;
        receiver_pid = clktimemilli * 97 % receiver_num + receiver_min;

        if (send(receiver_pid, msg) == OK) {
            /* Output the PID along with a time stamp	*/
            kprintf("\nmsg_num: %d\t%d ms\tProcess %d sent `%d' to Process %d.\n",
                    msg_num, clktimemilli, sender_pid, msg, receiver_pid);
        }
        else {
            kprintf("\nmsg_num: %d\t%d ms\tProcess %d sent `%d' to Process %d.[ERROR OCCUR] \n",
                    msg_num, clktimemilli, sender_pid, msg, receiver_pid);
        }

        for (i = 0; i < 500000; i++);   /* Busy loop    */
        sleepms(500);

    }
    return OK;
}
