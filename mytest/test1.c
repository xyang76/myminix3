#include <stdio.h>
#include <string.h>
#include "minix/ipc.h"


int main()
{
	message msg, *m;
	int parent, child, st, rv;
	m = &msg;
	st = 5;          //AMF_VALID && AMF_NOTIFY
	parent=getpid();
	if((child=fork())!=0){
		//this is parent
		msg.m1_i1 = 10;
		printf("send %d - %d\n", child, parent);
		rv=send(child, m);
		printf("yes, parent send success, rv=%d\n", rv);
        rv=receive(child, &msg, &st);
        printf("yes, parent rec success, rv=%d\n", rv);
	} else {
		//this is child
		printf("receive %d - %d\n", child, parent);
		//while(1){
        rv = receive(parent, &msg, &st);
        printf("yes, child receive success %d, rv=%d\n", msg.m1_i1, rv);
		rv=send(parent, m);
        printf("yes, child send success, rv=%d\n", rv);
	}
	while(1);		//infinity loop to keep this proc not terminate.
	return 0;
}