#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "unistd.h"
#include "errno.h"
#include "sys/types.h" 
#include "sys/wait.h"
#include "signal.h"
#include "mstack.h"
#include "malias.h"
#include "mshell.h"
#include "mprofile.h"

int 
main(int argc, char **argv)
{
    char cmd[MAXCOMMAND], path[MAXPPATH];
    
    printf("\n-------------------------------\n");
    printf("Welcome to my shell!\n");
    printf("Author: Xincheng Yang, Yanqing Gong\n");
    printf("HOME:/usr/src\n");
    printf("You also can change home env by command 'loadprofile'\n");
    printf("Eg: > loadprofile /etc\n");
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
            printf("incorrect input");
        }            
    }
}

void sigint_handler(int signal) 
{
    char rv[20];
    
    printf("Are you sure? (Y/N) :");
    gets(rv);
    fflush(stdout);
    
    if (rv[0]=='Y' || rv[0] == 'y'){
        exit(0);
    }
}

int 
precedence_check(char *cmd){
    int i,tier,max;
    for(i=0, tier=0, max=0; i<strlen(cmd); i++){
        if(cmd[i] == '('){
            tier++;
            if(max<tier){
                max = tier;
            }
        } else if(cmd[i] == ')'){
            tier--;
        }
    }
    return tier==0 ? max:-1;
}

int 
build_argv(char *cmd, int argc){
    char *argv[argc+10];
    int i, j, k;
    
    // Check empty command, if command is empty, then return 1.
    while(*cmd == ' '){
        cmd++;
    }
    if(strlen(cmd)== 0){
        return 1;
    }
    
    // Check alias
    getaliascmd(&cmd);
    
    // Seperate command into argv array.
    argv[0] = (char*) malloc(strlen(cmd)*sizeof(char));
    for(i=0, j=0, k=0; i<=strlen(cmd); i++, j++){
        if(cmd[i] == ' '){
            while(cmd[i+1] == ' '){
                i++;
            }
            argv[k][j] = '\0';
            j=-1;
            if(strlen(argv[k]) > 0){
                k++;
                argv[k] = (char*) malloc(strlen(cmd)*sizeof(char));
            } 
        } else {
            argv[k][j] = cmd[i];
        }
    }
    if(strlen(argv[k]) == 0){
        argv[k] = NULL;
    } else {
        argv[k+1] = NULL;
    }
    
    // Execute.
    execcmd(cmd, argv);
    
    // Free resources.
    for(i=0; argv[i] != NULL; i++){
        free(argv[i]);
    }
    return 0;
}

int 
execcmd(char *cmd, char** argv){
    int status, cpid;  
    
    if(strcmp(argv[0],"exit")==0){
        exit(0);
    } else if(strcmp(argv[0],"loadprofile")==0 && argv[1] != NULL){
        load_profile(argv[1]);
    } else if(strcmp(argv[0],"alias")==0){
        setmalias(cmd);
    } else if(strcmp(argv[0],"unalias")==0){
        if(argv[1]!=NULL){
            unmalias(argv[1]);
        } else {
            printf("[Error:1]Command [unalias] execute fail\n");
        }
    } else if(strcmp(argv[0],"cd")==0){
        if(argv[1] == NULL || strcmp(argv[1],"~")==0 || strcmp(argv[1],"")==0){
            chdir("/root");
        } else {
            if((status = chdir(argv[1])) != 0){
                printf("[Error:%d]Command [%s] execute fail\n", status, cmd);
            }
        }
    } else {
        if((cpid=fork()) == 0){
            //Execute command directly.
            if(execvp(argv[0], argv)<0){
                exit(1);
            }
        } else {
            waitpid(cpid, &status, 0);
            if (status != 0){
                printf("[Error:%d]Command [%s] execute fail\n", status, cmd);
            } 
        }
    }
    
    return 0;
}

int 
split_semicolon(char *cmd){
    char *str;
    int i, j, argc;
    
    str = (char*) malloc(strlen(cmd)*sizeof(char) + 10);
    for(i=0, j=0, argc=0; i<=strlen(cmd); i++, j++){
        if(cmd[i] == ';'){
            str[j] = '\0';
            build_argv(str, argc);
            argc = 0;
            j = -1;
        } else {
            if(cmd[i] == ' '){
                argc++;
            }
            str[j] = cmd[i];
        }
    }
    build_argv(str, argc);
    
    free(str);
    return 0;
}

void 
precedence_parser(char *cmd){
    char *str;
    void *value;
    int i, j;
    mstack *stk;
    
    initstack(&stk);
    
    str = (char*) malloc(strlen(cmd)*sizeof(char) + 10);
    for(i=0, j=0; i<=strlen(cmd); i++, j++){
        if(cmd[i] == '('){
            str[j] = ';';
            str[j+1] = '\0';
            push(str, stk);     //Push current str into stacks
            str = (char*) malloc(strlen(cmd)*sizeof(char) + 10);       //str point into new string
            j= -1;
        } else if(cmd[i] == ')'){
            str[j] = '\0';
            pop(&value, stk);   //Pop item from stack
            split_semicolon(str);
            free(str);
            str = (char *)value;
            j = strlen(str)-1;
        } else {
            str[j] = cmd[i]; 
        }
    }
    split_semicolon(str);
    
    free(str);
    freestack(stk);
}


