/* welcome.c - welcome */

#include <xinu.h>

/*-------------------------------------------------------------------------
 *  welcome  -  Display the welcome message and my full name and login name
 *-------------------------------------------------------------------------
 */
void   welcome() {

    kprintf("\nHello World!\n");
    kprintf("\nMy full name is Zichen Wang, and my login name is wang4113.\n");
    kprintf("\nYou can do something else, or do nothing; it's completely up to you.\n");
    kprintf("\n...creating a shell\n");

}
