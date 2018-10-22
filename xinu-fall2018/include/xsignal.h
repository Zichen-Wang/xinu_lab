/*
 * User: wang4113
 * date: 10/17/2018
*/

/* rms.h - specify signal numbers  */

#define SIGNUM  3

#define SIGRECV 0
#define SIGXCPU 1
#define SIGTIME 2

struct xsigtab {
    bool8 regyes;
    int (* fnt)();
    uint32 optarg;
};
