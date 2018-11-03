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
/*
 * type == PAGE_DIRECTORY_TABLE means find a free frame for page directory or page table
 * type == PAGE_VIRTUAL_HEAP means find a free frame for virtual heap
 */
{
    uint32	i;			/* Iterate through all frames   */
    /* Position in array [0, NFRAMES_FOR_PAGE_TABLE - 1] for page directory of page table	*/
    static	uint32 next_frame_pdt = 0;

    /* Position in array [NFRAMES_FOR_PAGE_TABLE, NFRAMES - 1] for virtual memory	*/
    static  uint32 next_frame_virt = NFRAMES_FOR_PAGE_TABLE;


    if (type == PAGE_DIRECTORY_TABLE) { /* find a frame for page directory or page table   */
        for (i = 0; i < NFRAMES_FOR_PAGE_TABLE; i++) {

            if (next_frame_pdt == NFRAMES_FOR_PAGE_TABLE) { /* Wrap around to beginning */
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

    else if (type == PAGE_VIRTUAL_HEAP) { /* find a frame for for virtual heap   */

        for (i = 0; i < NFRAMES_FOR_VIRTUAL_HEAP; i++) {

            if (next_frame_virt == NFRAMES) { /* Wrap around to beginning */
                next_frame_virt = NFRAMES_FOR_PAGE_TABLE;
            }

            if (inverted_page_table[next_frame_virt].fstate == F_FREE) {
                return next_frame_virt++;
            }
            else {
                next_frame_virt++;
            }
        }

        return SYSERR;
    }

    return SYSERR;

}