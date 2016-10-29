#include <lib.h>
#include <minix/com.h>
#include <minix/syslib.h>

int sys_ipcerrdtct(int function, endpoint_t src_e, endpoint_t dest_e)
{
	int r;
        message m;

        //return(_kernel_call(SYS_IPCERRDTCCT, &m));
	r = _taskcall(SYSTASK, SYS_IPCERRDTCCT, &m);
	return r;
}