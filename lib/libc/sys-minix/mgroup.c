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
int recovergroup(int grp_nr, int stategy){
    message m;
    
    m.m1_i1 = grp_nr;
    m.m1_i2 = stategy;
    return _syscall(PM_PROC_NR, CLOSEGP, &m);
}


