#define PARENTHESIS_UNMATCH 1
#define SET_ALIAS_ERR 2
#define ALIAS_NOT_EXIST 3
#define OPEN_PROFILE_ERR 4
#define SET_ENV_ERR 5
#define EMPTY_COMMAND 6
#define COMMAND_INCORRECT 7
#define COMMAND_EXECUTE_FAIL 8

void if_error(int rv, int ecode);
void set_error(int ecode);
void print_error(int ecode);