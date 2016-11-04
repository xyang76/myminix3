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

/* ipc_type */
#define SENDALL        0
#define RECANY         0
#define IPCNONBLOCK   -1
#define RECSENDB      -2
#define SENDRECB      -3
#define ANYSOURCE     -4 


/* group block stategies */
#define UB_ALL_REC    0             /* unblock when all receiver get this message */
#define UB_ANY_REC    1             /* unblock when any receiver get this message */
#define UB_ANY_CLEAR  2             /* when any receiver get this msg, then this message will be clear from message queue */



typedef struct{
    grp_nr_t g_nr;                  /* group number ptr */
    strategy g_sttg;                /* group strategy */
    int p_lst[NR_MGPROCS];          /* group processes */
    mqueue *valid_q;                /* valid message queue */
    mqueue *pending_q;              /* pending message queue */
    mqueue *invalid_q;              /* invalid message queue: may deadlock */
    grp_stat g_stat;                /* group state */
    int p_size;                     /* process size */
    int flag;                       /* flag */
}mgroup;

typedef struct{
    mgroup *group;                  /* group */
    endpoint_t sender;              /* sender */
    endpoint_t receiver;            /* receiver */
    int call_nr;                    /* type */                      
    message *msg;                   /* message */
}grp_message;