/*  myprogA.c - myprogA */

#include <xinu.h>

/*-------------------------------
 * myfuncA  -  Called by myprogA
 *-------------------------------
 */

char myfuncA(int x)
{
    pid32 pid;
    pid = getpid();
    kprintf("\nProcess Name: %s\n", (uint32)proctab[pid].prname);
    kprintf("After myfuncA() is called, the address of the top of the run-time stack is [0x%08X].\n",
            (uint32)proctab[pid].prstkptr);
    kprintf("Its content is 0x%02X.\n", (byte)*(proctab[pid].prstkptr));
    kprintf("\n\n");

    return (char)('a' + x % 26);
}

process myprogA(void) {

    int x;
    char y;
    pid32 pid;

    x = 10;
    /* The address and content of the top of the run-time stack */
    pid = getpid();
    kprintf("\nProcess Name: %s\n", (uint32)proctab[pid].prname);
    kprintf("Before myfuncA() is called, the address of the top of the run-time stack is [0x%08X].\n",
            (uint32)proctab[pid].prstkptr);
    kprintf("Its content is 0x%02X.\n", (byte)*(proctab[pid].prstkptr));
    kprintf("\n\n");

    /* Call myfuncA */
    y = myfuncA(x);

    /* To suppress the warning    */
    kprintf("\nThe result of myfunA function call is %c\n", y);

    return 0;
}
