#include "stdio.h"
#include "pm.h"
#include <sys/wait.h>
#include <assert.h>
#include <minix/callnr.h>
#include <minix/com.h>
#include <minix/sched.h>
#include <minix/vm.h>
#include <sys/ptrace.h>
#include <sys/resource.h>
#include <signal.h>
#include "mproc.h"
#include "param.h"

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