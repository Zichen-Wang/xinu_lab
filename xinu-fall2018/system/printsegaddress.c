/* printsegaddress.c - printsegaddress */

#include <xinu.h>

/*-------------------------------------------------------------------------------------------------------
 * printsegaddress  -  Print the addresses of the end of the text, data, and bss segments of the XINU OS
 *-------------------------------------------------------------------------------------------------------
 */

void printsegaddress()
{
    char i, *p;
    //kprintf("\nThe address of the begin of the text segment is [0x%08X].\n",
    //      (uint32)&text);
    kprintf("\nThe address of the end   of the text segment is [0x%08X].\n",
            (uint32)&etext - 1);
    //kprintf("\nThe address of the begin of the data segment is [0x%08X].\n",
    //      (uint32)&data);
    kprintf("\nThe address of the end   of the data segment is [0x%08X].\n",
            (uint32)&edata - 1);
    //kprintf("\nThe address of the begin of the bss  segment is [0x%08X].\n",
    //      (uint32)&bss);
    kprintf("\nThe address of the end   of the bss  segment is [0x%08X].\n",
            (uint32)&ebss - 1);

    p = (char *)&etext;
    kprintf("\nThe 6 bytes preceding the end of the text segment from low to high are:");
    for (i = 6; i >= 1; i--)
        kprintf(" 0x%02X", (byte)*(p - i));
    kprintf("\n");

    p = (char *)&edata;
    kprintf("\nThe 6 bytes preceding the end of the data segment from low to high are:");
    for (i = 6; i >= 1; i--)
        kprintf(" 0x%02X", (byte)*(p - i));
    kprintf("\n");

    p = (char *)&ebss;
    kprintf("\nThe 6 bytes preceding the end of the bss  segment from low to high are:");
    for (i = 6; i >= 1; i--)
        kprintf(" 0x%02X", (byte)*(p - i));
    kprintf("\n");

    kprintf("---------------------------------------------------------");

    p = (char *)&etext;
    kprintf("\nThe 6 bytes following the end of the text segment from low to high are:");
    for (i = 0; i < 6; i++)
        kprintf(" 0x%02X", (byte)*(p + i));
    kprintf("\n");

    p = (char *)&edata;
    kprintf("\nThe 6 bytes following the end of the data segment from low to high are:");
    for (i = 0; i < 6; i++)
        kprintf(" 0x%02X", (byte)*(p + i));
    kprintf("\n");

    p = (char *)&ebss;
    kprintf("\nThe 6 bytes following the end of the bss  segment from low to high are:");
    for (i = 0; i < 6; i++)
        kprintf(" 0x%02X", (byte)*(p + i));
    kprintf("\n");

}
