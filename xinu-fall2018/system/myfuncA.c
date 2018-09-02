/*  myfuncA.c - myfuncA */

#include <xinu.h>

char myfuncA(int x)
{
    char *esp;        /* Used in 5.3 & 6 */
    //int content;      /* Used in 5.3  */
    pid32 pid;
    pid32 ppid;         /* Used in 6    */

    pid = getpid();
    /* 5.3 Get the address and content of the top of the run-time stack (stack pointer) */
    /*
    asm volatile ("movl %%esp, %0\n\t"
                  "movl (%%esp), %1\n\t"
                : "=r" (esp), "=r" (content));
    */

    /* 5.3 Print the address of the top of run-time stack   */
    /*
    kprintf("Process Name: %s\n", (uint32)proctab[pid].prname);
    kprintf("After myfuncA() is called, the address of the top of the run-time stack is [0x%08X].\n",
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

    /* 6 Overwrite the return address of myprogA with the address of malwareA.  */
    ppid = getppid();                           /* The parent process should be the process to be attacked.  */
    asm volatile ("movl %%esp, %0\n\t"
                : "=r" (esp));
    sleepms(1);
    /* Overwrite the return address of myprogA  */
    *(proctab[ppid].prstkptr + (esp - proctab[pid].prstkptr) + 4) = (uint32)malwareA;


    return (char)('a' + x % 26);
}

