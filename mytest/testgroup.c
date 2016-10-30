#include <lib.h>    
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "testhelper.h"

/* test cases */
int test_opengroup_syscall();
int test_opengroup();
int test_closegroup_syscall();
int test_closegroup();   
int test_recovergroup_syscall();
int test_recovergroup();
int test_addproc_syscall();
int test_addproc();   
int test_rmproc_syscall();
int test_rmproc();   

int main(void) {
    /*open group*/
    test_opengroup_syscall();
    test_opengroup();
    
    /*close group*/
    test_closegroup_syscall();
    test_closegroup(); 

    /*add proc*/
    test_addproc_syscall(); 
    test_addproc();  
    
    /*remove proc*/
    test_rmproc_syscall();
    test_rmproc();  

    /*recover group*/
    test_recovergroup_syscall();
    test_recovergroup();
}

/* 
Test OPENGP syscall and Opengroup
*/
int test_opengroup_syscall(){
    message m;  
    int i;

    m.m1_i1 = 0;
 
    /* group id start from 0 */
    TEST_GREATER(_syscall(PM_PROC_NR, OPENGP, &m), -1, "test_open:opengroup should return id bigger than -1");
}

int test_opengroup(){
    int gid1, gid2;
    
    gid1 = opengroup(0);
    gid2 = opengroup(0);
    TEST_GREATER(gid1, -1, "opengroup should return id bigger than -1");        
    TEST_GREATER(gid2, gid1, "new group id > old group id");     
}

/* 
Test CLOSEGP syscall and Closegroup
*/
int test_closegroup_syscall(){
    message m;  
    int gid;

    gid=opengroup(0);

    m.m1_i1 = gid;
 
    TEST_EQUAL(_syscall(PM_PROC_NR, CLOSEGP, &m), 0, "test_close:closegroup should return 0");
}

int test_closegroup(){
    int gid;
    int f1;
    
    gid = opengroup(0);

    f1=closegroup(gid);

    TEST_EQUAL(f1, 0, "closegroup should return 0");         
}

/* 
Test RECOVERGP syscall and Recovergroup
*/
int test_recovergroup_syscall(){
    message m;  
    int i;
    int strategy=0;

    m.m1_i1 = 0;
    m.m1_i2 = strategy;
 
    TEST_EQUAL(_syscall(PM_PROC_NR, RECOVERGP, &m), 0, "test_recovery:recovergroup should return 0");
}

int test_recovergroup(){
    int gid;
    int strategy=0;
    int f;
	
    gid1 = opengroup(0);

    f1 = recovergroup(gid, strategy);

    TEST_EQUAL(f1, 0, "recovergroup should return 0");         
}

/* 
Test ADDPROC syscall and addproc
*/
int test_addproc_syscall(){
    message m;  
    int i;

    m.m1_i1 = opengroup(0);
    m.m1_i2 = fork();
 
    TEST_EQUAL(_syscall(PM_PROC_NR, ADDPROC, &m), 0, "test_addprocess:addproc should return 0");
}

int test_addproc(){
    int gid;
    int pid;
    int f;
    
    gid = opengroup(0);
    pid = fork();

    f = addproc(gid, pid);

    TEST_EQUAL(f, 0, "addproc should return 0");        
}

/* 
Test RMPROC syscall and rmproc
*/
int test_rmproc_syscall(){
    message m;  
    int gid;
    int pid;
    int f;
    
    gid = opengroup(0);
    pid = fork();

    addproc(gid, pid);

    m.m1_i1 = gid;
    m.m1_i2 = pid;
 
    TEST_EQUAL(_syscall(PM_PROC_NR, RMPROC, &m), 0, "test_removeprocess:rmproc should return 0");
}

int test_rmproc(){
    int gid;
    int pid;
    int f;
    
    gid = opengroup(0);
    pid = fork();

    addproc(gid, pid);
    f = rmproc(gid, pid);

    TEST_EQUAL(f, 0, "rmproc should return 0");        
}