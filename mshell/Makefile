CC=clang
CFLAGS=-I.
DEPS = 

main.o: main.c
	cc -o main.o -c main.c

mshell.o: mshell.c
	cc -o mshell.o -c mshell.c

mstack.o: mstack.c
	cc -o mstack.o -c mstack.c

malias.o: malias.c
	cc -o malias.o -c malias.c

mprofile.o: mprofile.c
	cc -o mprofile.o -c mprofile.c

errdef.o: errdef.c
	cc -o errdef.o -c errdef.c

myshell: main.o mshell.o mstack.o malias.o mprofile.o errdef.o
	cc -o myshell main.o mshell.o mstack.o malias.o mprofile.o errdef.o -I.

clean:
	rm myshell main.o mshell.o mstack.o malias.o mprofile.o errdef.o