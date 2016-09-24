#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "string.h"

int main(int argc, char **argv)
{
    char * argv2[] ={"ls",0};
    char *envp[]={"PATH=/bin", NULL};
    int k=0;   
    printf("Executing...\n");
    printf("value=%d\n", chdir(".."));
    if(fork() == 0){
    	k = execve("/bin/ls",argv2, envp);
	printf("rv=%d\n", k);
    }
    printf("rv=[%d]\n", k);
}