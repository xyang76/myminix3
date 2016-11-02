#include <stdio.h>
#include <string.h>
#include <lib.h>    // provides _syscall and message
#include "minix/ipc.h"


int main()
{
    message m, msg, *m_sg;  // Minix uses message to pass parameters to a system call
    int rv, child, st=5, status, pid;
    int parent = getpid();

    if((child=fork())==0){
        // This is child
//        msg.m1_i1 = 10;
//        msg.m_type=2;
//        msg.m_source=getpid();
//        m_sg = &msg;
        
        printf("start send\n");
//        m.m1_i1 = getpid();
//        m.m1_i2 = parent;
//        m.m1_p1 = (char*)m_sg;
//        m.m1_p2 = (char *)proclist;
        msend(parent, &msg, NULL);
        printf("finish send\n");
    } else {
        // This is parent
        printf("start rec \n");
        rv = mreceive(child, &m, NULL, 0);
        printf("start rec is %d, %d, %d\n", rv, m.m1_i1, status);
//        m.m1_i1 = PM_PROC_NR;
//        m.m1_p1 = m_sg;
//        m.m_source=getpid();
//        m.m_type = 2;
//        printf("start rec %d->%d\n", m.m_source, m.m1_i1);
//        rv= _syscall(PM_PROC_NR, MRECEIVE, &m);
//        printf("rec rv is %d\n", rv);
    }
}