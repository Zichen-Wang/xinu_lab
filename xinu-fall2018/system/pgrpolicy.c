/*
 * user: wang4113
 * data: 11/04/2018
 */

/* pgroplicy.c - pgroplicy */

#include <xinu.h>

/*----------------------------------------------------------
 *  pgroplicy  -  Change page replacement policy -- spolicy
 *----------------------------------------------------------
 */

syscall pgrpolicy(uint16 sp)
{
    if (sp == 0) {
        spolicy = FIFO;
    }
    else if (sp == 1) {
        spolicy = MYPOLICY;
    }

    return OK;
}

