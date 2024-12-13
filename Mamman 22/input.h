#ifndef INPUT_H
#define INPUT_H

/* Includes necessary libraries: */
#include "complex.h"
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

/* Max input size */
#define MAX_INPUT 100
/* Number of functions */
#define FUNC_COUNT 9

/* Enum representing the commands */
enum commands {
	READ_COMP, PRINT_COMP, ADD_COMP, SUB_COMP, MULT_COMP_REAL, MULT_COMP_IMG, MULT_COMP_COMP, ABS_COMP, STOP
} command;

/*
 * Reads a line of input from the user.
 * Parameters:
 * - char[]: Array to store the input line.
 */
void read_line(char[]);

/*
 * Processes a line of input by parsing and executing the command.
 * Parameters:
 * - char[]: The input line to process.
 */
void process_line(char[]);

/*
 * Skips leading spaces in a string and returns the first non-space character.
 * Parameters:
 * - char*: The input string.
 * Returns:
 * - char*: Pointer to the first non-space character in the string.
 */
char* skipSpaces(char*);

/*
 * Removes all spaces from a string.
 * Parameters:
 * - char*: The input string to modify.
 */
void removeSpaces(char*);

/*
 * Gets the command name from string and returns enum.
 * Parameters:
 * - char*: The input string containing the command.
 * Returns:
 * - enum commands: The parsed command as an enum value.
 */
enum commands getWord(char*);

/*
 * Checks if a character is a valid identifier for a complex number (A-F).
 * Parameters:
 * - char: The character to check.
 * Returns:
 * - int: 1 if valid, 0 otherwise.
 */
int isValidComplex(char);

/*
 * Retrieves a pointer to the complex number by the char(char A to pointer of complex A).
 * Parameters:
 * - char: The identifier of the complex number.
 * Returns:
 * - complex*: Pointer to the complex number.
 */
complex *getComplex(char);

/*
 * Skips to the first pointer after a num value in the string.
 *
 * Parameters:
 * - char*: The input string.
 *
 * Returns:
 * - char*: Pointer to the character immediately after the numeric value.
 */
char *skipNum(char*);

#endif
