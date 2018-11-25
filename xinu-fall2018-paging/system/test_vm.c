/*  test_vm.c - test_vm */

#include <xinu.h>

process test_vm(char c)
{
    pid32   pid;
    int *array;
    uint32  i;

    kprintf("before [2024 * 4096 + 8] = %d\n", *(int *)(2024 * 4096 + 8));
    kprintf("before pid: %d, frame num %d\n", currpid, proctab[currpid].page_directory[4].pd_base);
    *(int *)(2024 * 4096 + 8) = 9;
    //kprintf("before %d\n", *(int *)(0x01001008));
    //kprintf("after pid: %d, frame num %d\n", currpid, proctab[currpid].page_directory[4].pd_base);
    *(int *)(0x01001008) = 10;
    kprintf("after pid: %d, frame num %d\n", currpid, proctab[currpid].page_directory[4].pd_base);
    kprintf("after [2024 * 4096 + 8] = %d\n", *(int *)(2024 * 4096 + 8));
    kprintf("after [0x01001008] = %d\n", *(int *)(0x01001008));
    *(int *)(0x01001008) = 10;
    *(int *)(0x01000000) = 1;
    kprintf("after after [0x01001008] = %d\n", *(int *)(0x01001008));
    kprintf("after after [2025 * 4096] = %d\n", *(int *)(2025 * 4096));
    kprintf("fault number %d\n", get_faults());

    /*
    pid = getpid();

    array = (int *)vgetmem(10000);




    for (i = 2049; i >= 2048; i--) {
        array[i] = 10;
        kprintf("%d %d\n", i, array[i]);
    }


    vfreemem(array, 10000);
    */
    return 0;
}
