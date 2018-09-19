/*
 * User: wang4113
 * data: 09/19/2018
 */

/* igetpid.c - igetpid */

#include <xinu.h>

/*-----------------------------------------------------------
 * igetpid  -  The wrapper function of system call getpid()
 *-----------------------------------------------------------
 */

int igetpid(void)
{
    int syscall_val;

    asm volatile ("movl   $3, %%eax\n\t"
                  "int    $0x21\n\t"
                  "movl   %%eax, %0\n\t"
                  : "=r" (syscall_val));

    return syscall_val;
}