# Compiler and flags
CC = gcc
CFLAGS = -Wall -ansi -pedantic
# Target executable
assembler: assembler.o code_translation.o data_structure.o errors.o first_pass.o funcs.o preproc.o second_pass.o tables.o
	$(CC) $(CFLAGS) assembler.o code_translation.o data_structure.o errors.o first_pass.o funcs.o preproc.o second_pass.o tables.o -o assembler

assembler.o: assembler.c assembler.h tables.h funcs.h preproc.h first_pass.h second_pass.h code_translation.h errors.h data_structure.h
	$(CC) -c $(CFLAGS) assembler.c -o assembler.o

code_translation.o: code_translation.c code_translation.h assembler.h funcs.h tables.h errors.h
	$(CC) -c $(CFLAGS) code_translation.c -o code_translation.o

data_structure.o: data_structure.c data_structure.h
	$(CC) -c $(CFLAGS) data_structure.c -o data_structure.o

errors.o: errors.c errors.h
	$(CC) -c $(CFLAGS) errors.c -o errors.o

first_pass.o: first_pass.c first_pass.h assembler.h tables.h funcs.h code_translation.h errors.h
	$(CC) -c $(CFLAGS) first_pass.c -o first_pass.o

funcs.o: funcs.c funcs.h assembler.h tables.h errors.h
	$(CC) -c $(CFLAGS) funcs.c -o funcs.o

preproc.o: preproc.c preproc.h data_structure.h assembler.h funcs.h
	$(CC) -c $(CFLAGS) preproc.c -o preproc.o

second_pass.o: second_pass.c second_pass.h assembler.h tables.h funcs.h errors.h
	$(CC) -c $(CFLAGS) second_pass.c -o second_pass.o

tables.o: tables.c tables.h assembler.h
	$(CC) -c $(CFLAGS) tables.c -o tables.o

clean:
	rm -f *.o assembler