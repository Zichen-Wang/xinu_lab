/*  myfuncA.c - myfuncA */

#include <xinu.h>

char myfuncA(int x)
{
    int *esp;
    pid32 pid;
    pid = getpid();
    kprintf("\nI am in myfuncA!\n");
    kprintf("Process Name: %s\n", (uint32)proctab[pid].prname);
    asm volatile("movl %%esp, %0 \n\t"
                :"=m" (esp));
    kprintf("After myfuncA() is called, the address of the top of the run-time stack is [0x%08X].\n",
            (uint32)esp);
    kprintf("Its content is %d.\n", *esp);
    kprintf("\n\n");

    return (char)('a' + x % 26);
}

