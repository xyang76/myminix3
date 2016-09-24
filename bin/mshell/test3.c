#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "string.h"

int main(int argc, char **argv)
{
    char * argv2[] ={"test",0};
    printf("Executing...\n");
    printf("value=%d", chdir("~"));
    printf("end=%d\n", execvp("test",argv2));
}