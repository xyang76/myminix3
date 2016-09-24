#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "string.h"

int main(int argc, char **argv)
{
    char * argv2[] ={"test",0};
    int k=0;  
    printf("Executing...\n");
    printf("value=%d", chdir("~"));
    if(fork() == 0){
    	k = execvp("test",argv2);
	printf("rv=%d", k);
    }
    printf("rv=[%d]", k);
}