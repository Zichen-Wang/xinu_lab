/*  myprogA.c - myprogA */

#include <xinu.h>

extern  char myfuncA(int);

process myprogA(void) {

    int x;
    char y;
    pid32 pid;

    x = 10;
    /* The address and content of the top of the run-time stack */
    pid = getpid();
    kprintf("\nProcess Name: %s\n", (uint32)proctab[pid].prname);
    kprintf("\nBefore myfuncA() is called, the address of the top of the run-time stack is [0x%08X].\n",
            (uint32)proctab[pid].prstkptr);
    kprintf("Its content is 0x%02X\n", (byte)*(proctab[pid].prstkptr));
    kprintf("\n\n");

    /* Call myfuncA */
    y = myfuncA(x);

    return 0;
}
