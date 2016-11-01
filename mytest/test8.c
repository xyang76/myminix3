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


int main()
{
    message msg;
    int status,i, pid[10], st=5, rv;
    int server;
    if((server=fork()) == 0){
        message m, *m_ptr;
        int result;                 

      /* SEF local startup. */
//      env_setargs(argc, argv);
        sef_setcb_init_fresh(sef_cb_init_fresh);
  sef_setcb_init_restart(sef_cb_init_fail);

  /* No live update support for now. */

  /* Let SEF perform startup. */
        sef_startup();
      m_ptr = &m;
      while (TRUE) { 
          
        int status = sef_receive(0, m_ptr); 
        printf("message %d type %d", m_ptr->m_source, m_ptr->m_type);
        send(m_ptr->m_source, m_ptr);
      }
      exit(0);
    }
    
    for (i = 0; i < 10; i++){
        status = fork();
        if (status == 0 || status == -1) break;
        pid[i] = status;
    }
    if (status == -1){
        //Fork error
    } else if (status == 0){
        msg.m1_i1 = 10;
        msg.m_source = getpid();
        msg.m_type = 100;
        rv = send(server, &msg);
        printf("send %d %d\n", server, rv);
    } else {
        
//        printf("cur id:%d\n", parent);
//        while(1){
//            for(i=0; i<10; i++){
//                if(receive(pid[i], &msg, &st) == 0){
//                    printf("Yes rec success! %d\n", msg.m1_i1);
//                }
//            }
//        }
    }
	return 0;
}