#include "errdef.h"
#include "stdarg.h"

static int errcode;

void set_error(int ecode){
    errcode = ecode;
}

void if_error(int rv, int ecode){
    if(rv != 0){
        print_error(ecode);
    }
}

void print_error(int ecode, ...){
    va_list ap;
    
    if(ecode == 0){
        ecode = errcode;
    } else {
        errcode = ecode;
    }
    
    switch(ecode){
        case PARENTHESIS_UNMATCH:
            printf("Parenthesis not match, please check parenthesis of command.\n");
            break;
        case SET_ALIAS_ERR:
            printf("Set alias error, please check alias command.\n");
            break;
        case SET_ENV_ERR:
            printf("Set environment error, please modify profile.\n");
            break;
    }
    
    // print more msg.
    va_start(ap, ecode); 
    switch(ecode){
        case OPEN_PROFILE_ERR:
            printf("Can not open profile %s.\n", va_arg(ap, char*));
            break;
        case COMMAND_INCORRECT:
            printf("Command [%s] incorrect.\n", va_arg(ap, char*));
            break;
        case COMMAND_EXECUTE_FAIL:
            printf("Command [%s][return value:%d] execute fail.\n", va_arg(ap, char*), va_arg(ap, int));
    }
    va_end(ap);
}
