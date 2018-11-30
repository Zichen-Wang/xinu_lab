/*
 * user: wang4113
 * data: 11/01/2018
 */

/* paging_init.c - paging_init */

#include <xinu.h>


struct  frame_entry inverted_page_table[NFRAMES];
pt_t    *shared_page_table[5];
struct  bs_map_entry backing_store_map[MAX_BS_ENTRIES];

uint16  pgrpolicy;

uint32  page_fault_count;

int32   frameq_head, frameq_tail;

/*---------------------------------------------------------------------------
 *  paging_init  -  Define and initialize all necessary page data structures.
 *---------------------------------------------------------------------------
 */

void paging_init(void)
{
    uint32 i;

    /* Initialize frames */
    for (i = 0; i < NFRAMES; i++) {
        inverted_page_table[i].fstate = F_FREE;
    }

    /* Initialize shared page tables to NULL  */
    for (i = 0; i < 5; i++) {
        shared_page_table[i] = NULL;
    }

    /* Initialize backing store map */
    for (i = 0; i < MAX_BS_ENTRIES; i++) {
        backing_store_map[i].bs_state = BS_FREE;
    }

    /* set the page replacement policy  */
    set_pgrpolicy(0);   /* The argument spolicy being 0 sets policy to FIFO */

    page_fault_count = 0;
}
