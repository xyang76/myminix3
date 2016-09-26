#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "malias.h"
#include "errdef.h"

struct malias *current = NULL;

/*
 * get alias cmd, eg, if we have alias hello='ls -l', then getalisacmd("hello /etc") return "ls -l /etc".
 */
int getaliascmd(char **cmd){
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
    strcpy(*cmd, aliascmd);
    return 0;
}

/*
 * lookup alias by name.
 */
char *lookupmalias(char *name){
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

/*
 * set alias by cmd, eg, # alias hello='ls -l'.
 */
int setmalias(char *cmd){
    int len = strlen(cmd);
    char *p, *b, *e, name[len], aliname[len];
    
    p=strstr(cmd, "alias");
    if((p=strchr(p, ' ')) == NULL || strchr(p, '=') == NULL){
        print_error(SET_ALIAS_ERR);
        return SET_ALIAS_ERR;
    }
    while(*p==' '){
        p++;
    }
    
    // Get name
    b=name;
    for(;*p != '=';b++,p++){
        *b = *p;
    }
    *b='\0';
    
    // Get alias name
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
    
    return set(name, aliname);
}

/*
 * set alias by name and value.
 */
int set(char *name, char *aliasname){
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
    return 0;
}

/*
 * unalias by its name.
 */
int unmalias(char*name){
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
    
    set_error(ALIAS_NOT_EXIST);
    return ALIAS_NOT_EXIST;
}