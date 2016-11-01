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
    message msg;
    int status,i, pid[10], st=5, rv;
    int parent = getpid();
    for (i = 0; i < 10; i++){
        status = fork();
        if (status == 0 || status == -1) break;
        pid[i] = status;
    }
    if (status == -1){
        //Fork error
    } else if (status == 0){
//        if(i<1){        //Receiver
            
//        } else {
        msg.m1_i1 = 10;
        rv = send(parent, &msg);
        printf("send %d %d\n", parent, rv);
//        }
    } else {
//        printf("cur id:%d\n", parent);
//        while(1){
//            for(i=0; i<10; i++){
//                if(receive(pid[i], &msg, &st) == 0){
//                    printf("Yes rec success! %d\n", msg.m1_i1);
//                }
//            }
//        }
    }
	return 0;
}