#include <lib.h>    // provides _syscall and message
#include <stdio.h>

int main(void) {
    message m;  // Minix uses message to pass parameters to a system call
    int i;

    m.m1_i1 = 10;
    
    i = _kernel_call(SYS_IPCERRDTCT, &m);
    printf("rv is %d\n", i);   
}