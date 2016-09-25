#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "malias.h"


struct malias *current = NULL;

char *
getaliascmd(char *cmd){
    int i=0;
    char *p, *b, aliascmd[strlen(cmd)+MAXALIASSIZE];
    
    while(*cmd==' '){
        cmd++;
    }
    p = aliascmd;
    for(; *cmd != '\0' && *cmd != ' '; cmd++, p++){
        *p = *cmd;
    }
    *p = '\0';
    strcpy(aliascmd, lookupmalias(aliascmd));
    p = aliascmd + strlen(aliascmd);
    for(; *cmd != '\0'; cmd++, p++){
        *p = *cmd;
    }
    *p = '\0';
    
    return aliascmd;
}

char *
lookupmalias(char *name){
    struct malias *al;
    char *aliasname;
    
    aliasname = name;
    for(al=current; al != NULL; al=al->prev){
        if(strcmp(al->name, aliasname) == 0){
            aliasname = al->aliasname;
        }
    }
    return aliasname;
}

int 
setmalias(char *cmd){
    int len = strlen(cmd);
    char *p, *b, *e, name[len], aliname[len];
    
    p=strstr(cmd, "alias");
    if((p=strchr(p, ' ')) == NULL || strchr(p, '=') == NULL){
        printf("Incorrect input");
        return 1;
    }
    while(*p==' '){
        p++;
    }
    b=name;
    for(;*p != '=';b++,p++){
        *b = *p;
    }
    b=aliname;
    p++;
    if(*p == '\'' || *p == '"'){
        p++;
    }
    e=cmd+strlen(cmd);
    while(*e == ' ' || *e == '\'' || *e == '"' || *e=='\0'){
        e--;
    }
    
    for(e++;p != e; p++, b++){
        *b=*p;
    }
    return set(name, aliname);
}

int 
set(char *name, char *aliasname){
    struct malias *al;
    
    al = (struct malias *)malloc(sizeof(struct malias));
    strcpy(al->name, name);
    strcpy(al->aliasname, aliasname);
    al->next = NULL;
    
    if(current != NULL){
        current->next = al;
    } 
    al->prev = current;
    current = al;
    if(current->prev==NULL){
        printf("Set %s,%s\n", current->name, current->aliasname);
    } else {
        printf("Set %s,%s[/NULL]\n", current->name, current->aliasname);
    }
    return 0;
}

int 
unmalias(char*name){
    char *p;
    struct malias *al;
    
    p=name+strlen(name)-1;
    while(*p == ' '){
        *p='\0';
        p--;
    }
    for(al=current; al != NULL; al=al->prev){
        if(strcmp(al->name, name) == 0){
            if(al->prev != NULL){
                al->prev->next = al->next;
            } 
            if(al->next != NULL){
                al->next->prev = al->prev;
            }
            if(al->prev==NULL && al->next==NULL && al == current){
                current=NULL;
            }
            free(al);
            al = NULL;
            return 0;
        }
    }
    return 1;
}