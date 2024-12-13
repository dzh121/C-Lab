mycomp: mycomp.o input.o complex.o
	gcc -g -Wall -ansi -pedantic mycomp.o input.o complex.o -o mycomp -lm

mycomp.o: mycomp.c input.h
	gcc -c -Wall -ansi -pedantic mycomp.c -o mycomp.o

input.o: input.c complex.o input.h
	gcc -c -Wall -ansi -pedantic input.c -o input.o

complex.o: complex.c complex.h
	gcc -c -Wall -ansi -pedantic complex.c -o complex.o -lm
