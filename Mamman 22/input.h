#ifndef INPUT_H
#define INPUT_H

#include "complex.h"
#include <ctype.h>
#include <string.h>
#include <stdlib.h> /* for atof */

#define MAX_INPUT 100
#define FUNC_COUNT 9

enum commands {
	READ_COMP, PRINT_COMP, ADD_COMP, SUB_COMP, MULT_COMP_REAL, MULT_COMP_IMG, MULT_COMP_COMP, ABS_COMP, STOP
};

void read_line(char[]);
void process_line(char[]);
char* skipSpaces(char*);
void removeSpaces(char*);
enum commands getWord(char*);
int isValidComplex(char);
complex *getComplex(char);
char *nextNum(char*);

#endif
