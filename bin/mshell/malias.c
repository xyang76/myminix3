#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "malias.h"


struct malias *current = NULL;

int
getaliascmd(char **cmd){
    int i=0;
    char *p, *c, aliascmd[strlen(*cmd)+MAXALIASSIZE];
    
    p = aliascmd;
    c = *cmd;
    while(*c==' '){
        c++;
    }
    
    for(; *c != '\0' && *c != ' '; c++, p++){
        *p = *c;
    }
    *p = '\0';
    strcpy(aliascmd, lookupmalias(aliascmd));
    p = aliascmd + strlen(aliascmd);
    for(; *c != '\0'; c++, p++){
        *p = *c;
    }
    *p = '\0';
    printf("SET[%s] ", aliascmd);
    strcpy(*cmd, aliascmd);
    return 0;
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
    *b='\0';
    printf("ali: %s\n", aliname);
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
        printf("\n%s\nSet %s,%s\n", aliasname, current->name, current->aliasname);
    } else {
        printf("\n%s\nSet %s,%s[/NULL]\n", aliasname, current->name, current->aliasname);
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