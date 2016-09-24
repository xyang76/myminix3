#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "string.h"
#include "eval.h"

char *
padvance(char **path, char *name)
{
	char *p, *q;
	char *start;
	int len;

	if (*path == NULL)
		return NULL;
	start = *path;
	for (p = start ; *p && *p != ':' && *p != '%' ; p++);
	len = p - start + strlen(name) + 2;	/* "2" is for '/' and '\0' */
	while (stackblocksize() < len)
		growstackblock();
	q = stackblock();
	if (p != start) {
		memcpy(q, start, p - start);
		q += p - start;
		*q++ = '/';
	}
	strcpy(q, name);
	
	if (*p == '%') {
		pathopt = ++p;
		while (*p && *p != ':')  p++;
	}
	if (*p == ':')
		*path = p + 1;
	else
		*path = NULL;
	return stalloc(len);
}

int main(int argc, char **argv)
{
    char * argv2[] ={"ls",0};
    char * path1 = "/bin";
    char * path2 = "/usr/bin";
    char *envp[]={"PATH=/bin", NULL};   
    printf("Executing...\n");
    printf("value=%d", chdir(".."));
    printf("Is exist %s...\n", padvance(&path1, "ls"));
    printf("Is exist %s...\n", padvance(&path2, "ls"));
}
