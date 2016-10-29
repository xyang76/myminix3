#include <stdio.h>
#include <string.h>
#include <lib.h>    // provides _syscall and message
#include "minix/ipc.h"


int main()
{
    message m;  // Minix uses message to pass parameters to a system call
    int i;

    m.m1_i1 = 10;
    
    i = _syscall(PM_PROC_NR, 108, &m);
    printf("rv is %d\n", i);
}