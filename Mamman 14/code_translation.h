#ifndef CODE_TRANSLATION_H
#define CODE_TRANSLATION_H

#include "assembler.h"
#include "errors.h"
#include "funcs.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_OPCODE 15
#define MAX_FUNCT 4
#define MAX_REGISTER 7

/* Macros for error messages with centralized error handling */
#define EXTRANEOUS_TEXT(ch, file_name, line_number) \
if (ch) { \
print_ext_error(ERROR_EXTRA_TEXT, file_name, line_number); \
return -1; \
}

#define MISSING_PARM(ch, file_name, line_number) \
if (!ch) { \
print_ext_error(ERROR_MISSING_OPERAND, file_name, line_number); \
return -1; \
}

#define MISSING_COMMA(ch, file_name, line_number) \
if (ch != ',') { \
print_ext_error(ERROR_ILLEGAL_COMMA, file_name, line_number); \
return -1; \
}

#define MULTIPLE_COMMA(ch, file_name, line_number) \
if (ch == ',') { \
print_ext_error(ERROR_MULTIPLE_COMMA, file_name, line_number); \
return -1; \
}

typedef enum {
    IMMEDIATE = 0,
    DIRECT = 1,
    RELATIVE = 2,
    REGISTER_DIRECT = 3,
    EXTERNAL = 4
} AddressingMode;

typedef struct {
    char *name;       /* Instruction name (e.g., ADD, MOV) */
    int opcode;       /* Bits 23-18: Operation code */
    int funct;        /* Bits 7-3: Sub-function code */
    int src_mode;     /* Bits 17-16: Source addressing mode */
    int src_reg;      /* Bits 15-13: Source register */
    int dest_mode;    /* Bits 12-11: Destination addressing mode */
    int dest_reg;     /* Bits 10-8: Destination register */
    int are;          /* Bits 2-0: A, R, E bits */
    char src_label[MAX_LINE_LENGTH]; /* Label name for source operand */
    char dest_label[MAX_LINE_LENGTH];
} Instruction;

typedef struct {
    AddressingMode mode;
    int value;
} Operand;

Instruction *find_operation(char *name,char *file_name, int line_number);
int calculate_words(Instruction *inst, int src_mode, int dest_mode, char *file_name, int line_number);

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
unsigned int build_first_word(Instruction *inst, int src_mode, int src_reg, int dest_mode, int dest_reg, int are, char *file_name, int line_number);

char *getWord(char *line, char *word);
int getNum(char* line, int *num, char *file_name, int line_number);
char *skipSpaces(char *p);
void test_func();
void save();

#endif
