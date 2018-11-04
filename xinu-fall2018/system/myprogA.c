/*  myprogA.c - myprogA */

#include <xinu.h>

process myprogA(void)
{
    pid32   pid;
    int *array;

    pid = getpid();

    array = (int *)(vgetmem(40));
    kprintf("%d->[0x%08X]\n", pid, array);
    for (int i = 0; i < 10; i++)
        array[i] = pid;

    for (int i = 0; i < 10; i++)
        kprintf("%d->%d\n", pid, array[i]);


    return 0;
}
