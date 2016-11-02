#include <stdio.h>
#include <string.h>
#include <lib.h>    // provides _syscall and message
#include "minix/ipc.h"


int main()
{
    message m, msg, *m_sg;  // Minix uses message to pass parameters to a system call
    int rv, child, st=5, status;
    int parent = getpid();

    if((child=fork())==0){
        // This is child
        msg.m1_i1 = 10;
        msg.m_type=2;
        msg.m_source=getpid();
        
        m.m1_i1 = parent;
        m.m1_p1 = &msg;
        m.m_source=PM_PROC_NR;
        m.m_type = MSEND;
        printf("start send %d->%d\n", msg.m_source, m.m1_i1);
        rv= sendrec(PM_PROC_NR, &m);
        printf("send rv is %d\n", rv);
    } else {
        // This is parent
        waitpid(child, &status, 0);
        printf("start rec \n");
        rv = receive(PM_PROC_NR, &m, &st);
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