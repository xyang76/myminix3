#include "mprofile.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "unistd.h"
#include "errdef.h"

static char *profile="profile";

/*
 * load profile by name.
 */
int load_profile(char *name){
    profile = name;
    read_profile(profile);
    return 0;
}

/*
 * read profile.
 */
int read_profile(){
    FILE *fp;
    char buff[ENVSIZE];

    if(((fp=fopen(profile, "r")) == NULL)){
        print_error(OPEN_PROFILE_ERR, profile);
        return OPEN_PROFILE_ERR;
    }
    
    while(fgets(buff,sizeof(buff),fp) != NULL)
    {
        set_menv(buff);
    }
    
    fclose(fp);
    return 0;
}

/*
 * set environment. check it by command # env.
 */
int set_menv(char *buff){
    char name[strlen(buff)], envvalue[strlen(buff)], *p;
    int rv;
    
    if(strstr(buff, "export")!= NULL){
        buff = strstr(buff, "export");
        buff+=6;        //Length of "export "
    }
    
    while(*buff == ' '){
        buff++;
    }
    if(strlen(buff) == 0 || strchr(buff, '=') == NULL){
        return 1;
    }
    
    for(p=name; *buff != '='; p++, buff++){
        *p=*buff;
    }
    *p='\0';
    
    for(p=envvalue, buff++; *buff != '\0'; p++, buff++){
        *p=*buff;
    }
    *p='\0';
    
    if(strcmp(name, "HOME") == 0 || strcmp(name, "home") == 0){
        if((rv = chdir(envvalue)) != 0){
            print_error(REDIRECT_HOME_FAIL, envvalue, rv);
            return REDIRECT_HOME_FAIL;
        }
    }
    if_error(setenv(name, envvalue, 1), SET_ENV_ERR);
    return 0;
}
