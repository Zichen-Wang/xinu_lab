/*  myfuncA.c - myfuncA */

#include <xinu.h>

char myfuncA(int x)
{
    int *esp;
    int content;
    pid32 pid = -1;

    /* Get the address of the top of run-time stack */
    asm volatile ("movl %%esp, %0\n\t"
                  "movl (%%esp), %1\n\t"
                : "=r" (esp), "=r" (content));

    /* Print the process name and that address      */
    pid = getpid();
    kprintf("Process Name: %s\n", (uint32)proctab[pid].prname);
    kprintf("After myfuncA() is called, the address of the top of the run-time stack is [0x%08X].\n",
            (uint32)esp);
    kprintf("Its content is %d.\n", content);
    kprintf("\n\n");

    return (char)('a' + x % 26);
}

