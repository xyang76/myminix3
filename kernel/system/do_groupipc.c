#include "kernel/system.h"
#include <minix/endpoint.h>
#include "stdio.h"

/*===========================================================================*
  *                                do_group_ipc                             *
  *===========================================================================*/
extern static int deadlock(int function, register struct proc *caller,
	endpoint_t src_dst_e);
    
extern static int do_sync_ipc(struct proc * caller_ptr, /* who made the call */
			int call_nr,	/* system call number and flags */
			endpoint_t src_dst_e,	/* src or dst of the call */
			message *m_ptr);	/* users pointer to a message */

EXTERN struct proc proc[NR_TASKS + NR_PROCS];
  
int do_ipcerrdtct(struct proc *caller_ptr, message *m_ptr)
{
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

int do_singleipc(struct proc *caller_ptr, message *m_ptr)
{
    message *msg;
    int caller_e, src_dest_e, call_nr;
    
    struct proc *const caller_ptr = get_cpulocal_var(proc_ptr);
    caller_e = m_ptr.m1_i1 ;
    src_dest_e = m_ptr.m1_i2;
    call_nr = m_ptr.m1_i3;
    
//    for()
    printf("caller_ptr %d-%d\n", caller_ptr->p_nr, caller_ptr->p_endpoint);
    printf("caller_e %d-%d-%d\n", nr_to_id(caller_e), caller_e, id_to_nr(caller_e));
//    do_sync_ipc()
    return(OK);
}