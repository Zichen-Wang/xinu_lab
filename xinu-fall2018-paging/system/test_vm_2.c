/*  test_vm_2.c - test_vm_2 */

#include <xinu.h>

process test_vm_2(int s, int d)
{
    int *array;
    uint32  i, j;

    array = (int *)(vgetmem(1000 * 4));
    array[0] = s;
    for (i = 1; i < 1000; i++)
        array[i] = array[i - 1] + d;

    //kprintf("----%d----\n", array[123]);
    if (array[123] != s + d * 123) {
        kprintf("\nFAILED\n");
        vfreemem((char *)(array), 1000 * 4);
        return 0;
    }


    vfreemem((char *)(array), 1000 * 4);

    array = (int *)(vgetmem(10000 * 4));
    array[0] = s;
    j = s;
    for (i = 1; i < 10000; i++) {
        array[i] = array[i - 1] * 3 % d;
        if (i <= 123) {
            j = j * 3 % d;
        }
    }

    //kprintf("----%d----\n", array[123]);
    if (array[123] != j) {
        kprintf("\nFAILED\n");
        vfreemem((char *)(array), 10000 * 4);
        return 0;
    }

    vfreemem((char *)(array), 10000 * 4);
    kprintf("\nPASSED\n");

    return 0;
}
