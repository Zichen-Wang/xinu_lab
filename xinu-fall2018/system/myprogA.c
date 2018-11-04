/*  myprogA.c - myprogA */

#include <xinu.h>

process myprogA(void)
{
    pid32   pid;
    int *array;
    uint32  i;

    pid = getpid();

    array = (int *)vgetmem(10000);
    for (i = 0; i < 2500; i++)
        array[i] = i * pid;

    kprintf("pid %d, 1348th %d\n", pid, array[1348]);

    vfreemem((char *)(array), 10000);

    return 0;
}
