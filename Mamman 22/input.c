#include "input.h"
#include <stdio.h>

#define UNIDENTIFIED_VAR "Unidentified complex variable\n"
#define ILLEGAL_COMMA "Illegal comma\n"
#define UNDEFINED_COMMAND "Undefined command name\n"
#define INVALID_PARM "Invalid parameter - not a number\n"

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
char *commandNames[FUNC_COUNT] = {"read_comp", "print_comp", "add_comp", "sub_comp", "mult_comp_real",
								"mult_comp_img", "mult_comp_comp", "abs_comp", "stop"};

extern complex A, B, C, D, E, F;

void read_line(char line[]){
  	printf("\nEnter a command:\n");

    if(fgets(line, MAX_INPUT, stdin) == NULL){
		printf("EOF\n");
		exit(0);
	}
	printf("You entered: %s\n", line);

}

void process_line(char line[]){
	char* parmNum;
	char *p = line;
	enum commands command;
	p = skipSpaces(p);

	if (*p == '\n' || *p == '\0') {
		/* Empty line */
		return;
	}

	command = getWord(p);

	if (command == -1)
	{
        printf(UNDEFINED_COMMAND);
		return;
	}

	p+=strlen(commandNames[command]);
	removeSpaces(p);

	if (*p == ',')
	{
        printf(ILLEGAL_COMMA);
		return;
	}

	if (command == STOP)
	{
		EXTRANEOUS_TEXT(*p);
		exit(0);
	}

	MISSING_PARM(*p)

	/* *p is the complex variable */
	if (!isValidComplex(*p))
	{
		printf(UNIDENTIFIED_VAR);
		return;
	}

	if (command == ABS_COMP || command == PRINT_COMP)
	{
		EXTRANEOUS_TEXT(*(p+1))
		if (command == ABS_COMP)
			abs_comp(getComplex(*p));
		else if (command == PRINT_COMP)
			print_comp(getComplex(*p));
		return;
	}
	/* continue to read the parameters p[0] == first complex p[1] == comma p[2] == second complex*/
	MISSING_PARM(*(p+1))
	MISSING_COMMA(*(p+1))

	/* two parms are complex */
	if (command == ADD_COMP || command == SUB_COMP || command == MULT_COMP_COMP)
	{
		MISSING_PARM(*(p+2))
		MULTIPLE_COMMA(*(p+2))
		EXTRANEOUS_TEXT(*(p+3))

		if (!isValidComplex(*(p+2)))
		{
			printf(UNIDENTIFIED_VAR);
			return;
		}

		if (command == ADD_COMP)
			add_comp(getComplex(*p), getComplex(*(p+2)));
		else if (command == SUB_COMP)
			sub_comp(getComplex(*p), getComplex(*(p+2)));
		else
			mult_comp_comp(getComplex(*p), getComplex(*(p+2)));

		return;
	}
	/* one parm is complex second is digit*/
	if (command == MULT_COMP_REAL || command == MULT_COMP_IMG)
	{
		MULTIPLE_COMMA(*(p+2))
		if (!isdigit(*(p+2)) && (*(p+2) != '-' || !isdigit(*(p+3))))
		{
			printf(INVALID_PARM);
			return;
		}
		EXTRANEOUS_TEXT(*nextNum(p+2))
		if (command == MULT_COMP_REAL)
			mult_comp_real(getComplex(*p), atof(p+2));
		else
			mult_comp_img(getComplex(*p), atof(p+2));
		return;
	}

	/* both parms are digits */
	if (command == READ_COMP)
	{
		parmNum = nextNum(p+2);
		MISSING_PARM(*(parmNum+1))
		MISSING_PARM(*parmNum)
		MISSING_COMMA(*parmNum)

		parmNum++;


		if (!isdigit(*parmNum) && (*parmNum != '-' || !isdigit(*(parmNum+1))))
		{
			printf(INVALID_PARM);
			return;
		}
		MULTIPLE_COMMA(*parmNum)
		EXTRANEOUS_TEXT(*(nextNum(parmNum)))

		read_comp(getComplex(*p), atof(p+2), atof(parmNum));
	}
}
complex *getComplex(char a) {
	switch (a) {
	case 'A': return &A;
	case 'B': return &B;
	case 'C': return &C;
	case 'D': return &D;
	case 'E': return &E;
	case 'F': return &F;
	default: return NULL;
	}
}

int isValidComplex(char a)
{
	return a >= 'A' && a <= 'F';
}
enum commands getWord(char *p) {
	int i;
	for (i = 0; i < FUNC_COUNT; i++) {
		if (strncmp(p, commandNames[i], strlen(commandNames[i])) == 0) {
			return (enum commands)i;
		}
	}
	return -1; /* Invalid command */
}

char* skipSpaces(char *p){
	while(isspace(*p)){
		p++;
	}
	return p;
}

void removeSpaces(char *p) {
	char *write = p; /* Pointer to write to */
	char *read = p;  /* Pointer to read from */

	while (*read != '\0') {
		if (!isspace(*read)) {
			*write = *read;
			write++;
		}
		read++;
	}
	*write = '\0'; /* Null-terminate the modified string */
}

char *nextNum(char *ptr) {
	int decimalPointEncountered = 0;

	/* Handle optional negative sign */
	if (*ptr == '-') {
		ptr++;
	}

	while (*ptr) {
		if (*ptr == '.') {
			/* Stop if a second decimal point is found or no digit follows */
			if (decimalPointEncountered || !isdigit(*(ptr + 1))) {
				break;
			}
			decimalPointEncountered = 1; /* Mark decimal point encountered */
		} else if (!isdigit(*ptr)) {
			break; /* Stop at the first non-digit character */
		}
		ptr++; /* Move to the next character */
	}

	/* Return the pointer to the first character after the number */
	return ptr;
}


