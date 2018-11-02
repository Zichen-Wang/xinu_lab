/*
 * user: wang4113
 * data: 11/01/2018
 */

/* page_data_structure.h */

#define VIRTUAL_HEAP_BASE   2024        /* base frames for virtual heap	memory */
#define PAGE_TABLE_BASE     1024        /* base frames for page directory or page table memory */
#define DEVICE_FRAME_BASE   589824      /* base frames for device memory    */

#define F_FREE          0
#define F_USED_VIRT     1
#define F_PD            2
#define F_PT            3
#define F_PT_SHARED     4

typedef struct {

    uint16  fstate;      /* Define the status of a frame */
    pid32   pid;        /* Which process is using this frame?    */

    unsigned int virt_addr	: 20;   /* the virtual address of this frame    */

} frame_t;


#define FRAME_NUM_OF_PAGE_TABLE     1000    /* number of frames for page directory or page table */
#define FRAME_NUM_OF_VIRTUAL_HEAP   2072    /* number of frames for virtual heap    */

#defien PAGE_DIRECTORY_ENTRIES 1024    /* number of entries for page directory */
#define PAGE_TABLE_ENTRIES     1024    /* number of entries for page table */

#define FRAME_DIRECTORY_TABLE   0           /* Frame type used in findfframe()  */
#define FRAME_VIRTUAL_HEAP      1           /* Frame type used in findfframe()  */


/* in paging_init.c     */
extern  void paging_init(void);

/* in file create_pd.c	*/
extern	char * create_pd(pid32);

/* in file create_pt.c	*/
extern	char * create_pt(pid32);

/* in file create_shared_pt.c	*/
extern	void create_shared_pt(void);

/* in file findfframe.c */
extern  char * findfframe();