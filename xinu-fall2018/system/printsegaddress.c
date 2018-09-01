/* printsegaddress.c - printsegaddress */

#include <xinu.h>

/*-------------------------------------------------------------------------------------------------------
 * printsegaddress  -  Print the addresses of the end of the text, data, and bss segments of the XINU OS
 *-------------------------------------------------------------------------------------------------------
 */

void printsegaddress()
{
    int i, *p;
    //kprintf("\nThe address of the begin of text segment is [0x%08X].\n",
    //      (uint32)&text);
    kprintf("\nThe address of the end   of text segment is [0x%08X].\n",
            (uint32)&etext - 1);
    //kprintf("\nThe address of the begin of data segment is [0x%08X].\n",
    //      (uint32)&data);
    kprintf("\nThe address of the end   of data segment is [0x%08X].\n",
            (uint32)&edata - 1);
    //kprintf("\nThe address of the begin of bss  segment is [0x%08X].\n",
    //      (uint32)&bss);
    kprintf("\nThe address of the end   of bss  segment is [0x%08X].\n",
            (uint32)&ebss - 1);

    p = &etext;
    kprintf("\nThe 6 bytes preceding the end of the text segment are:");
    for (i = 6; i >= 1; i--)
        kprintf(" 0x%02X", *(p - i));
    kprintf("\n");

    p = &edata;
    kprintf("\nThe 6 bytes preceding the end of the data segment are:");
    for (i = 6; i >= 1; i--)
        kprintf(" 0x%02X", *(p - i));
    kprintf("\n");

    p = &ebss;
    kprintf("\nThe 6 bytes preceding the end of the bss  segment are:");
    for (i = 6; i >= 1; i--)
        kprintf(" 0x%02X", *(p - i));
    kprintf("\n");

    kprintf("---------------------------------------------------------");

    p = &etext;
    kprintf("\nThe 6 bytes following the end of the text segment are:");
    for (i = 0; i < 6; i++)
        kprintf(" 0x%02X", *(p + i));
    kprintf("\n");

    p = &edata;
    kprintf("\nThe 6 bytes following the end of the data segment are:");
    for (i = 0; i < 6; i++)
        kprintf(" 0x%02X", *(p + i));
    kprintf("\n");

    p = &ebss;
    kprintf("\nThe 6 bytes following the end of the bss  segment are:");
    for (i = 0; i < 6; i++)
        kprintf(" 0x%02X", *(p + i));
    kprintf("\n");

}
