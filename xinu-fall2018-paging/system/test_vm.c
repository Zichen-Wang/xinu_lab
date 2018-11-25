/*  test_vm.c - test_vm */

#include <xinu.h>

process test_vm(char c)
{
    pid32   pid;
    char *array;
    uint32  i;

    pid = getpid();

    array = vgetmem(10000);

    for (i = 4096; i < 5000; i++) {
        array[i] = 10;
        kprintf("%d %d\n", i, array[i]);
    }

    kprintf("Process %d: array base address is [[0x%08X]]\n", pid, array);
    for (i = 0; i < 10000; i += 4096)
        kprintf("PID %d page %d: %d\n", pid, i / 4096, array[i]);
    kprintf("\n");

    vfreemem(array, 10000);

    return 0;
}
