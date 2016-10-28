#include <stdio.h>
#include <string.h>
//#include <mprofile.h>
//extern int load_profile(char *name);
//#include "kernel/kernel.h"
//#include "arch_proto.h"
//#include <signal.h>
//#include <string.h>
//#include <assert.h>
//#include "proc.h"
//#include "proto.h"
//#include <machine/vm.h>
#include "minix/ipc.h"


int main()
{
	message msg, *m;
    int parent, child;
	
	msg.m1_i1 = 10;
    m = &msg;
	parent=getpid();
	if((child=fork())==0){
		send(child, m);
		printf("yes, send");
	} else {
		receive(parent, &msg, 0);
		printf("yes, receive");
	}

	return 0;
}

int test(int i){
    return 0;
}