/*  myfuncA.c - myfuncA */

#include <xinu.h>

char myfuncA(int x)
{
    //char *esp;      /* Used in 5.3  */
    //int content;    /* Used in 5.3  */
    pid32 pid;

    /* 5.3 Get the address and content of the top of the run-time stack (stack pointer) */
    /*
    asm volatile ("movl %%esp, %0\n\t"
                  "movl (%%esp), %1\n\t"
                : "=r" (esp), "=r" (content));
    */

    pid = getpid();
    /* 5.3 Print the address of the top of run-time stack   */
    /*
    kprintf("Process Name: %s\n", (uint32)proctab[pid].prname);
    kprintf("After myfuncA() is called, the address of the top of the run-time stack is [0x%08X].\n",
            (uint32)esp);
    kprintf("Its content is %d.\n", content);
    kprintf("\n\n");
    */

    /* 5.4 Print stack base, stack size, stack limit, stack pointer, PID, and parent PID.   */
    kprintf("Process Name: %s\n", (uint32)proctab[pid].prname);
    kprintf("stack base: [0x%08X]\n", (uint32)proctab[pid].prstkbase);
    kprintf("stack size: %d bytes\n", (uint32)(proctab[pid].prstkbase - proctab[pid].prstkptr + 4));
    kprintf("stack limit: %d bytes\n", proctab[pid].prstklen);
    kprintf("stack pointer: [0x%08X]\n", (uint32)proctab[pid].prstkptr);
    kprintf("PID: %d\n", pid);
    kprintf("PPID: %d\n", getppid());
    kprintf("\n\n");

    return (char)('a' + x % 26);
}

