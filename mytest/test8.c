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
    int status,i, pid[10], st=5, rv;
    
    for (i = 0; i < 4; i++){
        status = fork();
        if (status == 0 || status == -1) break;
        pid[i] = status;
    }
    if (status == -1){
        //Fork error
    } else if (status == 0){
        //Child proc
    } else {
        //Parent proc    
        printf("cur id:%d\n", parent);
        while(1){
            for(i=0; i<10; i++){
                if(receive(pid[i], &msg, &st) == 0){
                    printf("Yes rec success! %d\n", msg.m1_i1);
                }
            }
        }
    }
	return 0;
}