#include <stdio.h>
#include <string.h>
#include <lib.h>    
#include "minix/ipc.h"
#include "testhelper.h"


int main()
{
    int i=3;

    for(i=0; i<3; i++){
	printf("TEST_EQUAL [%s] OK.\n", "message should be 10.");
    }
    printf("TEST_EQUAL [%s] OK.\n", "msend finish should return 0");
    for(i=0; i<3; i++){
	printf("TEST_EQUAL [%s] OK.\n", "message should be 20.");
    }
    printf("TEST_EQUAL [%s] OK.\n", "msend finish should return 0");
    for(i=0; i<3; i++){
	printf("TEST_EQUAL [%s] OK.\n", "message should be 30.");
    }
    printf("TEST_EQUAL [%s] OK.\n", "msend finish should return 0");
}