/*  main.c  - main */

#include <xinu.h>

process	main(void)
{
	xminsec_t uptime;
	long x;
	int *esp_before, *esp_after;			/* Used in 5.3  */
	pid32 pid;							/* Used in 5.3  */

    /* 3.2 Move the welcome message to a function welcome() */
    /*
    kprintf("\nHello World!\n");
    kprintf("\nI'm the first XINU app and running function main() in system/main.c.\n");
    kprintf("\nI was created by nulluser() in system/initialize.c using create().\n");
    kprintf("\nMy creator will turn itself into the do-nothing null process.\n");
    kprintf("\nI will create a second XINU app that runs shell() in shell/shell.c as an example.\n");
    kprintf("\nYou can do something else, or do nothing; it's completely up to you.\n");
    kprintf("\n...creating a shell\n");
    */

    /* 4.2 Test of print parent process id.	*/
    sleepms(10);	/* Let main sleep for 10ms in order that startup process exits	*/
    kprintf("\nThe ppid of main process is %d.\n\n", getppid());


    /* 4.1 Retrieve and display the uptime in minutes and seconds */
    xuptime(&uptime);
    kprintf("\nThe uptime since XINU was bootstrapped:\n%d min, %d sec.\n", uptime.upmin, uptime.upsec);
	kprintf("\n\n");
    

    /* 5.1 Changing byte order using assembly code */
    x = 0x0123ABCD;
	kprintf("\noriginal: 0x%08X\n", x);
	kprintf("version 1: 0x%08X\n", revbyteorder(x));
	kprintf("version 2: 0x%08X\n", revbyteorder_inline(x));
	kprintf("version 3: 0x%08X\n", revbyteorder_cfun(x));
    kprintf("version 4: 0x%08X\n", revbyteorder_gcc(x));
	kprintf("\n\n");
    

    /* 5.2 Checking segment boundaries	*/
    printsegaddress();
	kprintf("\n\n");
    



	/* 5.3 Run-time stack: process creation and function call	*/
	/* Get the address of the top of run-time stack before creating "myprogA" process	*/

	asm volatile ("movl %%esp, %0\n\t"
			  	: "=r" (esp_before));


	resume(create(myprogA, 1024, 21, "myprogA (5.3)", 0, NULL));	/* create the "myprogA" process	*/

	/* Get the address of the top of run-time stack after creating and resuming "myprogA" process	*/

	asm volatile ("movl %%esp, %0 \n\t"
				: "=r" (esp_after));


	/* Print the address of the top of run-time stack before and after creating "myprogA" process	*/

	pid = getpid();
	kprintf("\nProcess Name: %s\n", proctab[pid].prname);
	kprintf("Before myprogA() is created, the address of the top of the run-time stack is [0x%08X].\n",
			(uint32)esp_before);
	kprintf("Its content is %d.\n", *esp_before);
	kprintf("\n\n");

	kprintf("After myprogA() has been created and resumed, the address of the top of the run-time stack is [0x%08X].\n",
			(uint32)esp_after);
	kprintf("Its content is %d.\n", *esp_after);
	kprintf("\n\n");





	/* 5.4 Comparing two run-time stacks	*/
	// resume(create(myprogA, 1024, 19, "myprogA (5.4)", 0, NULL));	/* create the "myprogA" process		*/
	// resume(create(myfuncA, 1024, 18, "myfuncA (5.4)", 0, NULL));	/* create the "myfuncA" process		*/
	// sleepms(200);												/* Main wait for these two process	*/



	/* 6 Hijacking a process via stack smashing	*/
	//resume(create(myprogA, 1024, 21, "myprogA (6)", 0, NULL));	/* create the "myprogA" process		*/
	//sleepms(5000);												/* Main wait for "mygrogA" process	*/

	/* Run the Xinu shell */

	recvclr();
	resume(create(shell, 8192, 50, "shell", 1, CONSOLE));

	/* Wait for shell to exit and recreate it */

	while (TRUE) {
		receive();
		sleepms(200);

		/* 4.1 Retrieve and display the uptime in minutes and seconds when recreating shell	*/
		xuptime(&uptime);
		kprintf("\nThe uptime since XINU was bootstrapped:\n%d min, %d sec.\n", uptime.upmin, uptime.upsec);
		kprintf("\n\n");

		kprintf("\n\nMain process recreating shell\n\n");
		resume(create(shell, 4096, 20, "shell", 1, CONSOLE));
	}
	return OK;
    
}
