/*  test_vm.c - test_vm */

#include <xinu.h>

process test_vm(char c)
{
    pid32   pid;
    char *array;
    uint32  i;

    pid = getpid();

    array = vgetmem(40000);

    for (i = 0; i < 40000; i++) {
        if (i % 4096 == 0)
            kprintf("%d %d\n", i, array[i]);
        array[i] = c;
        if (i % 4096 == 0)
            kprintf("%d %d\n", i, array[i]);
    }

    kprintf("Process %d: array base address is [[0x%08X]]\n", pid, array);
    for (i = 0; i < 40000; i += 4096)
        kprintf("PID %d page %d: %d\n", pid, i / 4096, array[i]);
    kprintf("\n");

    vfreemem(array, 40000);

    return 0;
}
