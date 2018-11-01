/*
 * User: wang4113
 * date: 09/25/2018
 */

/*  myTestProcC.c - myTestProcC */

#include <xinu.h>

/*-----------------------------------------------------
 *  myTestProcC  -  The test application of igetpid()
 *-----------------------------------------------------
 */

process myTestProcC(void)
{
    pid32   pid;
    pid = igetpid();            /* Used in Lab2 4 */

    kprintf("\n");
    kprintf("Process ID: %d\n", pid);
    kprintf("Process Name: %s\n", proctab[pid].prname);
    kprintf("\n");

    return OK;
}
