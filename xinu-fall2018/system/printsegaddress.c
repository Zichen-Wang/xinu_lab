/* printsegaddress.c - printsegaddress */

#include <xinu.h>

/*-------------------------------------------------------------------------------------------------------
 * printsegaddress  -  Print the addresses of the end of the text, data, and bss segments of the XINU OS
 *-------------------------------------------------------------------------------------------------------
 */

void printsegaddress()
{
    kprintf("\nThe address of the end of text segment is [0x%08X].\n", (uint32)&etext - 1);
    kprintf("\nThe address of the end of data segment is [0x%08X].\n", (uint32)&bss - 1);
    kprintf("\nThe address of the end of bss  segment is [0x%08X].\n", (uint32)&ebss - 1);
}