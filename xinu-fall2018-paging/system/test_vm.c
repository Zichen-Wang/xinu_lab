/*  test_vm.c - test_vm */

#include <xinu.h>

process test_vm(char c)
{
    pid32   pid;
    char *array;
    uint32  i;
    /*
    kprintf("before [2024 * 4096 + 8] = %d\n", *(int *)(2024 * 4096 + 8));
    kprintf("before pid: %d, page directory num %d\n", currpid, (uint32)(proctab[5].page_directory) / NBPG);
    kprintf("before pid: %d, frame num %d\n", currpid, proctab[currpid].page_directory[4].pd_base);
    *(int *)(2024 * 4096 + 8) = 9;
    //kprintf("before %d\n", *(int *)(0x01001008));
    kprintf("after pid: %d, frame num %d\n", currpid, ((pt_t*)(NBPG * proctab[currpid].page_directory[4].pd_base))[1].pt_base);

    //asm volatile ("cli");
    *(int *)(0x01001008) = 10;
    //asm volatile ("sti");
    kprintf("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n");
    kprintf("after pid: %d, frame num %d\n", currpid, ((pt_t*)(NBPG * proctab[currpid].page_directory[4].pd_base))[1].pt_base);
    kprintf("after pid: %d, page table num %d\n", currpid, proctab[currpid].page_directory[4].pd_base);
    kprintf("after [2024 * 4096 + 8] = %d\n", *(int *)(2024 * 4096 + 8));
    kprintf("after [0x01001008] = %d\n", *(int *)(0x01001008));
    kprintf("after twice [0x01001008] = %d\n", *(int *)(0x01001008));
    //asm volatile("cli");
    *(int *)(0x01000000) = 1;
    //asm volatile("sti");
    kprintf("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n");
    kprintf("after pid: %d, frame num %d\n", currpid, ((pt_t*)(NBPG * proctab[currpid].page_directory[4].pd_base))[0].pt_base);
    kprintf("after [0x01000000] = %d\n", *(int *)(0x01000000));
    kprintf("after after [2025 * 4096] = %d\n", *(int *)(2025 * 4096));
    kprintf("fault number %d\n", get_faults());
    */

    pid = getpid();

    array = vgetmem(60 * 4096);




    for (i = 0; i < 60 * 4096; i++) {
        array[i] = c;
        //array[i] = c;
    }

    for (i = 0; i < 60 * 4096; i += 4096) {
        kprintf("%d\n", array[i]);
    }

    for (i = 2024; i < 2030; i++) {
        kprintf("frame %d: %d\n", i, *(char *)(i * 4096));
    }


    vfreemem(array, 60 * 4096);

    return 0;
}
