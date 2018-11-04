/*  main.c  - main */

#include <xinu.h>
#include <stdio.h>

#define HOOK_DEBUG

process	main(void)
{
	uint32	i;
	/* Start the network */
	/* DO NOT REMOVE OR COMMENT THIS CALL */
	netstart();

	/* Initialize the page server */
	/* DO NOT REMOVE OR COMMENT THIS CALL */
	psinit();

	for (i = 0; i < 1000; i++) {
		resume(vcreate(myprogA, 1024, 100, 50, "myprogA", 0, NULL));
	}

	//resume(create(myprogA, 1024, 19, "myprogA_1", 0, NULL));
	//resume(create(myprogA, 1024, 19, "myprogA_2", 0, NULL));
	sleepms(100);

	kprintf("\n...creating a shell\n");
	recvclr();
	resume(create(shell, 8192, 50, "shell", 1, CONSOLE));

	/* Wait for shell to exit and recreate it */

	while (TRUE) {
		receive();
		sleepms(200);
		kprintf("\n\nMain process recreating shell\n\n");
		resume(create(shell, 4096, 20, "shell", 1, CONSOLE));
	}
	return OK;
}
