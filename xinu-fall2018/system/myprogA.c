/*  myprogA.c - myprogA */

#include <xinu.h>

process myprogA(void)
{
    //char y;           /* Used in 5.3  */
    //char *esp;        /* Used in 5.3  */
    //int content;      /* Used in 5.3  */
    pid32 pid;

    pid = getpid();
    /* 5.3 Get the address and content of the top of the run-time stack (stack pointer) */
    /*
    asm volatile ("movl %%esp, %0\n\t"
                  "movl (%%esp), %1\n\t"
                : "=r" (esp), "=r" (content));
    */

    /* 5.3 Print the address of the top of run-time stack   */
    /* Call myfuncA */
    /*
    y = myfuncA(10);

    kprintf("Process Name: %s\n", (uint32)proctab[pid].prname);
    kprintf("Before myfuncA() is called, the address of the top of the run-time stack is [0x%08X].\n",
            (uint32)esp);
    kprintf("Its content is %d.\n", content);
    kprintf("\n\n");
    */

    /* 5.4 & 6 Print stack base, stack size, stack limit, stack pointer, PID, and parent PID.   */

    kprintf("Process Name: %s\n", (uint32)proctab[pid].prname);
    kprintf("Stack Base: [0x%08X]\n", (uint32)proctab[pid].prstkbase);
    kprintf("Stack Size: %d bytes\n", (uint32)(proctab[pid].prstkbase - proctab[pid].prstkptr + 4));
    kprintf("Stack Limit: %d bytes\n", proctab[pid].prstklen);
    kprintf("Stack Pointer: [0x%08X]\n", (uint32)proctab[pid].prstkptr);
    kprintf("PID: %d\n", pid);
    kprintf("PPID: %d\n", getppid());
    kprintf("\n\n");

    /* To suppress the warning in 5.3    */
    // kprintf("\nThe result of myfunA function call is '%c'\n\n\n", y);

    /* 6 Spawn a process running myfuncA() with priority 20.    */
    resume(create(myfuncA, 1024, 20, "myfuncA (6)", 0, NULL));
    sleepms(3000);  /* myprogA() sleeps for 3 seconds   */

    /* 6 Print stack base, stack size, stack limit, stack pointer, PID, and parent PID.   */
    kprintf("Process Name: %s\n", (uint32)proctab[pid].prname);
    kprintf("Stack Base: [0x%08X]\n", (uint32)proctab[pid].prstkbase);
    kprintf("Stack Size: %d bytes\n", (uint32)(proctab[pid].prstkbase - proctab[pid].prstkptr + 4));
    kprintf("Stack Limit: %d bytes\n", proctab[pid].prstklen);
    kprintf("Stack Pointer: [0x%08X]\n", (uint32)proctab[pid].prstkptr);
    kprintf("PID: %d\n", pid);
    kprintf("PPID: %d\n", getppid());
    kprintf("\n\n");

    return 0;
}
