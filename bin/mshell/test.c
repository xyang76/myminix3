#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "string.h"
#include "eval.h"

int main(int argc, char **argv)
{
    char * argv2[] ={"ls",0};
    char *envp[]={"PATH=/bin", NULL};   
    printf("Executing...\n");
    printf("value=%d", chdir(".."));
    evalcmd(1,argv2);
}