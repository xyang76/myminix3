#include <stdio.h>
#include <string.h>
#include <lib.h>    
#include "minix/ipc.h"
#include "testhelper.h"


/**************************************************************
 *       test link
 *************************************************************/
int main()
{
    int rv;
    char *name1;
    char *name2;
    printf("enter name1:\n");
    gets(name1);
    printf("enter name2:\n");
    gets(name2);
    printf("name1: [%s] :: name2: [%s]\n", name1, name2);
    rv = link(name1, name2);
    printf("value %d\n", rv);
}