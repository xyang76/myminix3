#include "stdio.h"
#include "minix/ipc.h"
#include <lib.h>
#include <minix/com.h>
#include <minix/syslib.h>

int grpnum=900;

int do_opengroup()
{
    message m;
    int r;
    printf("Yes, messaging\n");
    r = sys_ipcerrdtct(2, 10, 15);
    //r = _taskcall(SYSTASK, SYS_IPCERRDTCT, &m);
    printf("Yes, messaging finish\n");
    return r;
}