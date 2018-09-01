/*  myfuncA.c - myfuncA */

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
    kprintf("\nAfter myprogA() is created, the address of the top of the run-time stack is [0x%08X].\n",
            (uint32)proctab[pid].prstkptr);
    kprintf("Its content is 0x%02X\n", (byte)*(proctab[pid].prstkptr));
    kprintf("\n\n");

    return (char)('a' + x % 26);
}
