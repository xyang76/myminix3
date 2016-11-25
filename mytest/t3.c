#include <stdio.h>
#include <string.h>
#include <lib.h>    
#include <unistd.h>

/**************************************************************
 *       test undelete
 *************************************************************/
int main()
{
    int rv;
    char *name;
    printf("enter name:\n");
    gets(name);
    rv = remove(name);
    printf("value %d\n", rv);
}