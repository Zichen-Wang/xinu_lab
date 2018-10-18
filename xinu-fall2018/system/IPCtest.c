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
    while (True) {
        send(3, currpid);
        sleepms(100);
    }
    return OK;
}
