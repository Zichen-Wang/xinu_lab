/*
 * user: wang4113
 * data: 11/01/2018
 */

/* page_data_structure.h */

#define PAGE_FAULT_NUM  14

#define F_FREE          0
#define F_USED_VIRT     1
#define F_PD            2
#define F_PT            3
#define F_SHARED_PT     4

struct frame_t {

    uint16  fstate;      /* Define the status of a frame */
    pid32   pid;        /* Which process is using this frame?    */

    unsigned int virt_page_num	: 20;   /* the virtual address of this frame    */
    int reference_count;

};

#define DEVICE_FRAME_BASE   589824      /* base frame for device memory    */

#define NFRAMES_FOR_PAGE_TABLE     1000    /* number of frames for page directory or page table */
#define NFRAMES_FOR_VIRTUAL_HEAP   2072    /* number of frames for virtual heap    */

#define PAGE_DIRECTORY_ENTRIES 1024    /* number of entries for page directory */
#define PAGE_TABLE_ENTRIES     1024    /* number of entries for page table */

#define PAGE_DIRECTORY_TABLE   0           /* Frame type used in findfframe()  */
#define PAGE_VIRTUAL_HEAP      1           /* Frame type used in findfframe()  */

/* in file paging_init.c    */
extern  struct frame_t inverted_page_table[NFRAMES];
extern  char * shared_page_table[5];
extern  int pferrorno;

/* in paging_init.c     */
extern  void paging_init(void);

/* in file create_shared_pt.c	*/
extern	void create_shared_pt(void);

/* in file create_pd.c	*/
extern	char * create_pd(pid32);

/* in file create_pt.c	*/
extern	char * create_pt(pid32);

/* in file free_frames.c	*/
extern	void free_frames(pid32);

/* in file findfframe.c */
extern  int findfframe(uint8);

/* in file vcreate.c    */
extern  pid32	vcreate(void *, uint32, uint16, pri16, char, uint32, ...);

/* in file hooks.c  */
extern  void hook_ptable_create(unsigned int);
extern  void hook_ptable_delete(unsigned int);
extern  void hook_pfault(char *);
extern  void hook_pswap_out(unsigned int, int);
extern  int get_faults(void);

/* in file enable_paging.S  */
extern  void enable_paging(void);

/* in file setCR3.S */
extern  void setCR3(uint32);

/* in file pfisr.S  */
extern  void pfisr(void);

/* in file pfhandler.c  */
extern  void pfhandler(void);
