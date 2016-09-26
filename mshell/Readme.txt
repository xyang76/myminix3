1.Compile myshell
# cd [path of myshell]
# make myshell

2.Reg myshell into ash shell env (you can also execute myshell use ./myshell), or put export command in /root/.profile
# export PATH=$PATH:/usr/src/mshell

3.Run myshell (if you do not finish step2, you can use ./myshell command) 
# myshell

4.Set profile
> loadprofile [path of profile, eg, > loadprofile /etc/profile]

5.See environment of myshell
> env

6.Quit myshell(or ctrl+c)
> exit 

7.Clean myshell
# make clean

----------------------myshell command support-------------------- 
> loadprofile [path of profile]
> env						// see myshell environment
> alias hello='ls -al'
> unalias hello
> ls						// command in /bin 
> git pull 					// command in usr/bin
> pwd; (cd /usr/src; (cd ~; pwd) pwd); cd /etc;
 

----------------------myshell error def(errdef.h)---------------------------
* some error will not print in console because they are not fatal error. 
* eg, unalias a name which not defined. 
* see error handler in errdef.c

#define PARENTHESIS_UNMATCH 1
#define SET_ALIAS_ERR 2
#define ALIAS_NOT_EXIST 3
#define OPEN_PROFILE_ERR 4
#define SET_ENV_ERR 5
#define EMPTY_COMMAND 6
#define COMMAND_INCORRECT 7
#define COMMAND_EXECUTE_FAIL 8

-----------------------git repository----------------------------------------
minix src: https://github.com/xyang76/myminix3
shellpath: https://github.com/xyang76/myminix3/tree/master/mshell