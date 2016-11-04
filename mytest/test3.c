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
        rv = msend(gid, &m, parent);
        printf("finish send %d-%d\n", rv, errno);
    } else {
        // This is parent
        printf("start rec \n");
        rv = mreceive(gid, &m, child);
        printf("finish rec %d-%d\n", rv, errno);
    }
    printf("finish test\n");
	return 0;
}
