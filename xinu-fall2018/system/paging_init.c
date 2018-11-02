/*
 * user: wang4113
 * data: 11/01/2018
 */

/* paging_init.c - paging_init */

#include <xinu.h>


struct frame_t inverted_page_table[NFRAMES];

char * shared_page_table[5];


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

    for (i = 0; i < 5; i++) {
        shared_page_table[i] = NULL;
    }
}
