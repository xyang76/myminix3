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
    char cmd[MAXCOMMAND], path[MAXPPATH], c;
    sigset_t mask;
    struct sigaction sa;
    int k=0, i;
    
    printf("\n-------------------------------\n");
    printf("Welcome to my shell!\n");
    printf("Author: Xincheng Yang, Yanqing Gong\n");
    printf("HOME:/usr/src\n");
    printf("You also can change profile path by command 'loadprofile'\n");
    printf("Eg: > loadprofile /etc/.profile\n");
    printf("-------------------------------\n");
    
    
    sa.sa_flags = 0;
    sa.sa_handler = sigint_handler;
    sigaction(SIGINT,  &sa, NULL);	        // Handler for ctrl+c interrupt.
    sigfillset(&mask);
    sigdelset(&mask,SIGINT);
    sigprocmask(SIG_BLOCK,&mask,NULL);
    
    read_profile();                         // Read profile from default profile
    
    while(1){
        getcwd(path, MAXPPATH);
        printf("\n%s> ", path);
        
        while(fgets(cmd, MAXCOMMAND, stdin) == NULL);

        printf("Current = %s\n", cmd);
//        precompile(cmd);  
        
        fflush(stdin);    
        k++;
        if(k>5){
            printf("Infinity\n");
            break;
        }
    }
}

