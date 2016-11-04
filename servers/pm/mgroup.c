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
static mgroup *cur_group;               /* current message group*/
static int lock = 0;                    /* simple busy lock, may cause bug. Better solution: 1.kernel call & spinlock 2. semaphore
                                         *  but i dont have enough time to finish this.    */

/* private methods prototype */
int invalid(int strategy);                                  /* valid strategy */ 
int deadlock(mgroup *g_ptr, int call_nr);                   /* valid deadlock */ 
int getgroup(int grp_nr, mgroup ** g_ptr);                  /* get group by its gid */
int getprocindex(mgroup *g_ptr, int proc);                  /* get proc index in group*/
endpoint_t getendpoint(int proc_id);                        /* get endpoint from proc list*/
void unblock(endpoint_t proc_e, message *msg);              /* unblock a process */
int searchinproc(mqueue *proc_q, grp_message *g_m);         /* search send->rec chain from proc */
void deadlock_rec(mqueue *proc_q, mqueue *src_q, mqueue *dest_q, int call_nr);  /*recursive detect deadlock */
int acquire_lock(void);                                     /* simple busy lock. better solution: kernel call & spinlock / semaphore*/
int release_lock(void);                                     /* simple busy lock */
int getprocqueue(endpoint_t proc_e, mqueue **proc_q);       /* get proc queue */

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
    // add a new message.
    cur_group = g_ptr;
    g_ptr->g_stat == M_SENDING;
    
    g_m = (grp_message *)malloc(sizeof(grp_message));
    g_m->group=g_ptr;
    g_m->sender=getendpoint(src);
    g_m->receiver=getendpoint(ipc_type);
    g_m->call_nr=SEND;
    g_m->msg= msg;
    queue_func->enqueue(g_m, g_ptr->pending_q);
    
    rv = deadlock(g_ptr, SEND);                                     // detect deadlock
    if (rv == 0){
        g_ptr->g_stat == M_READY;
        return SUSPEND;
    }
    return rv;
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
    
    cur_group = g_ptr;
    g_ptr->g_stat == M_RECEIVING;
    
    g_m = (grp_message *)malloc(sizeof(grp_message));
    g_m->group=g_ptr;
    g_m->receiver=getendpoint(src);
    g_m->sender=getendpoint(ipc_type);
    g_m->call_nr=RECEIVE;
    g_m->msg = NULL;                                             //Receiver do not need store message.
    queue_func->enqueue(g_m, g_ptr->pending_q);
    
    rv = deadlock(g_ptr, RECEIVE);                              // detect deadlock
    if (rv == 0){
        g_ptr->g_stat == M_READY;
        return SUSPEND;
    }
    return rv;
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
    if(cur_group->g_stat != M_READY) return;
    
    while(queue_func->dequeue(&value, cur_group->valid_q)){
         g_m = (grp_message *)value;
         queue_func->iterator(msg_queue);
         flag = 0;          
         while(queue_func->next(&value, msg_queue)){
            proc_q = (mqueue *)value;
             /* find match proc*/
            if(searchinproc(proc_q, g_m) > 0) {
                flag = 1;
                break;
            }
         }
         
         /* if not find match proc */
         if(flag == 0){
             // create a new proc in queue, and enqueue its first item.
             initqueue(&proc_q);
             proc_q->number = g_m->sender;
             if(g_m->call_nr == 1){
                printf("here we enqueue %d->%d\n", g_m->sender, g_m->receiver);
             }
             queue_func->enqueue(g_m, proc_q);
             queue_func->enqueue(proc_q, msg_queue);
         }
    }
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
    int rv = 0, sender;
    
    initqueue(&src_q);
    initqueue(&dest_q);
    // add all pending processes into valid_q
    while(queue_func->dequeue(&value, g_ptr->pending_q)){
        g_m = (grp_message *)value;
        if(!queue_func->hasvalue((void *)g_m->sender, src_q)){
            queue_func->enqueue((void *)g_m->sender, src_q);            // Only store once
            sender = g_m->sender;
        }
        queue_func->enqueue((void *)g_m->receiver, dest_q);            // Only store once
        queue_func->enqueue(g_m, g_ptr->valid_q);
    }
    
    // detect deadlock
    if(getprocqueue(sender, &proc_q) != -1){
        deadlock_rec(proc_q, src_q, dest_q, call_nr);
    }
    
