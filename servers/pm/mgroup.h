#include "mqueue.h"

#define NR_GRPS      256        /* max number of groups */
#define NR_MGPROCS   NR_PROCS   /* max number of processes in one group */

typedef int grp_nr_t;           /* group number ptr */
typedef int strategy;           /* send/receive/recovery strategy */
typedef int grp_stat;           /* group state */

/* msg group state */
#define M_UNUSED       0
#define M_READY        1  
#define M_SENDING      2      
#define M_RECEIVING    3 
#define M_DEADLOCK     4
#define M_ERROR        5 

/* send_type */
#define IPCALL         0
#define IPCSENDBLOCK   -1
#define IPCRECBLOCK    -2
#define IPCNONBLOCK    -3 
#define IPCANY         -4

/* group stategies */


typedef struct{
    int grp_nr;                     /* group */
    endpoint_t src;                 /* sender */
    endpoint_t dest;                /* receiver */
    int call_nr;                    /* type */                      
    message *msg;                   /* message */
}grp_message;

typedef struct{
    grp_nr_t g_nr;                  /* group number ptr */
    strategy g_sttg;                /* group strategy */
    int p_lst[NR_MGPROCS];          /* group processes */
    grp_stat g_stat;                /* group state */
    int p_size;                     /* process size */
    int flag;                       /* flag */
}mgroup;