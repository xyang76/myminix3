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

extern int mini_send(
  register struct proc *caller_ptr,	/* who is trying to send a message? */
  endpoint_t dst_e,			/* to whom is message being sent? */
  message *m_ptr,			/* pointer to message buffer */
  const int flags
);

int main()
{
    
	printf("Success\n");

	return 0;
}

int test(proc *pr){
    struct message m_pagefault;
    
    mini_send(pr, VM_PROC_NR, &m_pagefault, FROM_KERNEL);
}