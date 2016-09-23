#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "unistd.h"
#include "mstack.h"
#include "mshell.h"

int main(int argc, char **argv)
{
    char cmd[MAXCOMMAND];
    
    printf("Welcome to my shell!\n");
    while(1){
        gets(cmd);
        if(precedence_check(cmd) != -1){ 
            precedence_parser(cmd);
        } else {
            printf("incorrect input");
        }
    }
}

int precedence_check(char *cmd){
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

int evalcmd(char *cmd, int argc){
    char *argv[argc+2];
    int i, j, k;
    
    if(strlen(cmd) == 0){
        return 1;
    }
    
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
    
    execvp(argv[0], argv);  //Execute command
    
    for(i=0; argv[i] != NULL; i++){
        free(argv[i]);
    }
    return 1;
}

int splitcmd(char *cmd){
    char *str;
    int i, j, argc;
    
    str = (char*) malloc(strlen(cmd)*sizeof(char) + 10);
    for(i=0, j=0, argc=0; i<=strlen(cmd); i++, j++){
        if(cmd[i] == ';'){
            str[j] = '\0';
            evalcmd(str, argc);
            argc = 0;
            j = -1;
        } else {
            if(cmd[i] == ' '){
                argc++;
            }
            str[j] = cmd[i];
        }
    }
    evalcmd(str, argc);
    
    free(str);
    return 1;
}

void precedence_parser(char *cmd){
    char *str, *value;
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
            pop((char*)&value, stk);   //Pop item from stack
            splitcmd(str);
            free(str);
            str = value;
            j = strlen(value)-1;
        } else {
            str[j] = cmd[i]; 
        }
    }
    splitcmd(str);
    
    free(str);
    freestack(stk);
}

