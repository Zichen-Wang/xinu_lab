/*
 * user: wang4113
 * data: 11/01/2018
 */

/* findfframe.c - findfframe */
#include <xinu.h>

/*-------------------------------------
 *  findfframe  -  Find a free frame
 *-------------------------------------
 */


int findfframe(uint8 type)
/* type == FRAME_DIRECTORY_TABLE means find a free frame for page directory or page table
 * type == FRAME_VIRTUAL_HEAP means find a free frame for virtual heap
 */
{
    uint32	i;			/* Iterate through all frames   */
    static	uint32 next_frame_pdt = 0;	/* Position in array [0, 999] for page directory of page table	*/
    static  uint32 next_frame_virt = 1000; /* Position in array [1000, 3071] for virtual memory	*/


    if (type == FRAME_DIRECTORY_TABLE) { /* find a frame for page directory or page table   */
        for (i = 0; i < FRAME_NUM_OF_PAGE_TABLE; i++) {

            if (next_frame_pdt == FRAME_NUM_OF_PAGE_TABLE) { /* Wrap around to beginning */
                next_frame_pdt = 0;
            }

            if (inverted_page_table[next_frame_pdt].fstate == F_FREE) {
                return next_frame_pdt++;
            }
            else {
                next_frame_pdt++;
            }
        }

        return SYSERR;
    }

}