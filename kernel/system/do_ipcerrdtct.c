#include "kernel/system.h"
#include <minix/endpoint.h>
#include "stdio.h"

/*===========================================================================*
  *                                do_ipc_error_detect                        *
  *===========================================================================*/
int do_ipcerrdtct(struct proc *caller_ptr, message *m_ptr)
{
	printf("Yes, in ipc\n");
        return(OK);
}