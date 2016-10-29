#include "stdio.h"
#include "minix/ipc.h"
#include "syslib.h"

int grpnum=900;

int do_opengroup()
{
    message m;

    return(_kernel_call(SYS_IPCERRDTCT, &m));
}