/*  test_vm.c - test_vm */

#include <xinu.h>

process test_vm(void)
{
    pid32   pid;
    int *array;
    uint32  i, t;

    pid = getpid();

    for (t = 0; t < 2; t++) {
        array = (int *)vgetmem(10000);
        for (i = 0; i < 2500; i++) {
            array[i] = (t + 1) * pid * i;
        }

        kprintf("pid %d, 1348th %d\n", pid, array[1348]);

        vfreemem((char *)(array), 10000);
    }

    return 0;
}
