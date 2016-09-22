#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "mstack.h"

int main(int argc, char **argv)
{
    char *cmd = "aa(ls (a1)-l);((a4-(a2)1a5)(a3))as -l;";

    if(precedence_check(cmd) != -1){ 
        precedence_parser(cmd);
    } else {
        printf("incorrect input");
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

int evalcmd(char *cmd){
    if(strlen(cmd) == 0){
        return 1;
    }
    
    printf("%s\n", cmd);
    return 1;
}

int splitcmd(char *cmd){
    char *str;
    int i, j;
    
    str = (char*) malloc(strlen(cmd)*sizeof(char) + 10);
    for(i=0, j=0; i<=strlen(cmd); i++, j++){
        if(cmd[i] == ';'){
            str[j] = '\0';
            evalcmd(str);
            j = -1;
        } else {
            str[j] = cmd[i];
        }
    }
    evalcmd(str);
    
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
            pop(&value, stk);   //Pop item from stack
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

