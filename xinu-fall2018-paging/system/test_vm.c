/*  test_vm.c - test_vm */

#include <xinu.h>

process test_vm(char c)
{
    pid32   pid;
    char *array;
    uint32  i;

    pid = getpid();

    array = vgetmem(60*4096);

    for (i = 0; i < 60*4096; i++) {
        array[i] = c;
        if (i % 4096 == 0) {
           kprintf("i %d\n", i, *(char *)(i));
        }
    }

    kprintf("Process %d: array base address is [[0x%08X]]\n", pid, array);
    for (i = 0; i < 60*4096; i += 4096)
        kprintf("PID %d page %d: %d\n", pid, i / 4096, array[i]);
    kprintf("\n");

    vfreemem(array, 60*4096);

    return 0;
}
