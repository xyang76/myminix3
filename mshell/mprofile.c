#include "mprofile.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "unistd.h"

static char *profile="profile";

int load_profile(char *name){
    profile = name;
    read_profile(profile);
    return 0;
}

int read_profile(){
    FILE *fp;
	char buff[ENVSIZE];

	if(((fp=fopen(profile, "r")) == NULL)){
    	perror("Cant read profile.");
		return -1;
	}
    
    while(fgets(buff,sizeof(buff),fp) != NULL)
    {
        set_menv(buff);
    }
    return 0;
}

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
            printf("[Error:%d]Can not change dir to HOME\n", rv);
            return -1;
        }
    }
    setenv(name, envvalue, 1);
    return 0;
}
