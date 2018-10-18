/*
 * User: wang4113
 * date: 10/18/2018
 */
/*  IPCtest.c - IPCtest */

#include <xinu.h>

/*------------------------------------------------------
 *  IPCtest  -  A process for testing Test IPC in lab4
 *------------------------------------------------------
 */

process IPCtest(void)
{
    int i;

    while (TRUE) {
        send(3, currpid);
        for (i = 0; i < 3000000; i++);
        sleepms(1000);
    }
    return OK;
}