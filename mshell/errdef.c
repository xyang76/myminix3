#include "stdio.h"
#include "errdef.h"
#include "stdarg.h"

static int errcode;

/*
 * Set error code, then we can check out last error by error code.
 */
void set_error(int ecode){
    errcode = ecode;
}


/*
 * If errorcode equals 0, then print error.
 */
void if_error(int rv, int ecode){
    if(rv != 0){
        print_error(ecode);
    }
}

/*
 * Print information base on error code.
 */
void print_error(int ecode, ...){
    va_list ap;
    
    if(ecode == 0){
        ecode = errcode;
    } else {
        errcode = ecode;
    }
    
    va_start(ap, ecode); 
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
        case OPEN_PROFILE_ERR:
            printf("Can not open profile %s.\n", va_arg(ap, char*));
            break;
        case COMMAND_INCORRECT:
            printf("Command [%s] incorrect.\n", va_arg(ap, char*));
            break;
        case COMMAND_EXECUTE_FAIL:
            printf("Command [%s][return value:%d] execute fail.\n", va_arg(ap, char*), va_arg(ap, int));
            break;
    }
    va_end(ap);
}
