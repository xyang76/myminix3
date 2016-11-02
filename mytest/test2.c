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
        msg.m1_i1 = 10;
        msg.m_type=2;
        msg.m_source=getpid();
        m_sg = &msg;
        
        printf("start send %d->%d\n", msg.m_source, m.m1_i1);
        m.m1_i1 = getpid();
        m.m1_i2 = parent;
        m.m1_p1 = (char*)msg;
//        m.m1_p2 = (char *)proclist;
        if((pid=fork()) == 0){
            rv = _syscall(PM_PROC_NR, MSEND, &m);
            exit(rv);
        } else {
            waitpid(pid, &status, 0);
            return status;
        }
        printf("send rv is %d\n", rv);
        printf("send is not waiting %d\n", rv);
    } else {
        // This is parent
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