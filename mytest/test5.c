#include <stdio.h>
#include <string.h>
#include <lib.h>    
#include "minix/ipc.h"

int main()
{
    message m, *msg;
    int status,i, pid[10], rv, parent=getpid();
    int gid = opengroup(0);
    addproc(gid, parent);
    msg = &m;
    
    for (i = 0; i < 4; i++){
        status = fork();
        if (status == 0 || status == -1) break;
        pid[i] = status;
        addproc(gid, pid[i]);
    }
    if (status == -1){
        //Fork error
    } else if (status == 0){
        //Child proc
        while(mreceive(gid, &m, parent)!=0);    
        printf("finish receive %d-%d\n", rv, errno);	
    } else {
        //Parent proc    
        printf("this is parent, cur id:%d\n", parent);
        
//        for(i=0; i<4; i++){
//            printf("msend to %d ----------------- \n", pid[i]);
        rv = msend(gid, &m, SENDALL);
        closegroup(gid);
    }
    return 0;
}