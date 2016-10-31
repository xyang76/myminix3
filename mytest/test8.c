#include <stdio.h>
#include <string.h>
#include "minix/ipc.h"


int main()
{
    /*
	message msg, *m;
	int parent, child, st;
	m = &msg;
	st = 5;          //AMF_VALID && AMF_NOTIFY
	parent=getpid();
	if((child=fork())!=0){
		//this is parent
		msg.m1_i1 = 10;
		printf("send %d - %d\n", child, parent);
		receive(child, &msg, &st);
		//send(child, m);
		printf("yes, child receive success %d\n", msg.m1_i1);
	} else {
		//this is child
		msg.m1_i1 = 20;
		printf("receive %d - %d\n", child, parent);
		receive(parent, &msg, &st);
		printf("yes, parent receive success %d\n", msg.m1_i1);
	}*/
    
    int status,i, pid[10];
    for (i = 0; i < 10; i++){
        status = fork();
        if (status == 0 || status == -1) break;
        pid[i] = status;
    }
    if (status == -1){
        //Fork error
    } else if (status == 0){
        
    } else {
        for(i=0; i<10; i++){
            printf("pid=%d\n", pid[i]);
        }
    }
	return 0;
}