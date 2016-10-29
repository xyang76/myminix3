#include "stdio.h"
#include "minix/ipc.h"
#include "minix/syslib.h"

int grpnum=900;

int do_opengroup()
{
    message m;
    int r;
    printf("Yes, messaging\n");
    r = _taskcall(SYSTASK, SYS_IPCERRDTCT, &m);
    printf("Yes, messaging finish\n");
    return r;
}