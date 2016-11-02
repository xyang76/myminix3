#include <sys/cdefs.h>
#include "namespace.h"
#include <lib.h>
#include <unistd.h>

int opengroup(int strategy){
    message m;
    
    m.m1_i1 = strategy;
    return _syscall(PM_PROC_NR, OPENGP, &m);
}

int addproc(int grp_nr, int proc){
    message m;
    
    m.m1_i1 = grp_nr;
    m.m1_i2 = proc;
    return _syscall(PM_PROC_NR, ADDPROC, &m);
}

int rmproc(int grp_nr, int proc){
    message m;
    
    m.m1_i1 = grp_nr;
    m.m1_i2 = proc;
    return _syscall(PM_PROC_NR, RMPROC, &m);
}

int closegroup(int grp_nr){
    message m;
    
    m.m1_i1 = grp_nr;
    return _syscall(PM_PROC_NR, CLOSEGP, &m);
}

int recovergroup(int grp_nr, int strategy){
    message m;
    
    m.m1_i1 = grp_nr;
    m.m1_i2 = strategy;
    return _syscall(PM_PROC_NR, RECOVERGP, &m);
}

int msend(int dest, char *msg, int* proclist){
    message m;
    int rv, pid, status, proc_num, *p;
    
    for(p=proclist, proc_num=0; p != NULL && *p != 0; p++, proc_num++)
    
    m.m1_i1 = getpid();
    m.m1_i2 = dest;
    m.m1_i3 = proc_num;
    m.m1_p1 = msg;
    m.m1_p2 = (char *)proclist;
   
    return _syscall(PM_PROC_NR, MSEND, &m);
}

int mreceive(int src, char *msg, int* proclist, int *status_ptr){
    message m;
    int rv, pid, status;
    
    m.m1_i1 = getpid();
    m.m1_i2 = src;
    m.m1_p1 = msg;
    m.m1_p2 = (char *)proclist;
    m.m1_p3 = (char *)status_ptr;
    if((pid=fork()) == 0){
        rv = _syscall(PM_PROC_NR, MRECEIVE, &m);
        printf("rv is %d", rv);
        exit(rv);
    } else {
//        waitpid(pid, &status, 0);
        return status;
    }
}


