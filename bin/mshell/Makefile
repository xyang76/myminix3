mshell : mshell.o mstack.o
gcc -o mshell mshell.o mstack.o

test :  test.o
gcc -o test test.o

mstack.o : mstack.c mstack.h
	gcc -c mstack.c -I .

mshell.o : mshell.c mshell.h
	gcc -c mshell.c -I .

test.o : test.c 
	gcc -c test.c -I .
