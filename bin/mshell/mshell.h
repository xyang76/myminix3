#define MAXCOMMAND 500

int precedence_check(char *cmd);
int evalcmd(char *cmd, int argc);
int splitcmd(char *cmd);
void precedence_parser(char *cmd);
