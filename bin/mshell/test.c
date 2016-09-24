#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "string.h"

int main(int argc, char **argv)
{
    char * argv2[] ={"ls",0};
    execvp("ls",argv2);
    printf("Executing...\n");
    printf("value=%d", chdir("/usr/src"));
    execvp("ls",argv2);
}