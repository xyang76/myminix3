#include <lib.h>    
#include <stdio.h>
#include <uinstd.h>
#include "testhelper.h"

/* test cases */
int test_opengroup_syscall();
int test_opengroup();

int main(void) {
    test_opengroup_syscall();
    test_opengroup();
}

int test_opengroup_syscall(){
    message m;  
    int i;

    m.m1_i1 = 0;
    
    /* group id start from 0 */
    TEST_GREATER(_syscall(PM_PROC_NR, OPENGP, &m), -1, "opengroup should return id bigger than -1");
}

int test_opengroup(){
    int gid1, gid2;
    
    gid1 = opengroup(0);
    gid2 = opengroup(0);
    TEST_GREATER(gid1, -1, "opengroup should return id bigger than -1");        
    TEST_GREATER(gid2, gid1, "new group id > old group id");     
}