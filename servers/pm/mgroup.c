#include "stdio.h"
#include "pm.h"
#include <sys/wait.h>
#include <assert.h>
#include <minix/callnr.h>
#include <minix/com.h>
#include <minix/sched.h>
#include <minix/vm.h>
#include <sys/ptrace.h>
#include <sys/resource.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>   
#include "mproc.h"
#include "param.h"
#include "mgroup.h"

/* message queue(shared to all groups, because we need detect deadlock) */
/* message queue store proc queues */
static mqueue *msg_queue = NULL;        

static mgroup mgrp[NR_GRPS];            /* group table [this design is similar to proc design in minix] */
static int g_nr_ptr = 0;                /* group number ptr */
static int g_id_ctr = 1;                /* group id counter */
static message *k_msg;                  /* kernel level message */
static mgroup *cur_group;               /* current message group*/

/* private methods prototype */
int invalid(int strategy);                                  /* valid strategy */ 
int deadlock(mgroup *g_ptr, int call_nr);                   /* valid deadlock */ 
int getgroup(int grp_nr, mgroup ** g_ptr);                  /* get group by its gid */
int getprocindex(mgroup *g_ptr, int proc);                  /* get proc index in group*/
endpoint_t getendpoint(int proc_id);                        /* get endpoint from proc list*/
void unblock(endpoint_t proc_e, message *msg);              /* unblock a process */
int searchinproc(mqueue *proc_q, grp_message *g_m);         /* search send->rec chain from proc */
void deadlock_rec(mqueue *proc_q, mqueue *src_q, mqueue *dest_q, int call_nr);  /*recursive detect deadlock */

int do_opengroup()
{
    mgroup *g_ptr = NULL;
    int i, strategy;
    
    strategy = m_in.m1_i1;
    if(invalid(strategy)){                         // Make sure strategy is valid. 0 is allowed
        return EIVSTTG;                             // Invalid strategy. which defined in sys/errno.h
    }
    
    if(msg_queue == NULL){                          // Init message queue if it is null.
        initqueue(&msg_queue);
    }
    
    for(i=0; i<NR_GRPS; i++, g_nr_ptr++){
        g_nr_ptr %= NR_GRPS;                        // Circle detect.
        if(mgrp[g_nr_ptr].g_stat == M_UNUSED){      // This group is not used until now.
            g_ptr = mgrp + g_nr_ptr;
            break;
        }
    }
    
    if(g_ptr == NULL){                              // No avalible(free) group in PM server.
        return EGRPBUSY;                            // Resource busy
    }
    g_ptr->g_stat = M_READY;
    g_ptr->g_nr = g_id_ctr;
    g_ptr->g_sttg = strategy;
    g_ptr->p_size = 0;
    initqueue(&g_ptr->valid_q);
    initqueue(&g_ptr->pending_q);
    initqueue(&g_ptr->invalid_q_int);
    g_id_ctr++;
    
    return g_ptr->g_nr;
}

int do_addproc(){
    mgroup *g_ptr = NULL;
    int grp_nr, proc_id;	
    endpoint_t proc_ep;
    grp_nr = m_in.m1_i1;
    proc_id = m_in.m1_i2;
    if(getgroup(grp_nr, &g_ptr) == -1){
        return EIVGRP;
    }else if(g_ptr->p_size >= NR_MGPROCS){
        return EPROCLEN;                    // reach max length
    }else if((proc_ep=getendpoint(proc_id))<0){
        return EIVPROC;
    }else if(getprocindex(g_ptr, proc_id) != -1){
        return EPROCEXIST;                  // proc already exist
    }
    *(g_ptr->p_lst+g_ptr->p_size) = proc_id;
    g_ptr->p_size++;
    return 0;
}

