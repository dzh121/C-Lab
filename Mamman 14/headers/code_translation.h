#ifndef CODE_TRANSLATION_H
#define CODE_TRANSLATION_H

#include "globals.h"
#include "errors.h"
#include "funcs.h"
#include "tables.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_OPCODE 15
#define MAX_FUNCT 4

/* Macros for error messages with centralized error handling */
#define EXTRANEOUS_TEXT(ch, file_name, line_number) \
if (ch && ch != '\0' && !isspace(ch)) { \
print_ext_error(ERROR_EXTRA_TEXT, file_name, line_number); \
free(inst); \
return -1; \
}

#define MISSING_PARM(ch, file_name, line_number) \
if (!ch) { \
print_ext_error(ERROR_MISSING_OPERAND, file_name, line_number); \
free(inst); \
return -1; \
}

#define MISSING_COMMA(ch, file_name, line_number) \
if (ch != ',') { \
print_ext_error(ERROR_ILLEGAL_COMMA, file_name, line_number); \
free(inst); \
return -1; \
}

#define MULTIPLE_COMMA(ch, file_name, line_number) \
if (ch == ',') { \
print_ext_error(ERROR_MULTIPLE_COMMA, file_name, line_number); \
free(inst); \
return -1; \
}

typedef struct {
    AddressingMode mode;
    int value;
} Operand;

Instruction *find_operation(char *name);
int calculate_words(Instruction *inst, char *file_name, int line_number);

/*
 * Function: build_first_word
 * Description: Builds the first binary word of an instruction based on its structure.
 * Parameters:
 *   - inst: Pointer to the Instruction struct.
 *   - src_mode: Source addressing mode.
 *   - src_reg: Source register.
 *   - dest_mode: Destination addressing mode.
 *   - dest_reg: Destination register.
 *   - are: ARE bits (A=4, R=2, E=1).
 *   - file_name: Name of the source file for error reporting.
 *   - line_number: Line number for error reporting.
 * Returns:
 *   - Encoded 24-bit instruction word as an unsigned int.
 */
unsigned int build_first_word(Instruction *inst, char *file_name, int line_number);

char *getWord(char *line, char *word);
int getNum(char* line, int *num, char *file_name, int line_number);
char *skipSpaces(char *p);
int process_instruction(
    Instruction *inst,
    char *file_name,
    int line_number,
    DataList *data_list,
    label_table *label_head,
    int address);
void test_func();
int parse_instruction(
    char *line,
    InstructionList *instruction_list,
    char *file_name,
    int line_number
);
#endif
