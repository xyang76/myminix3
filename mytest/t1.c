#include <stdio.h>
#include <string.h>
#include <lib.h>    
#include "testhelper.h"
#include "minix/ipc.h"

/**************************************************************
 *       test undelete
 *************************************************************/
int main()
{
    int rv;
    char name[100];
    printf("enter name:\n");
    gets(name);
    rv = fundelete(name);
    if(rv == -1)
    printf("value %d\n", errno);
}