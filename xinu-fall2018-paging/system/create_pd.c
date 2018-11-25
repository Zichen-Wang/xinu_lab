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

pd_t * create_pd(pid32 pid)
{
    uint32  i;
    int     frame_num;
    pd_t    *pd;

    frame_num = findfframe(PAGE_DIRECTORY_TABLE);    /* Get a new frame for page directory   */

    if (frame_num == SYSERR) {     /* Error    */
        kprintf("Error to create a new page directory!\n");
        return (pd_t *)(SYSERR);
    }


    /* Set the inverted_page_table entry    */
    inverted_page_table[frame_num].fstate = F_USED_PD;
    inverted_page_table[frame_num].pid = pid;

    inverted_page_table[frame_num].fnext = -1;

    hook_pdir_create(frame_num);


    /* Initialize the page directory    */
    pd = (pd_t *)(NBPG * (frame_num + FRAME0));     /* base address of page directory   */
    for (i = 0; i < PAGE_DIRECTORY_ENTRIES; i++) {
        pd[i].pd_pres     = 0;
        pd[i].pd_write    = 1;
        pd[i].pd_user     = 0;
        pd[i].pd_pwt      = 0;
        pd[i].pd_pcd      = 0;
        pd[i].pd_acc      = 0;
        pd[i].pd_mbz      = 0;
        pd[i].pd_fmb      = 0;
        pd[i].pd_global   = 0;
        pd[i].pd_avail    = 0;
        pd[i].pd_base     = 0;
    }

    return pd;

};
