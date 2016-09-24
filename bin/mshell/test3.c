#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "string.h"

int main(int argc, char **argv)
{
    char * argv2[] ={"pwd",0};
    printf("Executing...\n");
    printf("value=%d", chdir("~"));
    execvp("pwd",argv2);
}