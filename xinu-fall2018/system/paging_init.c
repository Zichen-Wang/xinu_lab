/*
 * user: wang4113
 * data: 11/01/2018
 */

/* paging_init.c - paging_init */

#include <xinu.h>


struct frame_t inverted_page_table[NFRAMES];

pt_t * shared_page_table[5];


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
        inverted_page_table[i].reference_count = 0;

    }

    for (i = 0; i < 5; i++) {
        shared_page_table[i] = NULL;
    }
}
