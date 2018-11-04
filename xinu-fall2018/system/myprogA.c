/*  myprogA.c - myprogA */

#include <xinu.h>

process myprogA(void)
{
    pid32   pid;
    int     *array = NULL;

    pid = getpid();

    //array = (int *)(getmem(40));
    printf("\n%d: [0x%08X]\n", pid, array);

    return 0;
}
