/*  test_app.c - test_app */

#include <xinu.h>

process test_app(char c)
{
    pid32   pid;
    char *array;
    uint32  i;

    pid = getpid();

    array = getmem(5000);

    for (i = 0; i < 5000; i++)
        array[i] = c;

    kprintf("Process %d: array base address is [0x%0X]\n", pid, array);
    kprintf("%c\n", array[100]);

    return 0;
}
