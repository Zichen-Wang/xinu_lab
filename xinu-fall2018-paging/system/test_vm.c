/*  test_vm.c - test_vm */

#include <xinu.h>

process test_vm(char c)
{
    pid32   pid;
    int *array;
    uint32  i;

    kprintf("before %d\n", *(int *)(2024 * 4096 + 8));
    kprintf("before pid: %d, frame num %d\n", currpid, proctab[currpid].page_directory[4].pd_base);
    *(int *)(2024 * 4096 + 8) = 9;
    asm("cli");
    kprintf("before %d\n", *(int *)(0x01001008));
    //*(int *)(0x01001008) = 10;
    asm("sti");
    kprintf("after pid: %d, frame num %d\n", currpid, proctab[currpid].page_directory[4].pd_base);
    kprintf("after %d\n", *(int *)(2024 * 4096 + 8));
    kprintf("fault number %d\n", get_faults());
    pid = getpid();


    array = (int *)vgetmem(10000);




    for (i = 1026; i >= 1024; i--) {
        array[i] = 10;
        kprintf("%d %d\n", i, array[i]);
    }


    vfreemem(array, 10000);

    return 0;
}
