/*  myprogA.c - myprogA */

#include <xinu.h>

process myprogA(void) {

    int x;
    char y;
    int *esp;
    pid32 pid;

    x = 10;
    /* The address and content of the top of the run-time stack */
    pid = getpid();
    kprintf("\nI am in myprogA!\n");
    kprintf("Process Name: %s\n", (uint32)proctab[pid].prname);
    asm volatile("movl %%esp, %0 \n\t"
                 :"=m" (esp));
    kprintf("Before myfuncA() is called, the address of the top of the run-time stack is [0x%08X].\n",
            (uint32)esp);
    kprintf("Its content is %d.\n", *esp);
    kprintf("\n\n");

    /* Call myfuncA */
    y = myfuncA(x);

    /* To suppress the warning    */
    kprintf("\nThe result of myfunA function call is %c\n", y);

    return 0;
}
