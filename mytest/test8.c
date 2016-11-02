#include <stdio.h>
#include <string.h>
#include "minix/ipc.h"
#define _POSIX_SOURCE      1	/* tell headers to include POSIX stuff */
#define _MINIX             1	/* tell headers to include MINIX stuff */
#define _SYSTEM            1    /* get OK and negative error codes */

#include <sys/types.h>
#include <sys/param.h>
#include <limits.h>
#include <errno.h>
#include <regex.h>

#include <minix/callnr.h>
#include <minix/config.h>
#include <minix/type.h>
#include <minix/const.h>
#include <minix/com.h>
#include <minix/ds.h>
#include <minix/syslib.h>
#include <minix/sysinfo.h>
#include <minix/sysutil.h>
#include <minix/keymap.h>
#include <minix/bitmap.h>
#include <minix/rs.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>

#include <minix/endpoint.h>

int sef_cb_init_fresh(int type, sef_init_info_t *info){
    return 0;
}

int main()
{
    message msg;
    int status,i, pid[10], st=5, rv, parent=getpid();
    int gid = opengroup(0);
    addproc(gid, parent);
    
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
        while(1){
            rv=mreceive(gid, &msg, parent);
            if(rv==-1){
                printf("mreceive fail, error-no %d\n", errno);
                break;
            }
            printf("yes receive success! %d", rv);
        }
    } else {
        //Parent proc    
        printf("cur id:%d\n", parent);
        
        for(i=0; i<4; i++){
            if((rv=msend(gid, &msg, pid[i])) == 0){
                printf("Yes send success! %d\n", msg.m1_i1);
            } else {
                printf("msend fail, error-no %d\n", errno);
            }
        }
        closegroup(gid);
    }
	return 0;
}