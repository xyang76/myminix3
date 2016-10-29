#include "stdio.h"
#include "minix/ipc.h"
#include "minix/syslib.h"

int grpnum=900;

int do_opengroup()
{
    message m;

    return(_taskcall(SYSTASK, SYS_IPCERRDTCT, &m));
}