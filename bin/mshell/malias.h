#define MAXALIASSIZE 100

struct malias{
    struct malias *prev;
    char name[MAXALIASSIZE];
    char aliasname[MAXALIASSIZE];
    struct malias *next;
};

//char *getaliascmd(char *cmd);
char *lookupmalias(char *name);
int set(char *name, char *aliasname);
int setmalias(char *cmd);
int unmalias(char*name);