int do_rmproc(){
    mgroup *g_ptr = NULL;
    int i, grp_nr, proc_id;
    endpoint_t proc_ep;
    
    grp_nr = m_in.m1_i1;
    proc_id = m_in.m1_i2;
    if(getgroup(grp_nr, &g_ptr) == -1){
        return EIVGRP;
    } else if((proc_ep=getendpoint(proc_id))<0){
        return EIVPROC;
    } else if((i=getprocindex(g_ptr, proc_id)) == -1){
        return EIVPROC;                     // cant find proc in group
    } 
    
    for(; i<g_ptr->p_size-1;i++){
        *(g_ptr->p_lst+i) = *(g_ptr->p_lst+i+1);
    }
    g_ptr->p_size--;
    return 0;
} 

int do_closegroup(){
    mgroup *g_ptr = NULL;
    int grp_nr;
    
    grp_nr = m_in.m1_i1;
    if(getgroup(grp_nr, &g_ptr) == -1){
        return EIVGRP;
    }
    g_ptr->g_stat = M_UNUSED;
    g_ptr->g_nr = 0;
    g_ptr->g_sttg = 0;
    g_ptr->p_size = 0;
    
    return 0;
}

int do_recovergroup(){
    mgroup *g_ptr = NULL;
    int grp_nr, strategy;
    
    grp_nr = m_in.m1_i1;
    strategy = m_in.m1_i2;
    if(invalid(strategy)){                           // Make sure strategy is valid. 0 is allowed
        return EIVSTTG;
    }else if(getgroup(grp_nr, &g_ptr) == -1){
        return EIVGRP;
    }
    
    return 0;
}

int do_msend(){
    int rv=SUSPEND, src, grp_nr, ipc_type;
    message *msg;
    mgroup *g_ptr = NULL;
    grp_message *g_m;
    
    src = m_in.m1_i1;
    grp_nr = m_in.m1_i2;
    ipc_type = m_in.m1_i3;
    msg = (message*) malloc(sizeof(message));
    if(getgroup(grp_nr, &g_ptr) == -1){
        return EIVGRP;
    } else if(getprocindex(g_ptr, src) == -1){
        return -2;
    }
    if ((message *) m_in.m1_p1 != (message *) NULL) {
        rv = sys_datacopy(who_e, (vir_bytes) m_in.m1_p1,
            PM_PROC_NR, (vir_bytes) msg, (phys_bytes) sizeof(message));
        if (rv != OK) return(rv);
    }
    printf("now msend %d-%d-%d\n", src, ipc_type, msg->m1_i1);    
    // add a new message.
    cur_group = g_ptr;
    g_m = (grp_message *)malloc(sizeof(grp_message));
    g_m->group=g_ptr;
    g_m->sender=getendpoint(src);
    g_m->receiver=getendpoint(ipc_type);
    g_m->call_nr=SEND;
    g_m->msg= msg;
    g_ptr->pending_q->enqueue(g_m, g_ptr->pending_q);
    printf("msend finish\n");    
    return rv==0 ? SUSPEND : rv;
}

int do_mreceive(){
    int rv=SUSPEND, src, grp_nr, ipc_type;
    mgroup *g_ptr = NULL;
    grp_message *g_m;
    
    src = m_in.m1_i1;
    grp_nr = m_in.m1_i2;
    ipc_type = m_in.m1_i3;
    
    if(getgroup(grp_nr, &g_ptr) == -1){
        return EIVGRP;
    } else if(getprocindex(g_ptr, src) == -1){
        return -2;
    }
//    if ((message *) m_in.m1_p1 != (message *) NULL) {
//        rv = sys_datacopy(PM_PROC_NR,(vir_bytes) msg,
//            who_e, (vir_bytes) m_in.m1_p1, (phys_bytes) sizeof(m));
//        if (rv != OK) return(rv);
//    }
    
    printf("Now mreceive %d-%d\n", src, ipc_type);
    cur_group = g_ptr;
    g_m = (grp_message *)malloc(sizeof(grp_message));
    g_m->group=g_ptr;
    g_m->receiver=getendpoint(src);
    g_m->sender=getendpoint(ipc_type);
    g_m->call_nr=RECEIVE;
    g_m->msg= NULL;                                             //Receiver do not need store message.
    g_ptr->pending_q->enqueue(g_m, g_ptr->pending_q);
    printf("m receive finish\n");
    return rv==0 ? SUSPEND : rv;
}

