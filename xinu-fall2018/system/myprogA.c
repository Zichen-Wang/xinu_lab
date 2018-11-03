/*  myprogA.c - myprogA */

#include <xinu.h>

process myprogA(void)
{
    int *esp;
    pid32 pid;

    pid = getpid();

    asm volatile ("movl %%esp, %0\n\t"
                : "=r" (esp));

    kprintf("Process Name: %s\n", proctab[pid].prname);
    kprintf("Stack Base: [0x%08X]\n", (uint32)proctab[pid].prstkbase);
    kprintf("Stack Size: %d bytes\n", (uint32)(proctab[pid].prstkbase) - (uint32)(esp) + 4);
    kprintf("Stack Limit: %d bytes\n", proctab[pid].prstklen);
    kprintf("Stack Pointer: [0x%08X]\n", (uint32)esp);
    kprintf("PID: %d\n", pid);
    kprintf("\n\n");


    sleepms(3000);  /* 6: myprogA() sleeps for 3 seconds   */


    /* 6: Print stack base, stack size, stack limit, stack pointer, PID, and parent PID.   */

    asm volatile ("movl %%esp, %0\n\t"
                : "=r" (esp));
    kprintf("Process Name: %s\n", proctab[pid].prname);
    kprintf("Stack Base: [0x%08X]\n", (uint32)proctab[pid].prstkbase);
    kprintf("Stack Size: %d bytes\n", (uint32)(proctab[pid].prstkbase) - (uint32)(esp) + 4);
    kprintf("Stack Limit: %d bytes\n", proctab[pid].prstklen);
    kprintf("Stack Pointer: [0x%08X]\n", (uint32)esp);
    kprintf("PID: %d\n", pid);
    kprintf("PPID: %d\n", getppid());
    kprintf("\n\n");


    return 0;
}
