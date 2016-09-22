#include "stdio.h"
#include "stdlib.h"
#include "string.h"

int main(int argc, char **argv)
{
    printf("Executing...\n");
    char *argv[] = {"ls", "ls","-al", "/etc", NULL};
    execlp("ls", argv);
}