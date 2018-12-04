/*
 * user: wang4113
 * data: 12/03/2018
 */

#include <xinu.h>

/* find_bs_map.c - find_bs_map */

int find_bs_map(uint32 vp, pid32 pid)
{
    int i;
    for (i = 0; i < MAX_BS_ENTRIES; i++) {
        if (backing_store_map[i].pid == pid) {
            if (backing_store_map[i].virt_base_num <= vp
             && vp <= backing_store_map[i].virt_base_num + backing_store_map[i].npages) {
                return i;
            }
        }
    }
    kprintf("We cannot find backing store s for virtual page %d, pid %d.\n", vp, pid);
    return SYSERR;
}
