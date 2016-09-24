#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "unistd.h"
#include "errno.h"
#include "sys/types.h" 
#include "sys/wait.h"
#include "mstack.h"
#include "malias.h"
#include "mshell.h"

int 
main(int argc, char **argv)
{
    char cmd[MAXCOMMAND], path[MAXPPATH];
    
    printf("Welcome to my shell!");
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
    char *argv[argc+2];
    int i, j, k;
    
    // Check empty command, if command is empty, then return 1.
    for(i=0; i<=strlen(cmd); i++){
        if(cmd[i] != ' '){
            break;
        }
    }
    if(strlen(cmd) - i == 0){
        return 1;
    }
    
    // Seperate command into argv array.
    argv[0] = (char*) malloc(strlen(cmd)*sizeof(char));
    for(j=0, k=0; i<=strlen(cmd); i++, j++){
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
    return 1;
}

int 
execcmd(char *cmd, char** argv){
    int status, cpid;
    strcpy(argv[0], lookupalias(argv[0]));    
    
    if(strcmp(argv[0],"exit")==0){
        exit(0);
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

    return 1;
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
    return 1;
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


