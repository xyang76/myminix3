#include "syslib.h"

//int sys_ipcerrdtct(int function, endpoint_t src_e, endpoint_t dest_e, char *msg)
int sys_ipcerrdtct(int function, endpoint_t src_e, endpoint_t dest_e)
{
	int r;
    message m, msg;
    
    m.m1_i1 = dest_e;
    m.m1_p1 = msg;
    m.m_type = ((message*)msg).m_type;
    m.m_source = src_e;
    return (_kernel_call(SYS_IPCERRDTCT, &m));
}

int sys_groupsend(int function, endpoint_t src_e, endpoint_t dest_e)
{
	int r;
    message m, msg;
    
    m.m1_i1 = dest_e;
    m.m1_p1 = msg;
    m.m_type = ((message*)msg).m_type;
    m.m_source = src_e;
    return (_kernel_call(SYS_IPCERRDTCT, &m));
}

int sys_groupreceive(int function, endpoint_t src_e, endpoint_t dest_e)
{
	int r;
    message m, msg;
    
    m.m1_i1 = dest_e;
    m.m1_p1 = msg;
    m.m_type = ((message*)msg).m_type;
    m.m_source = src_e;
    return (_kernel_call(SYS_IPCERRDTCT, &m));
}