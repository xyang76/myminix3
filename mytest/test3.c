#include <stdio.h>
#include <string.h>
#include <lib.h>    // provides _syscall and message
#include "minix/ipc.h"

int main()
{
    int gid, rv, child, parent=getpid();
    message m;
    gid=opengroup(0);
	printf("Start test\n");
    if((child=fork())==0){
        // child
        printf("start send\n");
        addproc(gid, getpid());
        m.m1_i1 = 10;
        rv = msend(gid, &m, parent);
        printf("finish send %d-%d\n", rv, errno);
    } else {
        // This is parent
        printf("start rec \n");
        addproc(gid, parent);
        rv = mreceive(gid, &m, child);
        printf("finish rec %d-%d-%d\n", rv, errno, m.m1_i1);
    }
    printf("finish test\n");
	return 0;
}
