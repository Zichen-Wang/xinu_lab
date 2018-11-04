/*  myprogA.c - myprogA */

#include <xinu.h>

process myprogA(void)
{
    pid32   pid;
    int *array;
    uint32  i;

    pid = getpid();

    array = vgetmem(400);
    for (i = 0; i < 100; i++)
        array[i] = i * pid;

    kprintf("pid %d, 18th %d\n", pid, array[18]);

    return 0;
}
