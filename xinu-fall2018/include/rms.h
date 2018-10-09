/*
* User: wang4113
* date: 10/08/2018
*/

/* rms.h - define struct rmsparam */

typedef struct rmsparam
{
    uint16 rms_ct;      /* the computation time requirement of the real-time process to be created */
    uint16 rms_period;  /* the period of the real-time process  */
} rmsparam_t;

#define RMSBOUND    0.5     /* a conservative upper bound of RMS processes  */
#define RMSQUANTUM  1000    /* time slice of all RMS processes  */
