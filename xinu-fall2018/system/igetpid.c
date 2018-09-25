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

    asm volatile ("pushl  %%eax\n\t"        /* Save the old EAX value in the user stack  */
                  "movl   $3, %%eax\n\t"    /* Set syscall #3 to EAX            */
                  "int    $0x21\n\t"        /* Trap to interrupt table #33      */
                  "movl   %%eax, %0\n\t"    /* Save the return value to the variable syscall_vall   */
                  "popl   %%eax\n\t"        /* Restore the value of EAX         */
                : "=r" (syscall_val)        /* Extended inline assembly: syscall_vall is an output variable */
                :
                : "%eax");                  /* Tell the gcc that we used EAX in this inline assembly   */

    return syscall_val;
}
