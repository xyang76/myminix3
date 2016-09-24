#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "string.h"
#include <errno.h>

int main(int argc, char **argv)
{
    char * argv2[] ={"test",0};
    int k=-999;  
    printf("Executing...\n");
    printf("value=%d\n", chdir(".."));
    if(fork() == 0){
    	if(execvp("test",argv2)<0){
	    perror("ERROR1: exec failed ");
	}
	
	k = errno;
	errno = k;
    }
    k=errno;
    perror("ERROR2: exec failed ");
    printf("rv=[%d]\n", k);
}