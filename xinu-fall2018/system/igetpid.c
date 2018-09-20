/*
 * User: wang4113
 * date: 09/20/2018
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

    asm volatile ("pushl  %%eax\n\t"
                  "movl   $3, %%eax\n\t"
                  "int    $0x21\n\t"
                  "movl   %%eax, %0\n\t"
                  "popl   %%eax\n\t"
                  : "=r" (syscall_val));

    return syscall_val;
}