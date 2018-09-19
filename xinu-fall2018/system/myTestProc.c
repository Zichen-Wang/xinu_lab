/*
 * User: wang4113
 * data: 09/19/2018
 */

/*  myTestProc.c - myTestProc */

#include <xinu.h>

process myTestProc(void)
{
    pid32   z;          /* Used in Lab2 4   */
    z = igetpid();      /* Lab2 4: Get current process id by igetpid()  */

    kprintf("Process ID: %s\n", z);
    kprintf("Process Name: %s\n", proctab[z].prname);
    kprintf("\n\n");

    return 0;
}
