#include "syslib.h"

int sys_ipcerrdtct(int function, endpoint_t src_e, endpoint_t dest_e)
{
	int r;
        message m;

        return (_kernel_call(SYS_IPCERRDTCT, &m));
}