#include <stdio.h>
#include <string.h>
#include <lib.h>    
#include <unistd.h>
#include <testhelper.h>

/**************************************************************
 *       test undelete
 *************************************************************/
int main()
{
    int rv;
    char name[100];
    printf("enter name:\n");
    gets(name);
    rv = rcmkdir(name, 0777);
    if(rv == -1)
    printf("value %d\n", errno);
}