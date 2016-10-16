#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "unistd.h"
#include "sys/types.h" 
#include "signal.h"
#include "mshell.h"
#include "mprofile.h"
#include "errdef.h"

int main(int argc, char **argv)
{
    char cmd[MAXCOMMAND], path[MAXPPATH];
    sigset_t mask;
    int k=0;
    
    printf("\n-------------------------------\n");
    printf("Welcome to my shell!\n");
    printf("Author: Xincheng Yang, Yanqing Gong\n");
    printf("HOME:/usr/src\n");
    printf("You also can change profile path by command 'loadprofile'\n");
    printf("Eg: > loadprofile /etc/.profile\n");
    printf("-------------------------------\n");
    
    signal(SIGINT,  sigint_handler);        // Handler for ctrl+c interrupt.
    sigfillset(&mask);
    sigdelset(&mask,SIGQUIT);
    sigdelset(&mask,SIGKILL);
    sigdelset(&mask,SIGTERM);
    sigprocmask(SIG_BLOCK ,&mask, NULL);
    
    read_profile();                         // Read profile from default profile
    
    while(1){
        getcwd(path, MAXPPATH);
        printf("\n%s> ", path);
        
        if (fgets(cmd, MAXCOMMAND, stdin) == NULL){
            printf("cant get message from stdin!\n");
            break;
        }
        precompile(cmd);  
        fflush(stdin);
    }
}


