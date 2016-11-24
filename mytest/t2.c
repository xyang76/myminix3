#include <stdio.h>
#include <string.h>
#include <lib.h>    


/**************************************************************
 *       test undelete
 *************************************************************/
int main()
{
    int rv;
    char *name;
    printf("enter name:\n");
    gets(name);
    rv = rcmkdir(name, 0777);
    printf("value %d\n", rv);
}