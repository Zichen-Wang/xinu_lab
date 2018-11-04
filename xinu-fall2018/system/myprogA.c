/*  myprogA.c - myprogA */

#include <xinu.h>

process myprogA(void)
{
    pid32   pid;
    int *array;

    pid = getpid();

    kprintf("pid: %d\n", pid);

    return 0;
}
