#include <lib.h>    // provides _syscall and message
#include <stdio.h>

int main(void) {
    message m;  // Minix uses message to pass parameters to a system call
    int i;
    
    i = _syscall(PM_PROC_NR, 108, &m);
        /* _syscall leads to the system server identified by PM_PRO_NR (the PM
         * server process) invoking the function identified by call number 
         * PRINTMSG with parameters in the message copied to address &m
         */
    printf("rv is %d\n", i);   
}