CC=clang
CFLAGS=-I.
DEPS = 

mshell.o: mshell.c
	cc -o mshell.o -c mshell.c

mstack.o: mstack.c
	cc -o mstack.o -c mstack.c

malias.o: malias.c
	cc -o malias.o -c malias.c

myshell: mshell.o mstack.o malias.o
	cc -o myshell mshell.o mstack.o malias.o -I.

clean:
	rm myshell mshell.o mstack.o malias.o