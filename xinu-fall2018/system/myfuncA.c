/*  myfuncA.c - myfuncA */

#include <xinu.h>

char myfuncA(int x)
{
    char *esp;        /* Stack pointer; used in 5.3 & 5.4 & 6 */
    //uint32 i;
    pid32 pid;
    //pid32 ppid;         /* Used in 6    */

    pid = getpid();
    /* 5.3: Get the address and content of the top of the run-time stack (stack pointer) */

    asm volatile ("movl %%esp, %0\n\t"
                : "=r" (esp));


    /* 5.3: Print the address of the top of run-time stack   */

    kprintf("Process Name: %s\n", (uint32)proctab[pid].prname);
    kprintf("After myfuncA() is called, the address of the top of the run-time stack is [0x%08X].\n",
            (uint32)esp);
    kprintf("Its content is %d.\n", *esp);
    kprintf("\n\n");


    /* 5.4 & 6 Print stack base, stack size, stack limit, stack pointer, PID, and parent PID.   */
    /*
    asm volatile ("movl %%esp, %0\n\t"
                : "=r" (esp));
    kprintf("Process Name: %s\n", (uint32)proctab[pid].prname);
    kprintf("Stack Base: [0x%08X]\n", (uint32)proctab[pid].prstkbase);
    kprintf("Stack Size: %d bytes\n", (uint32)(proctab[pid].prstkbase - esp + 4));
    kprintf("Stack Limit: %d bytes\n", proctab[pid].prstklen);
    kprintf("Stack Pointer: [0x%08X]\n", (uint32)esp);
    kprintf("PID: %d\n", pid);
    kprintf("PPID: %d\n", getppid());
    kprintf("\n\n");
    */

    /* 6: Overwrite the return address of myprogA with the address of malwareA.  */
    //ppid = getppid();                           /* The parent process should be the process to be attacked.  */
    /* Overwrite the return address of myprogA  */
    
    /* Print the stack of myprogA to find the return address    */
    /*
    for (i = (uint32)proctab[ppid].prstkbase; i >= (uint32)proctab[ppid].prstkptr; i -= 4) {
        kprintf("*** [0x%08X] 0x%08X ***\n", i, *(int *)i);
    }
    */

    /* The return address of 'sleepms' should just follow the argument '3000'(0xBB4)  */
    
    /* In this case, the return address is 'prstkbase - 52' */
    //*(int*)(proctab[ppid].prstkbase - 52) = (uint32)malwareA;

    /* Save the INITRET address preceding the return address in order that myprogA can exit normally   */
    //*(int*)(proctab[ppid].prstkbase - 48) = *(int*)(proctab[ppid].prstkbase - 4);



    /* Bonus: Overwrite the return address of myprogA with the address of malwareB without any disruption on myprogA    */
    //*(int*)(proctab[ppid].prstkbase - 48) = *(int*)(proctab[ppid].prstkbase - 52);
    //*(int*)(proctab[ppid].prstkbase - 52) = (uint32)malwareB;

    return (char)('a' + x % 26);
}

