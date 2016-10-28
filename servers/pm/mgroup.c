#include "stdio.h"
#include "minix/ipc.h"

int grpnum=900;

int do_opengroup()
{
    grpnum++;
    return grpnum;
}