//    printf("d3 %d, %d  ", cur_group->valid_q->size, cur_group->invalid_q_int->size);
//    printqueue(src_q, "src_q_dm3");
    
    // Remove deadlock processes from valid_q
    queue_func->iterator(g_ptr->valid_q);
    while(queue_func->next(&value, g_ptr->valid_q)){
        g_m = (grp_message *)value;
        if(queue_func->hasvalue((void*)g_m->receiver, g_ptr->invalid_q_int)){
            queue_func->removeitem(g_ptr->valid_q);
            rv = ELOCKED;
        }
    }
//    printf("d4 va%d, iv%d, src%d,dest %d, rv %d  ", cur_group->valid_q->size, cur_group->invalid_q_int->size, src_q->size,dest_q->size, rv);
    closequeue(src_q);
    closequeue(dest_q);
    return rv;
}

/*
 * Recursive detect deadlock.
 */
void deadlock_rec(mqueue *proc_q, mqueue *src_q, mqueue *dest_q, int call_nr){
    grp_message *msg_m, *m_test;
    endpoint_t dest_e;
    void *value;
    
    // Put all receiver into dest_q from current proc.
    queue_func->iterator(proc_q);
    while(queue_func->next(&value, proc_q)){
        msg_m = (grp_message *)value;
        if(call_nr == 1){
            printf("v : %d->%d [%d] %d\n", msg_m->sender, msg_m->receiver, msg_m->call_nr, proc_q->size);
        }
        if(msg_m->call_nr != call_nr) continue;
        queue_func->enqueue((void *)msg_m->receiver, dest_q);
        printf("enqueue %d: %d->%d [%d] :: ", proc_q->number, msg_m->sender, msg_m->receiver, msg_m->call_nr);
    }
//    printqueue(src_q, "src_q");
    printqueue(dest_q, "dest_q");
    
    // iterative get nextproc.
    queue_func->iterator(dest_q);
    if(queue_func->dequeue(&value, dest_q)){
        dest_e = (int) value;
        if(queue_func->hasvalue((void *)dest_e, src_q)){
            printf("deadlock:%d - ", dest_e);
            printqueue(src_q, "src_q_deadlock");
            cur_group->g_stat = M_DEADLOCK;                                          //Deadlock
            queue_func->enqueue((void *)dest_e, cur_group->invalid_q_int);           //Deadlock queue
        } else {
            queue_func->enqueue((void *)dest_e, src_q);
        }
        
        if(getprocqueue(dest_e, &proc_q) != -1 && proc_q->size > 0){
            deadlock_rec(proc_q, src_q, dest_q, call_nr);  
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
    
    if(g_m->sender == proc_q->number){                              //Only check/store sender. do not need check twice: sender and receiver
        queue_func->iterator(proc_q);
        
        while(queue_func->next(&value, proc_q)){
            msg_m=(grp_message *)value;
            if(msg_m->call_nr == g_m->call_nr) continue;           // Only search send->receive
             // If sender and receiver match: sender = sender, callnr = SEND+RECEIVE, receiver = receiver
            if(msg_m->receiver == g_m->receiver){
                msg = msg_m->call_nr == SEND ? msg_m->msg : g_m->msg;
                
                printf("unblock %d-%d-%d\n", msg_m->sender, msg_m->receiver, msg->m1_i1);
                unblock(msg_m->receiver, msg);
                unblock(msg_m->sender, msg);
                
                printf("before remove %d\n", proc_q->size);
                queue_func->removeitem(proc_q);              //Remove current message from proc_queue(not proc)
                printf("after remove %d\n", proc_q->size);
//                free(msg_m);
//                free(g_m);
                return 2;
            }
        }
        queue_func->enqueue(g_m, proc_q);                   //If not match, then enqueue this message.
        if(g_m->call_nr == 1){
            printf("Here we enqueue %d-%d\n", g_m->sender, g_m->receiver);
        }
        return 1;
    }
    return 0;
}

int getprocqueue(endpoint_t proc_e, mqueue **proc_q){
    void *value;
    mqueue *q;
    
    queue_func->iterator(msg_queue);
    while(queue_func->next(&value, msg_queue)){
        q = (mqueue *)value;
        if(q->number == proc_e){
            (*proc_q) = q;
            return 0;
        }
    }
    return -1;
}

int acquire_lock(void){
    while(lock != 0);
    lock = 1;       // Enter critical region.
}

int release_lock(void){
    lock = 0;
}
