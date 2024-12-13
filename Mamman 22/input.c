#include "input.h"
#include <stdio.h>

/* Error messages */
#define UNIDENTIFIED_VAR "Unidentified complex variable\n"
#define ILLEGAL_COMMA "Illegal comma\n"
#define UNDEFINED_COMMAND "Undefined command name\n"
#define INVALID_PARM "Invalid parameter - not a number\n"

/* Macros for error messages */
#define EXTRANEOUS_TEXT(ch)\
	if (ch){\
		printf("Extraneous text after end of command\n");\
		return;\
	}
#define MISSING_PARM(ch)\
	if (!ch){\
		printf("Missing parameter\n");\
	return;\
}
#define MISSING_COMMA(ch)\
	if (ch != ','){\
		printf("Missing comma\n");\
	return;\
}
#define MULTIPLE_COMMA(ch)\
	if (ch == ','){\
		printf("Multiple consecutive commas\n");\
	return;\
}

/* Functions the user writes */
char *commandNames[FUNC_COUNT] = {"read_comp", "print_comp", "add_comp", "sub_comp", "mult_comp_real",
								"mult_comp_img", "mult_comp_comp", "abs_comp", "stop"};

/* Complex variables from mycomp */
extern complex A, B, C, D, E, F;

void read_line(char line[]){
	printf("--------------------------------------\n");
  	printf("Enter a command:\n");

    if(fgets(line, MAX_INPUT, stdin) == NULL){
		printf("Reached EOF\n");
		exit(0);
	}
	printf("You entered: %s", line);
}

void process_line(char line[]){
	/* Process the input line */
	char* parmNum;
	char *p = line;
	/* Skip leading spaces */
	p = skipSpaces(p);

	if (*p == '\n' || *p == '\0') {
		/* Empty line */
		return;
	}

	/* Get the command */
	command = getWord(p);

	if (command == -1)
	{
		/* Invalid command */
        printf(UNDEFINED_COMMAND);
		return;
	}

	/* Skip the command name */
	p+=strlen(commandNames[command]);
	/* Skip all the spaces */
	removeSpaces(p);

	/* Current command is command_name shouldn't be a comma*/
	if (*p == ',')
	{
		/* Illegal comma */
        printf(ILLEGAL_COMMA);
		return;
	}

	/* Check if the command is STOP (The only one word command)*/
	if (command == STOP)
	{
		/* Check for extraneous text (if there is text after the command name)*/
		EXTRANEOUS_TEXT(*p);
		printf("\nGoodbye!\n");
		exit(0);
	}

	/* Check for missing parameters we know we need now the first parm if there is none then missing parm */
	MISSING_PARM(*p)

	/* if there is text it needs to be a complex number (command_name (A-F)) */
	if (!isValidComplex(*p))
	{
		/* Unidentified complex variable */
		printf(UNIDENTIFIED_VAR);
		return;
	}

	/* ABS and PRINT need one parm */
	if (command == ABS_COMP || command == PRINT_COMP)
	{
		/* Check for extraneous text (if there is text after the complex variable)*/
		EXTRANEOUS_TEXT(*(p+1))

		/* Do the function */
		if (command == ABS_COMP)
			abs_comp(getComplex(*p));
		else if (command == PRINT_COMP)
			print_comp(getComplex(*p));
		/* We are done */
		return;
	}

	/* continue to read the parameters command_name (A-F), (number/complex), (number/complex) where P is at the first complex*/
	/* If there is nothing after the complex variable then missing parm */
	MISSING_PARM(*(p+1))
	/* If there isnt a comma then missing comma */
	MISSING_COMMA(*(p+1))

	/* The functions that both params are complex numbers */
	if (command == ADD_COMP || command == SUB_COMP || command == MULT_COMP_COMP)
	{
		/* Check there is something after the comma */
		MISSING_PARM(*(p+2))
		/* Check to see that there aren't two commas */
		MULTIPLE_COMMA(*(p+2))
		/* Check for extraneous text (if there is text after the second complex variable)*/
		EXTRANEOUS_TEXT(*(p+3))

		/* Check if the second parm is a complex number */
		if (!isValidComplex(*(p+2)))
		{
			/* Unidentified complex variable */
			printf(UNIDENTIFIED_VAR);
			return;
		}

		/* Do the function p[0]=first complex, p[2]=second complex*/
		if (command == ADD_COMP)
			add_comp(getComplex(*p), getComplex(*(p+2)));
		else if (command == SUB_COMP)
			sub_comp(getComplex(*p), getComplex(*(p+2)));
		else
			mult_comp_comp(getComplex(*p), getComplex(*(p+2)));
		/* We are done */
		return;
	}

	/* Functions that the first parm is complex then a number  */
	if (command == MULT_COMP_REAL || command == MULT_COMP_IMG)
	{
		/* Check there is something after the comma */
		MISSING_PARM(*(p+2))
		/* Check to see that there aren't two commas */
		MULTIPLE_COMMA(*(p+2))

		/* Check if the second parm is a number or "-" and then a number */
		if (!isdigit(*(p+2)) && (*(p+2) != '-' || !isdigit(*(p+3))))
		{
			/* Invalid parameter */
			printf(INVALID_PARM);
			return;
		}
		/* Check for extraneous text (if there is text after the second parameter)*/
		EXTRANEOUS_TEXT(*skipNum(p+2))

		/* Do the function atof the second number because the number is to the end */
		if (command == MULT_COMP_REAL)
			mult_comp_real(getComplex(*p), atof(p+2));
		else
			mult_comp_img(getComplex(*p), atof(p+2));
		/* We are done */
		return;
	}

	/* This function both params are digits */
	if (command == READ_COMP)
	{
		/* Check there is something after the comma */
		MISSING_PARM(*(p+2))
		/* Check to see that there aren't two commas */
		MULTIPLE_COMMA(*(p+2))

		/* Get the first char after the first number */
		parmNum = skipNum(p+2);
		/* Check there is something after the number*/
		MISSING_PARM(*parmNum)
		/* Check to see that there are comma after the first number */
		MISSING_COMMA(*parmNum)

		/* Get the first char after the comma */
		parmNum++;
		/* Check if the second parm is a number or "-" and then a number */
		if (!isdigit(*parmNum) && (*parmNum != '-' || !isdigit(*(parmNum+1))))
		{
			printf(INVALID_PARM);
			return;
		}
		/* Check to see that there aren't two commas */
		MULTIPLE_COMMA(*parmNum)
		/* Check for extraneous text (if there is text after the second number)*/
		EXTRANEOUS_TEXT(*(skipNum(parmNum)))

		/* Do the function */
		read_comp(getComplex(*p), atof(p+2), atof(parmNum));
		/* We are done */
		return;
	}
}
complex *getComplex(char a) {
	/* Return a pointer to the complex variable corresponding to the character */
	switch (a) {
	case 'A': return &A;
	case 'B': return &B;
	case 'C': return &C;
	case 'D': return &D;
	case 'E': return &E;
	case 'F': return &F;
	default: return NULL; /* Invalid complex variable */
	}
}

