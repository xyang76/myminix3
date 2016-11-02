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
#include "mproc.h"
#include "param.h"
#include "mgroup.h"

static mgroup mgrp[NR_GRPS];            /* group table [this design is similar to proc design in minix] */
static mqueue *send_queue = NULL;       /* send queue*/
static mqueue *rec_queue = NULL;        /* receive queue */
static mqueue *unblock_queue = NULL;    /* unblock queue */
static int g_nr_ptr = 0;                /* group number ptr */
static int g_id_ctr = 1;                /* group id counter */

/* private methods prototype */
int invalid(int strategy);                                  /* valid strategy */ 
int deadlock(int src, int dest, int call_nr);               /* valid deadlock */ 
int getgroup(int grp_nr, mgroup ** g_ptr);                  /* get group by its gid */
int getprocindex(mgroup *g_ptr, int proc);                  /* get proc index in group*/
endpoint_t getendpoint(int proc_id);                        /* get endpoint from proc list*/
int getmproc(int proc_id);
int revokeproc(int proc_id);    
int rec_from(mgroup *g_ptr, int src, int dest, message *m);
int sent_to(mgroup *g_ptr, int src, int dest, message *m)

int do_opengroup()
{
    mgroup *g_ptr = NULL;
    int i, strategy;
    
    if(send_queue == NULL || rec_queue == NULL || unblock_queue == NULL){    //Init queues
        initQueue(&send_queue);
        initQueue(&rec_queue);
        initQueue(&unblock_queue);
    }
    
    strategy = m_in.m1_i1;
    if(invalid(strategy)){                         // Make sure strategy is valid. 0 is allowed
        return EIVSTTG;                             // Invalid strategy. which defined in sys/errno.h
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
    g_id_ctr++;
    
    return g_ptr->g_nr;
}

int do_addproc(){
    mgroup *g_ptr = NULL;
    int grp_nr, proc;	
    endpoint_t proc_ep;
    printf("in add proc\n");
    grp_nr = m_in.m1_i1;
    proc = m_in.m1_i2;
    if(!getgroup(grp_nr, &g_ptr)){
        printf("in add proc1.1\n");
        return EIVGRP;
    }else if(g_ptr->p_size >= NR_MGPROCS){
        printf("in add proc1.2\n");
        return EPROCLEN;                    // reach max length
    }else if((proc_ep=getendpoint(proc))<0){
        printf("in add proc1.3\n");
        return EIVPROC;
    }else if(getprocindex(g_ptr, proc) != -1){
        printf("in add proc1.4\n");
        return EPROCEXIST;                  // proc already exist
    }
    printf("in add proc2\n");
    *(g_ptr->p_lst+g_ptr->p_size) = proc;
    g_ptr->p_size++;
    return 0;
}

int do_rmproc(){
    mgroup *g_ptr = NULL;
    int i, grp_nr, proc;
    endpoint_t proc_ep;
    
    grp_nr = m_in.m1_i1;
    proc = m_in.m1_i2;
    if(getgroup(grp_nr, &g_ptr)){
        return EIVGRP;
    } else if((proc_ep=getendpoint(proc))<0){
        return EIVPROC;
    } else if((i=getprocindex(g_ptr, proc)) == -1){
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
    if(getgroup(grp_nr, &g_ptr)){
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
    }else if(getgroup(grp_nr, &g_ptr)){
        return EIVGRP;
    }
    
    return 0;
}

int do_msend(){
    int rv, src, grp_nr, send_type, *p;
    message m;
    mgroup *g_ptr = NULL;
    
    src = m_in.m1_i1;
    grp_nr = m_in.m1_i2;
    send_type = m_in.m1_i3;

    if(!getgroup(grp_nr, &g_ptr)){
        return EIVGRP;
    } else if(getprocindex(g_ptr, src) == -1){
        return -2;
    }
    rv = sys_datacopy(who_e, (vir_bytes) m_in.m1_p1,
		PM_PROC_NR, (vir_bytes) &m, (phys_bytes) sizeof(m));
        
    for(p=g_ptr->p_lst; p<g_ptr->p_lst+NR_MGPROCS && p <= g_ptr->p_lst+p_size; p++){  
        if(*p != src){
            sendnb(getendpoint(*p), &m);
        }
    }
    
    printf("Now msend finish\n");
    return rv;
}

int do_mreceive(){
    int rv, src, grp_nr, rec_type, *proclist, *status_ptr, *p;
    message m, *msg;
    mgroup *g_ptr = NULL;
    
    src = m_in.m1_i1;
    grp_nr = m_in.m1_i2;
    rec_type = m_in.m1_i3;
    
    if(!getgroup(grp_nr, &g_ptr)){
        return EIVGRP;
    } else if(getprocindex(g_ptr, src) == -1){
        return -2;
    }
    if ((message *) m_in.m1_p1 != (message *) NULL) {
        rv = sys_datacopy(PM_PROC_NR,(vir_bytes) msg,
            who_e, (vir_bytes) m_in.m1_p1, (phys_bytes) sizeof(m));
        if (rv != OK) return(rv);
    }
    
    printf("Now mreceive\n");
    /*
    switch(rec_type){
        case RECANY:                                            // Will not block
            for(p=g_ptr->p_lst; p<NR_MGPROCS && *p != 0; p++){
                if(*p != src && iswaiting(*p)>0 && isinqueue(src, *p, send_queue)){
                    return rec_from(g_ptr, src, *p, msg);
                }
            }
            break;
        case IPCNONBLOCK:                                       // will block
            for(p=g_ptr->p_lst; p<NR_MGPROCS && *p != 0; p++){
                if(*p != src && iswaiting(*p)==0){
                    rv = rec_from(g_ptr, src, *p, msg);
                    mp->mp_flags |= WAITING;
                    return rv==0? (SUSPEND) : rv;
                }
            }
            break;
        default:
            if(getprocindex(g_ptr, getendpoint(rec_type) == -1)){
                return -3;
            }else if(rec_type != src && iswaiting(rec_type)>0 && isinqueue(src, rec_type, send_queue)){
                return rec_from(g_ptr, src, rec_type, msg);  //NON block
            } else {
                rv = rec_from(g_ptr, src, rec_type, msg);  
                mp->mp_flags |= WAITING;
                return rv==0? (SUSPEND) : rv;
            }
    }*/
    printf("m receive finish %d\n", rv);
    return 0;
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

int iswaiting(int proc_id){
    register struct mproc *rmp;
    if(proc_id < 0){
        return -1;
    }
    for (rmp = &mproc[NR_PROCS-1]; rmp >= &mproc[0]; rmp--){ 
        if (!(rmp->mp_flags & IN_USE)) continue;
        if (proc_id > 0 && proc_id == rmp->mp_pid){
            if(rmp->mp_flags & WAITING){
                return 1;
            } 
            return 0;
        } 
    }
    return -1;
}

int isinqueue(int src, int proc_id, mqueue *queue){
    return 1;
}

int unblock_list(){
}

int sent_to( mgroup *g_ptr, int src, int dest, message *m){
    grp_message *g_m;
    
    if(dest != src && iswaiting(dest)>0 && isinqueue(src, dest, rec_queue)){
        //non block getitem
//        for(int i=0; i<)
        //unblock item
        
    } else {
        // add item
        g_m = (grp_message*) malloc(sizeof(grp_message));
        g_m->grp_nr = g_ptr->g_nr;
        g_m->msg = m;
        g_m->call_nr = SEND;
        g_m->dest = dest;
        g_m->src = src;
        push(&g_m, send_queue);
        mp->mp_flags |= WAITING;
        return (SUSPEND);
    } 
}

int rec_from(mgroup *g_ptr, int src, int dest, message *m){
    grp_message *g_m;
    if(dest != src && iswaiting(dest)>0 && isinqueue(src, dest, rec_queue)){
        //non block getitem
        
        //unblock item
        
    } else {
        g_m = (grp_message*) malloc(sizeof(grp_message));
        g_m->grp_nr = g_ptr->g_nr;
        g_m->msg = m;
        g_m->call_nr = RECEIVE;
        g_m->dest = dest;
        g_m->src = src;
        push(g_m, rec_queue);
        mp->mp_flags |= WAITING;
        return (SUSPEND);
    }
}
