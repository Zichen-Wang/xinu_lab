/*  myprogA.c - myprogA */

#include <xinu.h>

process myprogA(void)
{
    pid32   pid;
    uint32  cr3;
    int *array;

    pid = getpid();

    array = getmem(40);
    for (int i = 0; i < 10; i++)
        array[i] = pid;

    for (int i = 0; i < 10; i++)
        kprintf("\npid %d: %d\n", pid, array[i]);


    return 0;
}
