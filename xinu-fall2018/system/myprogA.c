/*  myprogA.c - myprogA */

#include <xinu.h>

process myprogA(void)
{
    pid32   pid;
    uint32  cr3;

    asm volatile ("movl %%cr3, %0"
                : "=r" (cr3));

    pid = getpid();
    kprintf("\n%d 0x%08X 0x%08X\n", pid, proctab[pid].page_directory, cr3);


    return 0;
}
