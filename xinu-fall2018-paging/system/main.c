/*  main.c  - main */

#include <xinu.h>
#include <stdio.h>

process	main(void)
{
	/* Start the network */
	/* DO NOT REMOVE OR COMMENT THIS CALL */
	netstart();

	/* Initialize the page server */
	/* DO NOT REMOVE OR COMMENT THIS CALL */
	psinit();
	/*
	resume(create(test_app, 1024, 19, "test_app", 1, 'a'));
	resume(create(test_app, 1024, 19, "test_app", 1, 'b'));
	resume(create(test_app, 1024, 19, "test_app", 1, 'c'));
	*/

	resume(vcreate(test_vm, 1024, 100, 19, "test_vm", 1, 'a'));
	resume(vcreate(test_vm, 1024, 100, 19, "test_vm", 1, 'b'));
	resume(vcreate(test_vm, 1024, 100, 19, "test_vm", 1, 'c'));
	/*
	resume(vcreate(test_vm_2, 1024, 100, 19, "test_vm_2", 2, 1, 2));
	resume(vcreate(test_vm_2, 1024, 100, 19, "test_vm_2", 2, 23, 17));
	*/
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
