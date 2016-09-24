#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "string.h"

int main(int argc, char **argv)
{
    char * argv2[] ={"ls","-al",0};
    execvp("ls",argv2);
    printf("Executing...\n");
    printf("value=%d", chdir("/root"));
    execvp("ls",argv2);
}