/*  main.c  - main */

#include <xinu.h>

process	main(void)
{
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

    xminsec_t uptime;

    xuptime(&uptime);
    kprintf("\nThe uptime since XINU was bootstrapped:\n%d min, %d sec\n", uptime.upmin, uptime.upsec);

    /* Test of function revbyteorder */
    long x = 10;
	kprintf("\noriginal: %d; reversed %d\n", x, revbyteorder(x));


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

		kprintf("\n\nMain process recreating shell\n\n");
		resume(create(shell, 4096, 20, "shell", 1, CONSOLE));
	}
	return OK;
    
}
