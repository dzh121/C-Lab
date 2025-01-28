# Compiler and flags
CC = gcc
CFLAGS = -Wall -ansi -pedantic

# Target executable
assembler: assembler.o code_translation.o data_structure.o errors.o first_pass.o funcs.o preproc.o second_pass.o tables.o
	$(CC) $(CFLAGS) assembler.o code_translation.o data_structure.o errors.o first_pass.o funcs.o preproc.o second_pass.o tables.o -o assembler

assembler.o: src/assembler.c headers/assembler.h headers/globals.h headers/funcs.h headers/preproc.h headers/first_pass.h headers/second_pass.h headers/errors.h
	$(CC) -c $(CFLAGS) src/assembler.c -o assembler.o

code_translation.o: src/code_translation.c headers/code_translation.h headers/globals.h headers/funcs.h headers/tables.h headers/errors.h
	$(CC) -c $(CFLAGS) src/code_translation.c -o code_translation.o

data_structure.o: src/data_structure.c headers/data_structure.h headers/globals.h headers/funcs.h headers/errors.h
	$(CC) -c $(CFLAGS) src/data_structure.c -o data_structure.o

errors.o: src/errors.c headers/errors.h headers/globals.h
	$(CC) -c $(CFLAGS) src/errors.c -o errors.o

first_pass.o: src/first_pass.c headers/first_pass.h headers/globals.h headers/tables.h headers/funcs.h headers/code_translation.h headers/errors.h
	$(CC) -c $(CFLAGS) src/first_pass.c -o first_pass.o

funcs.o: src/funcs.c headers/funcs.h headers/globals.h headers/errors.h
	$(CC) -c $(CFLAGS) src/funcs.c -o funcs.o

preproc.o: src/preproc.c headers/preproc.h headers/globals.h headers/data_structure.h headers/errors.h headers/funcs.h
	$(CC) -c $(CFLAGS) src/preproc.c -o preproc.o

second_pass.o: src/second_pass.c headers/second_pass.h headers/globals.h headers/code_translation.h headers/tables.h headers/funcs.h headers/errors.h
	$(CC) -c $(CFLAGS) src/second_pass.c -o second_pass.o

tables.o: src/tables.c headers/tables.h headers/funcs.h headers/globals.h headers/assembler.h
	$(CC) -c $(CFLAGS) src/tables.c -o tables.o

clean:
	rm -f *.o *.am *.ob *.ent *.ext assembler
