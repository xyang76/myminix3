#include <stdio.h>
#include <string.h>
#include "minix/ipc.h"


int main()
{
	message msg, *m;
	int parent, child, st;
	m = &msg;
	st = 5;          //AMF_VALID && AMF_NOTIFY
	parent=getpid();
	if((child=fork())!=0){
		//this is parent
		msg.m1_i1 = 10;
		printf("send %d - %d\n", child, parent);
		//send(child, m);
		printf("yes, child send success\n");
	} else {
		//this is child
		printf("receive %d - %d\n", child, parent);
		receive(parent, &msg, &st);
		printf("yes, parent receive success %d\n", msg.m1_i1);
	}

	return 0;
}