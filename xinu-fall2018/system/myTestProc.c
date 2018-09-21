/*
 * User: wang4113
 * date: 09/19/2018
 */

#include <xinu.h>

/*  myTestProc.c - myTestProc */

process myTestProc(void)
{
    pid32   pid;          /* Used in Lab2 4   */
    pid = igetpid();      /* Lab2 4: Get current process id by igetpid()  */

    kprintf("\n");
    kprintf("Process ID: %d\n", pid);
    kprintf("Process Name: %s\n", proctab[pid].prname);
    kprintf("\n");

    return OK;
}
