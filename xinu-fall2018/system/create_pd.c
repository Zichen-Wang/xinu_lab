/*
 * user: wang4113
 * data: 11/01/2018
 */

/* create_pd.c - create_pd */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  create_pd  -  Create a new page directory for a process
 *------------------------------------------------------------------------
 */

char * create_pd(pid32 pid)
{
    uint32 i;
    int frame_num;
    pd_t *pd_entry;

    frame_num = findfframe(PAGE_DIRECTORY_TABLE);    /* Get a new frame for page directory   */

    if (frame_num == SYSERR) {     /* Error    */
        kprintf("Error to create a new page directory!\n");
        return (char *)(SYSERR);
    }

    /* Set the inverted_page_table entry    */
    inverted_page_table[frame_num].fstate = F_PD;
    inverted_page_table[frame_num].pid = pid;


    /* Initialize the page directory    */
    for (i = 0; i < PAGE_DIRECTORY_ENTRIES; i++) {
        pd_entry = (pd_t *)(NBPG * (FRAME0 + frame_num) + i * 4);  /* address of page directory entry   */

        pd_entry -> pd_pres     = 0;
        pd_entry -> pd_write    = 0;
        pd_entry -> pd_user     = 0;
        pd_entry -> pd_pwt      = 0;
        pd_entry -> pd_pcd      = 0;
        pd_entry -> pd_acc      = 0;
        pd_entry -> pd_mbz      = 0;
        pd_entry -> pd_fmb      = 0;
        pd_entry -> pd_global   = 0;
        pd_entry -> pd_avail    = 0;
        pd_entry -> pd_base     = 0;

    }

    return (char *)(NBPG * (FRAME0 + frame_num));

};
