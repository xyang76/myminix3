#include "kernel/system.h"
#include <minix/endpoint.h>
#include "stdio.h"

/*===========================================================================*
  *                                do_ipc_error_detect                        *
  *===========================================================================*/
 extern int mini_send(
  register struct proc *caller_ptr,	/* who is trying to send a message? */
  endpoint_t dst_e,			/* to whom is message being sent? */
  message *m_ptr,			/* pointer to message buffer */
  const int flags
);

extern static int dbug;
  
int do_ipcerrdtct(struct proc *caller_ptr, message *m_ptr)
{
    dbug = 1;
//    message *msg;
//	int rv, dest, *proclist;
//    
//    dest = m_ptr.m1_i1;
//    printf("Yes, in ipc %d\n", dest);
//    msg = (message*)m_ptr.m1_p1;
//    proclist = (int*)m_ptr.m1_p2;
//    rv = mini_send(caller_ptr, dest, msg, 0);
//    printf("Yes, ipc end %d\n", rv);
    return(OK);
}