/*  main.c  - main */

#include <xinu.h>

process	main(void)
{
	xminsec_t uptime;
	long x;
	pid32 pid;

    /* Move the welcome message to a function welcome() */
    /*
    	kprintf("\nHello World!\n");
    	kprintf("\nI'm the first XINU app and running function main() in system/main.c.\n");
    	kprintf("\nI was created by nulluser() in system/initialize.c using create().\n");
    	kprintf("\nMy creator will turn itself into the do-nothing null process.\n");
    	kprintf("\nI will create a second XINU app that runs shell() in shell/shell.c as an example.\n");
    	kprintf("\nYou can do something else, or do nothing; it's completely up to you.\n");
    	kprintf("\n...creating a shell\n");
    */

    /* Retrieve and display the uptime in minutes and seconds */
    xuptime(&uptime);
    kprintf("\nThe uptime since XINU was bootstrapped:\n%d min, %d sec\n", uptime.upmin, uptime.upsec);
	kprintf("\n\n");


    /* Test of function revbyteorder */
    x = 10;
	kprintf("\noriginal: %d\n", x);
	kprintf("version 1: %d\n", revbyteorder(x));
	kprintf("version 2: %d\n", revbyteorder_inline(x));
	kprintf("version 3: %d\n", revbyteorder_cfun(x));
    kprintf("version 4: %d\n", revbyteorder_gcc(x));
	kprintf("\n\n");


    /* Test of the addresses of the end of text, data and bss segments	*/
	printsegaddress();
	kprintf("\n\n");




	/* Test of the address of the top of run-time stack	*/
	pid = getpid();
	kprintf("\nProcess Name: %s\n", (uint32)proctab[pid].prname);
	kprintf("Before myprogA() is created, the address of the top of the run-time stack is [0x%08X].\n",
			(uint32)proctab[pid].prstkptr);
	kprintf("Its content is 0x%02X\n", (byte)*(proctab[pid].prstkptr));
	kprintf("\n\n");

	kprintf("\nCreating myprogA...\n");

	recvclr();
	resume(create(myprogA, 1024, 20, "myprogA", 0));	/* create the "myprogA" process	*/

	receive();											/* wait for "myprogA" to exit	*/
	sleepms(200);

	kprintf("\nProcess Name: %s\n", (uint32)proctab[pid].prname);
	kprintf("After myprogA() has been created and resumed, the address of the top of the run-time stack is [0x%08X].\n",
			(uint32)proctab[pid].prstkptr);
	kprintf("Its content is 0x%02X\n", (byte)*(proctab[pid].prstkptr));
	kprintf("\n\n");




	/* Run the Xinu shell */

	recvclr();
	resume(create(shell, 8192, 50, "shell", 1, CONSOLE));

	/* Wait for shell to exit and recreate it */

	while (TRUE) {
		receive();
		sleepms(200);

		/* Retrieve and display the uptime in minutes and seconds when recreating shell	*/
		xuptime(&uptime);
		kprintf("\nThe uptime since XINU was bootstrapped:\n%d min, %d sec\n", uptime.upmin, uptime.upsec);
		kprintf("\n\n");

		kprintf("\n\nMain process recreating shell\n\n");
		resume(create(shell, 4096, 20, "shell", 1, CONSOLE));
	}
	return OK;
    
}
