#include <stdio.h>
#include <string.h>
#include <lib.h>    // provides _syscall and message
#include "minix/ipc.h"


int main()
{
	message msg, *m;
    int parent, child, i;
    m = &msg;
    st = 5;          //AMF_VALID && AMF_NOTIFY
	parent=getpid();
	if((child=fork())!=0){
        //this is parent
        msg.m1_i1 = 10;
        printf("send %d - %d\n", child, parent);
		i = _syscall(PM_PROC_NR, 108, &msg);
		printf("rv is %d\n", i); 
	} else {
        //this is child
        printf("receive %d - %d\n", child, parent);
		i = _syscall(PM_PROC_NR, 108, &msg);
		printf("rv is %d\n", i); 
	}

	return 0;
}