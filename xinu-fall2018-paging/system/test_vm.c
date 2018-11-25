/*  test_vm.c - test_vm */

#include <xinu.h>

process test_vm(char c)
{
    pid32   pid;
    char *array;
    uint32  i;

    pid = getpid();

    array = vgetmem(60*4906);

    for (i = 0; i < 60*4906; i++)
        array[i] = c;

    kprintf("Process %d: array base address is [[0x%0X]]\n", pid, array);
    for (i = 0; i < 60*4906; i += 4096)
        kprintf("PID %d page %d: %c", pid, i / 4096, array[i]);
    kprintf("\n");

    vfreemem(array, 60*4906);

    return 0;
}
