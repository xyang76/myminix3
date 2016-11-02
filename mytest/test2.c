#include <stdio.h>
#include <string.h>
#include <lib.h>    // provides _syscall and message
#include "minix/ipc.h"


int main()
{
    message m, msg, *m_sg;  // Minix uses message to pass parameters to a system call
    int rv, child, st=5, status, pid;
    int pidlst[5] = {1,2,3,4,0};
    int parent = getpid();

    if((child=fork())==0){
        printf("start send\n");
        msend(parent, &msg, pidlst);
        printf("finish send\n");
    } else {
        // This is parent
//        printf("start rec \n");
//        rv = receive(PM_PROC_NR, &m, 1);
//        printf("start rec is %d, %d, %d\n", rv, m.m1_i1, status);
    }
}