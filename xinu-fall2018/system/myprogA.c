/*  myprogA.c - myprogA */

#include <xinu.h>

process myprogA(void)
{
    int *esp;
    pid32 pid;

    pid = getpid();
    kprintf("\n%d: 0x%08X\n", pid, proctab[pid].page_directory);


    return 0;
}
