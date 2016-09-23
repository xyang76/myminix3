CC=clang
CFLAGS=-I.
DEPS = 

mshell.o: mshell.c
	cc -o mshell.o -c mshell.c

mstack.o: mstack.c
	cc -o mstack.o -c mstack.c

myshell: mshell.o mstack.o
	cc -o myshell mshell.o mstack.o -I.

clean:
	rm myshell mshell.o mstack.o