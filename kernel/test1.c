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
    int parent, child, k, *st;
	
	msg.m1_i1 = 10;
    m = &msg;
    k = 5;
    st = &k;
	parent=getpid();
	if((child=fork())!=0){
        //this is parent
        printf("send %d - %d\n", child, parent);
		send(child, m);
		printf("yes, child send\n");
	} else {
        //this is child
        printf("receive %d - %d\n", child, parent);
		receive(parent, &msg, st);
		printf("yes, parent receive\n");
	}

	return 0;
}

int test(int i){
    return 0;
}