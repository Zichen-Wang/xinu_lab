/*  myprogA.c - myprogA */

#include <xinu.h>

process myprogA(void) {

    int x;
    char y;
    int *esp;
    int content;
    pid32 pid = -1;

    x = 10;
    /* Get the address and content of the top of the run-time stack */

    asm volatile ("movl %%esp, %0\n\t"
                  "movl (%%esp), %1\n\t"
                : "=r" (esp), "=r" (content));
    /* Call myfuncA */
    y = myfuncA(x);

    /* Print the process name and that address      */
    pid = getpid();
    kprintf("Process Name: %s\n", (uint32)proctab[pid].prname);
    kprintf("Before myfuncA() is called, the address of the top of the run-time stack is [0x%08X].\n",
            (uint32)esp);
    kprintf("Its content is %d.\n", content);
    kprintf("\n\n");

    /* To suppress the warning    */
    kprintf("\nThe result of myfunA function call is '%c'\n", y);

    return 0;
}
