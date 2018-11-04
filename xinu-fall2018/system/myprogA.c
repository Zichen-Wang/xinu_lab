/*  myprogA.c - myprogA */

#include <xinu.h>

process myprogA(void)
{
    int *esp;
    pid32 pid;

    pid = getpid();
    kprintf("PID: %d\n", pid);


    return 0;
}