/*
 * Check message queue, when find match grp_message, send reply to its src & dest, then unblock both of them.
 */
void do_server_ipc(){
    int rv=0, flag;
    mqueue *proc_q;
    void *value;
    grp_message *g_m;
    
    // Only check current group
    printf("server ipc start\n");
    while(cur_group->pending_q->dequeue(&value, cur_group->pending_q)){
         g_m = (grp_message *)value;
         msg_queue->iterator(msg_queue);
         flag = 0;          
         printf("cur %d-%d\n", g_m->sender, g_m->receiver);
         while(msg_queue->next(&value, msg_queue)){
            proc_q = (mqueue *)value;
             /* find match proc*/
            if(searchinproc(proc_q, g_m) > 0) {
                printf("find %d-%d\n", g_m->sender, g_m->receiver);
                flag = 1;
                break;
            }
         }
         
         /* if not find match proc */
         if(flag == 0){
             // create a new proc in queue, and enqueue its first item.
             printf("not find %d-%d\n", g_m->sender, g_m->receiver);
             initqueue(&proc_q);
             proc_q->number = g_m->sender;
             proc_q->enqueue(g_m, proc_q);
             msg_queue->enqueue(proc_q, msg_queue);
         }
    }
    printf("server ipc finish %d\n", rv);
}

/*  ========================= private methods ================================*/

int getprocindex(mgroup *g_ptr, int proc){
    int i;
    
    for(i=0; i<g_ptr->p_size;i++){
        if(*(g_ptr->p_lst+i) == proc){
            return i;
        }
    }
    return -1;
}

int getgroup(int grp_nr, mgroup ** g_ptr){
    int i, k;
    
    if(grp_nr < 0 || grp_nr>=NR_GRPS){
        return -1;
    }
    
    for(i=0, k=g_nr_ptr; i<NR_GRPS; i++, k--){
        k=(k+NR_GRPS)%NR_GRPS;
        if(mgrp[k].g_stat != M_UNUSED && mgrp[k].g_nr == grp_nr){       // find the group in group table.
            (*g_ptr) = &mgrp[k];
            return 0;
        }
    }
    return -1;
}

int invalid(int strategy){
    return 0;
}

endpoint_t getendpoint(int proc_id){
    register struct mproc *rmp;
    if(proc_id < 0){
        return -1;
    }
    for (rmp = &mproc[NR_PROCS-1]; rmp >= &mproc[0]; rmp--){ 
        if (!(rmp->mp_flags & IN_USE)) continue;
        if (proc_id > 0 && proc_id == rmp->mp_pid) return rmp->mp_endpoint;
    }
    return -1;
}

void unblock(endpoint_t proc_e, message *msg){
    register struct mproc *rmp;
    int s, proc_nr;
    
    for (rmp = &mproc[NR_PROCS-1]; rmp >= &mproc[0]; rmp--){ 
        if (!(rmp->mp_flags & IN_USE)) continue;
        if(proc_e == rmp->mp_endpoint){
            if (pm_isokendpt(rmp->mp_endpoint, &proc_nr) != OK) {
                panic("handle_vfs_reply: got bad endpoint from VFS: %d", rmp->mp_endpoint);
            }
            setreply(proc_nr, OK);
            if ((rmp->mp_flags & (REPLY | IN_USE | EXITING)) == (REPLY | IN_USE)) {
              memcpy(&rmp->mp_reply, msg, sizeof(message));
              printf("ub_send %d-%d\n", msg->m1_i1, rmp->mp_reply.m1_i1);
              s=sendnb(rmp->mp_endpoint, &rmp->mp_reply);
              if (s != OK) {
                  printf("PM can't reply to %d (%s): %d\n",
                      rmp->mp_endpoint, rmp->mp_name, s);
              }
              rmp->mp_flags &= ~REPLY;
            }
        }
    }
}

