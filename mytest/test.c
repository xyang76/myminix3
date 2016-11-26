#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lib.h>    
#include <unistd.h>
#include <testhelper.h>

/**************************************************************
 *       test undelete
 *************************************************************/
int main()
{
	FILE *fp;
	int rv;
	rv = rcmkdir("/usr/src/mytest/t1", 0777);
	ASSERT_EQUAL(rv, 0);
	rv = chdir("/usr/src/mytest/t1");
	ASSERT_EQUAL(rv, 0);
	fp = fopen("file.txt","w+");
	if(fp == NULL) printf("fopen error\n");
	fclose(fp);
	rv = remove("file.txt");
	ASSERT_EQUAL(rv, 0);
	rv = fundelete("file.txt");
	TEST_EQUAL(rv, 0, "undelete should be success.");
	rv = fundelete("filenono.txt");
	TEST_EQUAL(rv, ENOENT, "can not undelete a file not exist.");
	return 0;
;}