int isValidComplex(char a)
{
	/* Check if the character is a valid complex variable A-F */
	return a >= 'A' && a <= 'F';
}

enum commands getWord(char *p) {
	int i;
	/* Compare the input string with the command names */
	for (i = 0; i < FUNC_COUNT; i++) {
		if (strncmp(p, commandNames[i], strlen(commandNames[i])) == 0) {
			return i; /* Return the command index */
		}
	}
	return -1; /* Invalid command */
}

char* skipSpaces(char *p){
	/* Skip leading spaces */
	while(isspace(*p)){
		p++;
	}
	return p;
}

void removeSpaces(char *p) {
	char *write = p; /* Pointer to write to */
	char *read = p;  /* Pointer to read from */

	while (*read != '\0') {
		/* Copy non-space characters */
		if (!isspace(*read)) {
			*write = *read; /* Copy the character */
			write++; /* Move the write pointer */
		}
		read++; /* Move the read pointer */
	}
	*write = '\0'; /* Null-terminate the modified string */
}

char *skipNum(char *ptr) {
	int decimalPointEncountered = 0;

	/* Handle optional negative sign */
	if (*ptr == '-') {
		ptr++;
	}

	/* Skip the digits and the optional decimal point */
	while (*ptr) {
		if (*ptr == '.') {
			/* Stop if a second decimal point is found or no digit follows */
			if (decimalPointEncountered || !isdigit(*(ptr + 1))) { break;}

			decimalPointEncountered = 1; /* Mark decimal point encountered */
		} else if (!isdigit(*ptr)) {
			break; /* Stop at the first non-digit character */
		}
		ptr++; /* Move to the next character */
	}

	/* Return the pointer to the first character after the number */
	return ptr;
}


