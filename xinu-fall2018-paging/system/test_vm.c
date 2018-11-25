/*  test_vm.c - test_vm */

#include <xinu.h>

process test_vm(char c)
{
    pid32   pid;
    int *array;
    uint32  i;

    kprintf("before %d\n", *(int *)(2024 * 4096 + 8));
    *(int *)(2024 * 4096 + 8) = 9;
    asm("cli");
    *(int *)(0x01001008) = 10;
    asm("sti");
    kprintf("pid: %d, frame num %d\n", currpid, ((pt_t *)((proctab[currpid].page_directory[4]).pd_base * NBPG))[1].pt_base);
    kprintf("after %d\n", *(int *)(2024 * 4096 + 8));
    kprintf("after %d\n", *(int *)(0x01001008));
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
