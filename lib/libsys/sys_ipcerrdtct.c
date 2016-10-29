#include "syslib.h"

sys_ipcerrdtct(int function, endpoint_t src_e, endpoint_t dest_e)
{
        message m;

        return(_kernel_call(SYS_IPCERRDTCCT, &m));
}