int deadlock(mgroup *g_ptr, int call_nr){
    grp_message *g_m;
    mqueue *proc_q, *src_q, *dest_q, *invalid_q;
    void *value;
    
    initqueue(&src_q);
    initqueue(&dest_q);
    while(g_ptr->pending_q->dequeue(&value, g_ptr->pending_q)){
        g_m = (grp_message *)value;
        src_q->enqueue((void *)g_m->sender, src_q);
        dest_q->enqueue((void *)g_m->receiver, dest_q);
        g_ptr->valid_q->enqueue(g_m, g_ptr->valid_q);
    }
    msg_queue->iterator(msg_queue);
    if(msg_queue->next(&value, msg_queue)){
        proc_q = (mqueue *)value;
        deadlock_rec(proc_q, src_q, dest_q, call_nr);
    }
    g_ptr->valid_q->iterator(g_ptr->valid_q);
    
    closequeue(src_q);
    closequeue(dest_q);
    return 0;
}

/*
 * Recursive detect deadlock.
 */
void deadlock_rec(mqueue *proc_q, mqueue *src_q, mqueue *dest_q, int call_nr){
    grp_message *msg_m;
    endpoint_t dest_e;
    void *value;
    
    // Put all receiver into dest_q from current proc.
    proc_q->iterator(proc_q);
    while(proc_q->next(&value, proc_q)){
        msg_m = (grp_message *)value;
        if(msg_m->call_nr != call_nr) continue;
        dest_q->enqueue((void *)msg_m->receiver, dest_q);
    }
    
    // iterative get nextproc.
    dest_q->iterator(dest_q);
    while(dest_q->dequeue(&value, dest_q)){
        dest_e = (int) value;
        if(src_q->hasvalue((void *)dest_e, src_q)){
            cur_group->g_stat = M_DEADLOCK;                                         //Deadlock
            cur_group->flag = ELOCKED;                                              //Deadlock
            cur_group->invalid_q_int->enqueue((void *)dest_e, cur_group->invalid_q_int);    //Deadlock queue
        } else {
            src_q->enqueue((void *)dest_e, src_q);
        }
        msg_queue->iterator(msg_queue);
        while(msg_queue->next(&value, msg_queue)){
            proc_q = (mqueue *)value;
            if(proc_q->number == dest_e){
                deadlock_rec(proc_q, src_q, dest_q, call_nr);       //Recursive.
            }
            break;
        }
    }
}

/*
 * search msg match from queues.
 */
int searchinproc(mqueue *proc_q, grp_message *g_m){
    grp_message *msg_m;
    message *msg;
    void *value;
    
    if(g_m->sender == proc_q->number){                   //Only check/store sender. do not need check twice: sender and receiver
        proc_q->iterator(proc_q);
        
        while(proc_q->next(&value, proc_q)){
            msg_m=(grp_message *)value;
            if(msg_m->call_nr == g_m->call_nr) continue;           // Only search send->receive
             // If sender and receiver match: sender = sender, callnr = SEND+RECEIVE, receiver = receiver
            if(msg_m->receiver == g_m->receiver){
                msg = msg_m->call_nr == SEND ? msg_m->msg : g_m->msg;
                
                printf("unblock %d-%d-%d\n", msg_m->receiver, msg_m->sender, msg->m1_i1);
                unblock(msg_m->receiver, msg);
                unblock(msg_m->sender, msg);
                
                proc_q->removeitem(proc_q);              //Remove current message from proc_queue(not proc)
                free(msg_m);
                free(g_m);
                return 2;
            }
        }
        proc_q->enqueue(g_m, proc_q);                   //If not match, then enqueue this message.
        return 1;
    }
    return 0;
}
