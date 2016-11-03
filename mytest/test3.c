//#include <stdio.h>
//#include <string.h>
//#include <mprofile.h>
//extern int load_profile(char *name);
#include "kernel.h"
//#include "arch_proto.h"
#include <signal.h>
#include <string.h>
#include <assert.h>
#include "proc.h"
#include "proto.h"
#include <machine/vm.h>

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
