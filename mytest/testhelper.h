#include <stdio.h>
#include <stdlib.h>
#include "sys/errno.h"
#include <unistd.h>

#define TRUE 1
#define FALSE 0

static inline int ASSERT_EQUAL(int result, int expect)
{
    if(result == expect){
        return TRUE;
    }
    return FALSE;
}

static inline void TEST_EQUAL(int result, int expect, char *msg){
    if(result == expect){
        printf("TEST [%s] OK.\n", msg);
    } else {
        printf("TEST [%s] Error, expect %d, result %d\n", msg, expect, result);
        abort();
    }
}

static inline void TEST_GREATER(int val1, int val2, char *msg){
    if(val1 > val2){
        printf("TEST [%s] OK.\n", msg);
    } else {
        printf("TEST [%s] Error, value1 %d, value2 %d\n", msg, val1, val2);
        abort();
    }
}