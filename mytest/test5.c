#include <lib.h>    // provides _syscall and message
#include <stdio.h>
#include <minix/syslib.h>

int main(void) {
    message m;  // Minix uses message to pass parameters to a system call
    int i;

    m.m1_i1 = 10;
    
    i = sys_ipcerrdtct(2, 10, 15);
    printf("rv is %d\n", i);   
}