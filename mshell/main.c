#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "unistd.h"
#include "sys/types.h" 
#include "signal.h"
#include "mshell.h"
#include "mprofile.h"
#include "errdef.h"

int 
main(int argc, char **argv)
{
    char cmd[MAXCOMMAND], path[MAXPPATH];
    
    printf("\n-------------------------------\n");
    printf("Welcome to my shell!\n");
    printf("Author: Xincheng Yang, Yanqing Gong\n");
    printf("HOME:/usr/src\n");
    printf("You also can change profile path by command 'loadprofile'\n");
    printf("Eg: > loadprofile /etc/.profile\n");
    printf("-------------------------------\n");
    
    signal(SIGINT,  sigint_handler);
    read_profile();
    
    while(1){
        getcwd(path, MAXPPATH);
        printf("\n%s> ", path);
        
        gets(cmd);

        if(precedence_check(cmd) != -1){ 
            precedence_parser(cmd);
        } else {
            print_error(PRECEDENCE_UNMATCH);
        }            
    }
}

