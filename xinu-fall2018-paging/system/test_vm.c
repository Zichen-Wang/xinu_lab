/*  test_vm.c - test_vm */

#include <xinu.h>

process test_vm(char c)
{
    pid32   pid;
    int *array;
    uint32  i;

    pid = getpid();

    array = vgetmem(40000);

    for (i = 0; i < 10000; i++) {
        array[i] = c;
    }

    kprintf("Process %d: array base address is [[0x%08X]]\n", pid, array);
    for (i = 0; i < 10000; i += 1024)
        kprintf("PID %d page %d: %d\n", pid, i / 1024, array[i]);
    kprintf("\n");

    vfreemem(array, 40000);

    return 0;
}
