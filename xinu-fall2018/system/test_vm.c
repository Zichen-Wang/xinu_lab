/*  test_vm.c - test_vm */

#include <xinu.h>

process test_vm(char c)
{
    pid32   pid;
    char *array;
    uint32  i;

    pid = getpid();

    array = vgetmem(5000);

    for (i = 0; i < 5000; i++)
        array[i] = c;

    kprintf("Process %d: array base address is [[0x%0X]]\n", pid, array);
    kprintf("%c\n", array[100]);

    vfreemem(array, 5000);

    return 0;
}
