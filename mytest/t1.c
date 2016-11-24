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
    rv = undelete(name);
    printf("value %d\n", rv);
}