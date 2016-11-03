#include "kernel/system.h"
#include <minix/endpoint.h>
#include "stdio.h"

/*===========================================================================*
  *                                do_group_ipc                             *
  *===========================================================================*/
/*extern int deadlock(int function, register struct proc *caller,
	endpoint_t src_dst_e);*/
    
extern int do_sync_ipc2(struct proc * caller_ptr, 
			int call_nr,	
			endpoint_t src_dst_e,	
			message *m_ptr);	

EXTERN struct proc proc[NR_TASKS + NR_PROCS];
  
int do_ipcerrdtct(struct proc *caller_ptr, message *m_ptr)
{
    int caller_e;
    struct proc *call_p=NULL;
    caller_e = m_ptr->m1_i1;
    
    for (call_p = &proc[0]; call_p < &proc[NR_TASKS + NR_PROCS]; call_p++){
        if(call_p->p_endpoint == caller_e){
            return call_p->p_nr;
        }
    } 
    return(OK);
}

int do_singleipc(struct proc *caller_ptr, message *m_ptr)
{
    message msg;
    struct proc *call_p=NULL;
    int caller_e, src_dest_e, call_nr, r;
    
    caller_e = m_ptr->m1_i1 ;
    src_dest_e = m_ptr->m1_i2;
    call_nr = m_ptr->m1_i3;
    
    /* copy message from caller*/
//    if((r=data_copy_vmcheck(caller_ptr, caller_ptr->p_endpoint,
//		(vir_bytes) m_ptr->m1_p1, KERNEL, (vir_bytes) &msg,
//		(phys_bytes) sizeof(struct msg))) != OK)
//        return r;
//    
    printf("kernel caller_ptr2 %d-%d-%d\n", caller_ptr->p_nr, caller_ptr->p_endpoint,nr_to_id(caller_ptr->p_nr));
//    printf("msg=%d\n", msg.m1_i1);
    if(call_nr == 98){
        call_nr = SEND;
        printf("caller_e %d, src-dest %d\n", caller_e, src_dest_e);
    } else if (call_nr == 99){
        call_nr = RECEIVE;
        printf("caller_e %d, src-dest %d\n", caller_e, src_dest_e);
    } else {    
        for (call_p = &proc[0]; call_p < &proc[NR_TASKS + NR_PROCS]; call_p++){
            if(call_p->p_endpoint == caller_e){
                printf("start ipc send\n");
                return do_sync_ipc2(call_p, call_nr, src_dest_e, m_ptr);
            }
        } 
    }
    return(OK);
}