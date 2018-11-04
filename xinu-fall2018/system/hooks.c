/*
 * user: wang4113
 * data: 11/01/2018
 */

/* hooks.c - hook_ptable_create, hook_ptable_delete, hook_pfault, hook_pswap_out, get_faults */

#include <xinu.h>

#define HOOK_DEBUG

/*--------------------------------------------------------------------------------
 *  hook_ptable_create  -  This hook should be called when creating a page table
 *--------------------------------------------------------------------------------
 */

static int page_fault_count = 0;

void hook_ptable_create(unsigned int pagenum)
{
#ifdef HOOK_DEBUG
    kprintf("Created page table %d\n\n", pagenum);
#endif
}

void hook_ptable_delete(unsigned int pagenum)
{
#ifdef HOOK_DEBUG
    kprintf("Deleted page table %d\n\n", pagenum);
#endif
}

void hook_pfault(char *addr)
{
    page_fault_count++;
#ifdef HOOK_DEBUG
    kprintf("Page fault for address 0x%08X\n\n", addr);
#endif
}

void hook_pswap_out(unsigned int pagenum, int framenum)
{
#ifdef HOOK_DEBUG
    kprintf("Replacing frame number %d, virtual page %d\n\n", framenum, pagenum);
#endif
}

int get_faults()
{
    return page_fault_count;
}