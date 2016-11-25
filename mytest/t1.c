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
    char *name;
    printf("enter name:\n");
    gets(name);
    rv = fundelete(name);
    printf("value %d\n", rv);
}