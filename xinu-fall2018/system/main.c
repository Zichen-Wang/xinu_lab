/*  main.c  - main */

#include <xinu.h>

process	main(void)
{
	//xminsec_t uptime;					/* Used in Lab1 4.1	*/
	//long x;								/* Used in Lab1 5.1	*/
	//int *esp_before, *esp_after;		/* Used in Lab1 5.3  */
	//pid32 pid;							/* Used in Lab1 5.3 & Lab2 3.3 */
	rmsparam_t test_rms[4];             /* Used in Lab3 4   */

    /* Lab1 3.2: Move the welcome message to a function welcome() */
    /*
    kprintf("\nHello World!\n");
    kprintf("\nI'm the first XINU app and running function main() in system/main.c.\n");
    kprintf("\nI was created by nulluser() in system/initialize.c using create().\n");
    kprintf("\nMy creator will turn itself into the do-nothing null process.\n");
    kprintf("\nI will create a second XINU app that runs shell() in shell/shell.c as an example.\n");
    kprintf("\nYou can do something else, or do nothing; it's completely up to you.\n");
    kprintf("\n...creating a shell\n");
    */

    /* Lab1 4.2: Test of print parent process id.	*/
    //sleepms(10);	/* Lab1 4.2: Let main sleep for 10ms in order that startup process exits.	*/
	/* Lab1 4.2 Print the PPID of the main process	*/
    //kprintf("\nThe ppid of the main process is %d.\n\n", getppid());


    

    /* Lab1 5.1: Changing byte order using assembly code */
    /*
    x = 0x0123ABCD;
	kprintf("Lab1 5.1: The number to be reversed byte is 0x%08X.\n", x);
	kprintf("Version 1: 0x%08X\n", revbyteorder_asm(x));
	kprintf("Version 2: 0x%08X\n", revbyteorder_inline(x));
	kprintf("Version 3: 0x%08X\n", revbyteorder(x));
    kprintf("Version 4: 0x%08X\n", revbyteorder_gcc(x));
	kprintf("\n\n");
    */
    

    /* Lab1 5.2: Checking segment boundaries	*/
    /*
    printsegaddress();
	kprintf("\n\n");
    */
    



	/* Lab1 5.3: Run-time stack: process creation and function call	*/
	/* Lab1 5.3: Get the address of the top of run-time stack before creating "myprogA" process	*/
	/*
	asm volatile ("movl %%esp, %0\n\t"
			  	: "=r" (esp_before));
	*/

	//resume(create(myprogA, 1024, 21, "myprogA (5.3)", 0, NULL));	/* 5.3: create the "myprogA" process	*/

	/* Lab1 5.3: Get the address of the top of run-time stack after creating and resuming "myprogA" process	*/
	/*
	asm volatile ("movl %%esp, %0 \n\t"
				: "=r" (esp_after));
	*/

	/* Lab1 5.3: Print the address of the top of run-time stack before and after creating "myprogA" process	*/
	/*
	pid = getpid();
	kprintf("\nProcess Name: %s\n", proctab[pid].prname);
	kprintf("Before myprogA() is created, the address of the top of the run-time stack is [0x%08X].\n",
			(uint32)esp_before);
	kprintf("------The 32 bytes preceding the top of the stack are as follows------\n");
	kprintf("[0x%08X] 0x%08X (top)\n", (uint32)esp_before, *esp_before);
	kprintf("[0x%08X] 0x%08X\n", (uint32)(esp_before + 1), *(esp_before + 1));
	kprintf("[0x%08X] 0x%08X\n", (uint32)(esp_before + 2), *(esp_before + 2));
	kprintf("[0x%08X] 0x%08X\n", (uint32)(esp_before + 3), *(esp_before + 3));
	kprintf("[0x%08X] 0x%08X\n", (uint32)(esp_before + 4), *(esp_before + 4));
	kprintf("[0x%08X] 0x%08X\n", (uint32)(esp_before + 5), *(esp_before + 5));
	kprintf("[0x%08X] 0x%08X\n", (uint32)(esp_before + 6), *(esp_before + 6));
	kprintf("[0x%08X] 0x%08X\n", (uint32)(esp_before + 7), *(esp_before + 7));

	kprintf("\n\n");

	kprintf("\nProcess Name: %s\n", proctab[pid].prname);
	kprintf("After myprogA() has been created and resumed, the address of the top of the run-time stack is [0x%08X].\n",
			(uint32)esp_after);
	kprintf("------The 32 bytes preceding the top of the stack are as follows------\n");
	kprintf("[0x%08X] 0x%08X (top)\n", (uint32)esp_after, *esp_after);
	kprintf("[0x%08X] 0x%08X\n", (uint32)(esp_after + 1), *(esp_after + 1));
	kprintf("[0x%08X] 0x%08X\n", (uint32)(esp_after + 2), *(esp_after + 2));
	kprintf("[0x%08X] 0x%08X\n", (uint32)(esp_after + 3), *(esp_after + 3));
	kprintf("[0x%08X] 0x%08X\n", (uint32)(esp_after + 4), *(esp_after + 4));
	kprintf("[0x%08X] 0x%08X\n", (uint32)(esp_after + 5), *(esp_after + 5));
	kprintf("[0x%08X] 0x%08X\n", (uint32)(esp_after + 6), *(esp_after + 6));
	kprintf("[0x%08X] 0x%08X\n", (uint32)(esp_after + 7), *(esp_after + 7));
	kprintf("\n\n");
	kprintf("\n\n");
	*/


	/* Lab1 5.4: Comparing two run-time stacks	*/
	//resume(create(myprogA, 1024, 19, "myprogA (5.4)", 0, NULL));	/* Lab1 5.4: create the "myprogA" process			*/
	//resume(create(myfuncA, 1024, 18, "myfuncA (5.4)", 1, 10));		/* Lab1 5.4: create the "myfuncA" process			*/
	//sleepms(200);													/* Lab1 5.4: "Main" waits for these two processes	*/



	/* Lab1 6: Hijacking a process via stack smashing	*/
	//resume(create(myprogA, 1024, 21, "myprogA (6)", 0, NULL));	/* Lab1 6: create the "myprogA" process			*/
	//sleepms(5000);												/* Lab1 6: "Main" waits for the "mygrogA" process	*/



	/* Lab1 4.1: Retrieve and display the uptime in minutes and seconds */
	//xuptime(&uptime);
	//kprintf("\nThe uptime since XINU was bootstrapped:\n%d min, %d sec.\n", uptime.upmin, uptime.upsec);
	//kprintf("\n\n");

	/*
	 * User: wang4113
	 * date: 09/19/2018
	 */
	/* Lab2 3.1: Print the values of clktimemilli and clktime	*/
	//sleepms(1379);	/* Sleep a while just for test	*/
	//kprintf("\nThe uptime since XINU was bootstrapped in seconds: %ds.\n", clktime);
	//kprintf("\nThe uptime since XINU was bootstrapped in milliseconds: %dms.\n", clktimemilli);


    /*
     * User: wang4113
     * date: 09/24/2018
     */
    /* Lab2 3.2: Monitoring process gross CPU usage */
    //resume(pid=create(myTestProcA, 1024, 21, "myTestProcA", 0, NULL));
	/* Test the gross CPU usage of the non-current process  */
	//kprintf("non-current process gross CPU usage: %d ms\n",
	//		proctab[pid].pgrosscpu);
    //sleepms(2000);

	/*
     * User: wang4113
     * date: 09/25/2018
     */
	/* Lab2 3.3: Monitoring process waiting time */
	/*
	resume(create(myTestProcB, 1024, 19, "myTestProcB_0", 0, NULL));
	resume(create(myTestProcB, 1024, 18, "myTestProcB_1", 0, NULL));
	*/
	//sleepms(2000);		/* Lab2 3.3: "Main" waits for these "myTestProcB" processes		*/

	/*
	 * User: wang4113
	 * date: 09/19/2018
	 */
	/* Lab2 4: Test of print process id by igetpid()	*/
	/* Spawn several app processes to test igetpid()	*/
    /*
	resume(create(myTestProcC, 1024, 19, "myTestProcC_0", 0, NULL));
	resume(create(myTestProcC, 1024, 18, "myTestProcC_1", 0, NULL));
	resume(create(myTestProcC, 1024, 17, "myTestProcC_2", 0, NULL));
	resume(create(myTestProcC, 1024, 16, "myTestProcC_3", 0, NULL));
    */
	//sleepms(1000);      /* Lab2 4: "Main" waits for these "myTestProcC" processes		*/


    /*
     * User: wang4113
     * date: 09/20/2018
     */

    /* Lab2 5.5: Scenario 1: create 8 app processes that are all CPU-bound  */
    //kprintf("\nScenario 1: create 8 app processes that are all CPU-bound.\n");
    //kprintf("PID\tproctype\tclktimemilli (ms)\tgross CPU usage (ms)\taverage waiting time (ms)\n");
    //sleepms(5);    /* Lab2 5.5: Make sure "Main" process has the highest priority when creating apps		*/
    /*
    resume(create(appR3test, 1024, INITPRIO, "appR3test", 1, 0));
    resume(create(appR3test, 1024, INITPRIO, "appR3test", 1, 0));
    resume(create(appR3test, 1024, INITPRIO, "appR3test", 1, 0));
    resume(create(appR3test, 1024, INITPRIO, "appR3test", 1, 0));
    resume(create(appR3test, 1024, INITPRIO, "appR3test", 1, 0));
    resume(create(appR3test, 1024, INITPRIO, "appR3test", 1, 0));
    resume(create(appR3test, 1024, INITPRIO, "appR3test", 1, 0));
    resume(create(appR3test, 1024, INITPRIO, "appR3test", 1, 0));
    */
    //sleepms(20000);      /* Lab2 5.5: "Main" waits for these "appR3test" processes		*/


    /* Lab2 5.5: Scenario 2: create 8 app processes that are all I/O-bound  */
    //kprintf("\nScenario 2: create 8 app processes that are all I/O-bound.\n");
    //kprintf("PID\tproctype\tclktimemilli (ms)\tgross CPU usage (ms)\taverage waiting time (ms)\n");
    //sleepms(5);    /* Lab2 5.5: Make sure "Main" process has the highest priority when creating apps		*/
    /*
    resume(create(appR3test, 1024, INITPRIO, "appR3test", 1, 1));
    resume(create(appR3test, 1024, INITPRIO, "appR3test", 1, 1));
    resume(create(appR3test, 1024, INITPRIO, "appR3test", 1, 1));
    resume(create(appR3test, 1024, INITPRIO, "appR3test", 1, 1));
    resume(create(appR3test, 1024, INITPRIO, "appR3test", 1, 1));
    resume(create(appR3test, 1024, INITPRIO, "appR3test", 1, 1));
    resume(create(appR3test, 1024, INITPRIO, "appR3test", 1, 1));
    resume(create(appR3test, 1024, INITPRIO, "appR3test", 1, 1));
    */

    //sleepms(20000);      /* Lab2 5.5: "Main" waits for these "appR3test" processes		*/

    /* Lab2 5.5: Scenario 3: create 4 app processes that are CPU-bound and 4 app processes that are I/O-bound  */

    //kprintf("\nScenario 3: create 4 app processes that are CPU-bound and 4 app processes that are I/O-bound.\n");
    //kprintf("PID\tproctype\tclktimemilli (ms)\tgross CPU usage (ms)\taverage waiting time (ms)\n");
    //sleepms(5);    /* Lab2 5.5: Make sure "Main" process has the highest priority when creating apps		*/
    /*
    resume(create(appR3test, 1024, INITPRIO, "appR3test", 1, 0));
    resume(create(appR3test, 1024, INITPRIO, "appR3test", 1, 0));
    resume(create(appR3test, 1024, INITPRIO, "appR3test", 1, 0));
    resume(create(appR3test, 1024, INITPRIO, "appR3test", 1, 0));
    resume(create(appR3test, 1024, INITPRIO, "appR3test", 1, 1));
    resume(create(appR3test, 1024, INITPRIO, "appR3test", 1, 1));
    resume(create(appR3test, 1024, INITPRIO, "appR3test", 1, 1));
    resume(create(appR3test, 1024, INITPRIO, "appR3test", 1, 1));
    */

    //sleepms(20000);      /* Lab2 5.5: "Main" waits for these "appR3test" processes		*/

    /*
     * User: wang4113
     * date: 10/07/2018
     */

    /* Lab3 3.2: Scenario 1: create 8 app processes that are all CPU-bound  */
    //kprintf("\nScenario 1: create 8 app processes that are all CPU-bound.\n");
    //kprintf("PID\tproctype\tclktimemilli (ms)\tgross CPU usage (ms)\taverage waiting time (ms)\n");
    //sleepms(5);    /* Lab3 3.2: Consistent with Lab2 5.5		*/
    /*
    resume(create(appR3test, 1024, INITPRIO, "appR3test", 1, 0));
    resume(create(appR3test, 1024, INITPRIO, "appR3test", 1, 0));
    resume(create(appR3test, 1024, INITPRIO, "appR3test", 1, 0));
    resume(create(appR3test, 1024, INITPRIO, "appR3test", 1, 0));
    resume(create(appR3test, 1024, INITPRIO, "appR3test", 1, 0));
    resume(create(appR3test, 1024, INITPRIO, "appR3test", 1, 0));
    resume(create(appR3test, 1024, INITPRIO, "appR3test", 1, 0));
    resume(create(appR3test, 1024, INITPRIO, "appR3test", 1, 0));
    */
    //sleepms(20000);      /* Lab3 3.2: "Main" waits for these "appR3test" processes		*/

    /* Lab3 3.2: Scenario 2: create 8 app processes that are all I/O-bound  */
    //kprintf("\nScenario 2: create 8 app processes that are all I/O-bound.\n");
    //kprintf("PID\tproctype\tclktimemilli (ms)\tgross CPU usage (ms)\taverage waiting time (ms)\n");
    //sleepms(5);    /* Lab3 3.2: Consistent with Lab2 5.5		*/
    /*
    resume(create(appR3test, 1024, INITPRIO, "appR3test", 1, 1));
    resume(create(appR3test, 1024, INITPRIO, "appR3test", 1, 1));
    resume(create(appR3test, 1024, INITPRIO, "appR3test", 1, 1));
    resume(create(appR3test, 1024, INITPRIO, "appR3test", 1, 1));
    resume(create(appR3test, 1024, INITPRIO, "appR3test", 1, 1));
    resume(create(appR3test, 1024, INITPRIO, "appR3test", 1, 1));
    resume(create(appR3test, 1024, INITPRIO, "appR3test", 1, 1));
    resume(create(appR3test, 1024, INITPRIO, "appR3test", 1, 1));
    */
    //sleepms(20000);      /* Lab3 3.2: "Main" waits for these "appR3test" processes		*/

    /* Lab3 3.2: Scenario 3: create 4 app processes that are CPU-bound and 4 app processes that are I/O-bound  */
    //kprintf("\nScenario 3: create 4 app processes that are CPU-bound and 4 app processes that are I/O-bound.\n");
    //kprintf("PID\tproctype\tclktimemilli (ms)\tgross CPU usage (ms)\taverage waiting time (ms)\n");
    //sleepms(5);    /* Lab3 3.2: Consistent with Lab2 5.5		*/
    /*
    resume(create(appR3test, 1024, INITPRIO, "appR3test", 1, 0));
    resume(create(appR3test, 1024, INITPRIO, "appR3test", 1, 0));
    resume(create(appR3test, 1024, INITPRIO, "appR3test", 1, 0));
    resume(create(appR3test, 1024, INITPRIO, "appR3test", 1, 0));
    resume(create(appR3test, 1024, INITPRIO, "appR3test", 1, 1));
    resume(create(appR3test, 1024, INITPRIO, "appR3test", 1, 1));
    resume(create(appR3test, 1024, INITPRIO, "appR3test", 1, 1));
    resume(create(appR3test, 1024, INITPRIO, "appR3test", 1, 1));
    */
    //sleepms(20000);      /* Lab2 3.2: "Main" waits for these "appR3test" processes		*/

    /* Lab3 3.3: Dynamic workload: create 4 app processes that are CPU-bound and 4 app processes that are I/O-bound  */
    //kprintf("\nDynamic workload: create 4 app processes that are CPU-bound and 4 app processes that are I/O-bound.\n");
    //kprintf("PID\tproctype\tclktimemilli (ms)\tgross CPU usage (ms)\taverage waiting time (ms)\n");
    //sleepms(5);    /* Lab3 3.3: Consistent with Lab3 3.2		*/
    /*
    resume(create(appR3test, 1024, INITPRIO, "appR3test", 1, 0));
    sleepms(500);
    resume(create(appR3test, 1024, INITPRIO, "appR3test", 1, 0));
    sleepms(500);
    resume(create(appR3test, 1024, INITPRIO, "appR3test", 1, 0));
    sleepms(500);
    resume(create(appR3test, 1024, INITPRIO, "appR3test", 1, 0));
    sleepms(500);
    resume(create(appR3test, 1024, INITPRIO, "appR3test", 1, 1));
    sleepms(500);
    resume(create(appR3test, 1024, INITPRIO, "appR3test", 1, 1));
    sleepms(500);
    resume(create(appR3test, 1024, INITPRIO, "appR3test", 1, 1));
    sleepms(500);
    resume(create(appR3test, 1024, INITPRIO, "appR3test", 1, 1));
    sleepms(500);
    */
    //sleepms(20000);      /* Lab2 3.3: "Main" waits for these "appR3test" processes		*/


    /* Lab3 4: Test 4 real-time processes   */

    test_rms[0].rms_ct = 10;
    test_rms[0].rms_period = 50;

    test_rms[1].rms_ct = 20;
    test_rms[1].rms_period = 200;

    test_rms[2].rms_ct = 3;
    test_rms[2].rms_period = 30;

    test_rms[3].rms_ct = 7;
    test_rms[3].rms_period = 100;

    kprintf("PID\tx\ty\tperiod number\tclktimemilli\ty - (clktimemilli - period_start)\n");
    proctab[currpid].prprio = MAXPRIO;  /* Promote the priority of main process */

    resume(rms_create(rms_app, 1024, test_rms + 0, "rms_app", 2, test_rms[0].rms_ct, test_rms[0].rms_period));
    resume(rms_create(rms_app, 1024, test_rms + 1, "rms_app", 2, test_rms[1].rms_ct, test_rms[1].rms_period));
    resume(rms_create(rms_app, 1024, test_rms + 2, "rms_app", 2, test_rms[2].rms_ct, test_rms[2].rms_period));
    resume(rms_create(rms_app, 1024, test_rms + 3, "rms_app", 2, test_rms[3].rms_ct, test_rms[3].rms_period));

    /* Lab3 4.5: 4 additional CPU-bound processes  */
    /*
    resume(create(appR3test, 1024, INITPRIO, "appR3test", 1, 0));
    resume(create(appR3test, 1024, INITPRIO, "appR3test", 1, 0));
    resume(create(appR3test, 1024, INITPRIO, "appR3test", 1, 0));
    resume(create(appR3test, 1024, INITPRIO, "appR3test", 1, 0));
    */
    proctab[currpid].prprio = 20;       /* Demote the priority of main process  */
    sleepms(50000);                     /* main process wait    */



	/* Run the Xinu shell */

	recvclr();
	resume(create(shell, 8192, 50, "shell", 1, CONSOLE));

	/* Wait for shell to exit and recreate it */

	while (TRUE) {
		receive();
		sleepms(200);

		/* Lab1 4.1: Retrieve and display the uptime in minutes and seconds when recreating a shell	*/
		/*
		xuptime(&uptime);
		kprintf("\nThe uptime since XINU was bootstrapped:\n%d min, %d sec.\n", uptime.upmin, uptime.upsec);
		kprintf("\n\n");
		*/

		kprintf("\n\nMain process recreating shell\n\n");
		resume(create(shell, 4096, 20, "shell", 1, CONSOLE));
	}
	return OK;
    
}
