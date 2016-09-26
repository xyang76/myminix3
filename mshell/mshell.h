#define MAXCOMMAND 500
#define MAXPPATH 100

int precedence_check(char *cmd);
int build_argv(char *cmd, int argc);
int split_semicolon(char *cmd);
int execcmd(char *cmd, char** argv);
int precedence_parser(char *cmd);
void precompile(char *cmd);
void sigint_handler(int signal);
