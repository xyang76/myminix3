#include <stdio.h>
#include <string.h>
#include <lib.h>    // provides _syscall and message
#include "minix/ipc.h"

int main()
{
    int child, parent=getpid();
    
	printf("Start test\n");
    if((child=fork())==0){
        // child
        printf("start send\n");
        rv = msend(1, &msg, parent);
        printf("finish send %d\n", rv);
    } else {
        // This is parent
        printf("start rec \n");
        rv = mreceive(1, &m, child);
        printf("start rec %d", rv);
    }
    printf("finish test\n");
	return 0;
}
