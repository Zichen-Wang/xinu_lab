/*  myfuncA.c - myfuncA */

#include <xinu.h>

char myfuncA(int x)
{
    char *esp;
    int content;
    pid32 pid;

    /* Get the address of the top of run-time stack */
    asm volatile ("movl %%esp, %0\n\t"
                  "movl (%%esp), %1\n\t"
                : "=r" (esp), "=r" (content));

    /* 5.3 Print the address of the top of run-time stack   */
    /*
    pid = getpid();
    kprintf("Process Name: %s\n", (uint32)proctab[pid].prname);
    kprintf("After myfuncA() is called, the address of the top of the run-time stack is [0x%08X].\n",
            (uint32)esp);
    kprintf("Its content is %d.\n", content);
    kprintf("\n\n");
    */

    /* 5.4 Print stack base, stack size, stack limit, stack pointer, PID, and parent PID.   */
    kprintf("Process Name: %s\n", (uint32)proctab[pid].prname);
    kprintf("stack base: [0x%08X]\n", (uint32)proctab[pid].prstkbase);
    kprintf("stack size: %d bytes\n", (uint32)(proctab[pid].prstkbase - esp + 4));
    kprintf("stack limit: %d bytes\n", proctab[pid].prstklen);
    kprintf("stack pointer: [0x%08X]\n", (uint32)esp);
    kprintf("PID: %d\n", getpid());
    kprintf("PPID: %d\n", getppid());
    kprintf("\n\n");

    return (char)('a' + x % 26);
}

