#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "string.h"

int main(int argc, char **argv)
{
    char * argv2[] ={"test",0};
    int k=-999;  
    printf("Executing...\n");
    printf("value=%d\n", chdir(".."));
    if(fork() == 0){
    	if(execvp("test",argv2) <0){
	    printf("Command not found\n");
	}
    }
    printf("rv=[%d]\n", k);
}