/*  myprogA.c - myprogA */

#include <xinu.h>

process myprogA(void)
{
    pid32   pid;
    int *array;
    uint32  i, t;

    pid = getpid();

    for (t = 0; t < 1000; t++) {
        array = (int *)vgetmem(10000);
        for (i = 0; i < 2500; i++) {
            array[i] = t * i;
        }

        kprintf("\npid %d, 1348th %d\n", pid, array[1348]);

        vfreemem((char *)(array), 10000);
    }

    return 0;
}
