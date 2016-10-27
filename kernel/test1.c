//#include <stdio.h>
//#include <string.h>
//#include <mprofile.h>
//extern int load_profile(char *name);
#include "kernel/kernel.h"
#include "arch_proto.h"
#include <signal.h>
#include <string.h>
#include <assert.h>
#include "kernel/proc.h"
#include "kernel/proto.h"
#include <machine/vm.h>


int main()
{
    
	printf("Success\n");

	return 0;
}

int test(struct proc *pr){
    message m_pagefault;
    
    mini_send(pr, VM_PROC_NR, &m_pagefault, FROM_KERNEL);
}