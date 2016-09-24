#define MAXCOMMAND 500
#define MAXPPATH 100

int precedence_check(char *cmd);
int build_argv(char *cmd, int argc);
int split_semicolon(char *cmd);
int execcmd(char *cmd, char** argv);
void precedence_parser(char *cmd);
