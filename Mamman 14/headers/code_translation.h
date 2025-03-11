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

#define MAX_OPCODE 15 /* Maximum opcode value (4-bit encoding) */
#define MAX_FUNCT 4   /* Maximum funct value (2-bit encoding) */

#define OPCODE_RTS 14 /* RTS opcode value */
#define OPCODE_STOP 15 /* STOP opcode value */
#define OPCODE_CLR 5 /* CLR, NOT, DEC opcode value */
#define OPCODE_JMP 9 /* JMP, BNE, JSR opcode value */
#define OPCODE_RED 12 /* RED opcode value */
#define OPCODE_PRN 13 /* PRN opcode value */


#define MAX_IMMEDIATE_VALUE 1048575 /* 21-bit signed integer range 3 bits for ARE */
#define MIN_IMMEDIATE_VALUE -1048576 /* 21-bit signed integer range 3 bits for ARE */

#define MAX_DATA_VALUE 8388607  /* Maximum data value (24 bits) */
#define MIN_DATA_VALUE -8388608 /* Minimum data value (24 bits) */


/*
 * Macros for centralized error handling.
 * These macros ensure errors are reported consistently throughout the assembler.
 */

/* Checks for extraneous text after an instruction */
#define EXTRANEOUS_TEXT(ch, file_name, line_number) \
if (ch && ch != '\0' && !isspace(ch)) { \
    print_ext_error(ERROR_EXTRA_TEXT, file_name, line_number); \
    free(inst); \
    return -1; \
}

/* Checks if an operand is missing */
#define MISSING_PARM(ch, file_name, line_number) \
if (!ch) { \
    print_ext_error(ERROR_MISSING_OPERAND, file_name, line_number); \
    free(inst); \
    return -1; \
}

/* Checks if a comma is missing between operands */
#define MISSING_COMMA(ch, file_name, line_number) \
if (ch != ',') { \
    print_ext_error(ERROR_ILLEGAL_COMMA, file_name, line_number); \
    free(inst); \
    return -1; \
}

/* Checks for multiple consecutive commas, which is invalid */
#define MULTIPLE_COMMA(ch, file_name, line_number) \
if (ch == ',') { \
    print_ext_error(ERROR_MULTIPLE_COMMA, file_name, line_number); \
    free(inst); \
    return -1; \
}

/**
 * @brief Structure representing an operand in an instruction.
 *
 * This structure holds the addressing mode of an operand and its value.
 */
typedef struct {
    AddressingMode mode; /* Addressing mode of the operand */
    int value; /* Value of the operand */
} Operand;


/**
 * @brief Finds an operation based on its name.
 *
 * Searches the instruction table for the given operation name and returns
 * a pointer to the corresponding instruction if found.
 *
 * @param name The name of the operation to search for.
 * @return Instruction* Pointer to the found instruction, or NULL if not found.
 */
Instruction *find_operation(char *name);

/**
 * @brief Calculates the number of words an instruction will use in memory.
 *
 * This function determines the number of additional words required
 * based on the addressing modes of the source and destination operands.
 *
 * @param inst Pointer to the instruction being analyzed.
 * @param file_name Name of the source file (for error reporting).
 * @param line_number Line number of the instruction in the source file.
 * @return int The number of words required for the instruction.
 */
int calculate_words(Instruction *inst, char *file_name, int line_number);

/**
 * @brief Builds the first word of an instruction.
 *
 * Encodes the opcode, addressing modes, registers, funct, and ARE bits
 * into a single 24-bit machine code word.
 *
 * @param inst Pointer to the instruction to encode.
 * @param file_name Name of the source file (for error reporting).
 * @param line_number Line number of the instruction in the source file.
 * @return unsigned int The encoded first word of the instruction.
 */
unsigned int build_first_word(Instruction *inst, char *file_name, int line_number);

/**
 * @brief Extracts the next word from a line of assembly code.
 *
 * Reads a word from the line and stores it in the provided buffer.
 * Stops at spaces, commas, or end-of-line characters.
 *
 * @param line Pointer to the current position in the input string.
 * @param word Pointer to the buffer where the extracted word will be stored.
 * @return char* Pointer to the next position in the input string after the extracted word.
 */
char *getWord(char *line, char *word);

/**
 * @brief Extracts a numeric value from a line of assembly code.
 *
 * Parses an integer from the given line and stores it in the provided variable.
 * Ensures that the extracted value is a valid number.
 *
 * @param line Pointer to the input string containing the number.
 * @param num Pointer to an integer where the extracted value will be stored.
 * @param file_name Name of the source file (for error reporting).
 * @param line_number Line number of the instruction in the source file.
 * @return int The number of characters consumed while parsing the number.
 */
int getNum(char *line, int *num, char *file_name, int line_number);

/**
 * @brief Skips leading spaces in a string.
 *
 * Advances the pointer past any leading whitespace characters and returns
 * the updated pointer position.
 *
 * @param p Pointer to the input string.
 * @return char* Pointer to the first non-space character in the string.
 */
char *skipSpaces(char *p);

/**
 * @brief Encodes an operand into a machine code word.
 *
 * Converts an operand value and addressing mode into a 24-bit machine code word.
 *
 * @param operand The value of the operand to encode.
 * @param type The addressing mode of the operand.
 * @param file_name Name of the source file (for error reporting).
 * @param line_number Line number of the instruction in the source file.
 * @return unsigned int The encoded machine code word representing the operand.
 */
unsigned int encode_operand(int operand, int type, char *file_name, int line_number);

/**
 * @brief Processes an instruction and encodes it into machine code.
 *
 * Converts an instruction into its corresponding machine code representation
 * and adds it to the data list.
 *
 * @param inst Pointer to the instruction to process.
 * @param file_name Name of the source file (for error reporting).
 * @param line_number Line number of the instruction in the source file.
 * @param data_list Pointer to the list where the instruction's machine code will be stored.
 * @param label_head Pointer to the label table for resolving labels.
 * @param address The memory address where the instruction will be stored.
 * @return int Returns 0 if processing is successful, otherwise 1.
 */
int process_instruction(Instruction *inst, char *file_name, int line_number, DataList *data_list,
                        label_table *label_head, int address);

/**
 * @brief Parses an instruction from a line of assembly code.
 *
 * Extracts the instruction name, operands, and addressing modes from the given line
 * and adds the parsed instruction to the instruction list.
 *
 * @param line Pointer to the input string containing the instruction.
 * @param instruction_list Pointer to the instruction list where the parsed instruction will be stored.
 * @param file_name Name of the source file (for error reporting).
 * @param line_number Line number of the instruction in the source file.
 * @return int Returns 0 if parsing is successful, otherwise 1.
 */
int parse_instruction(char *line, InstructionList *instruction_list, char *file_name, int line_number);

/**
 * @brief Parses an operand from a line of assembly code.
 *
 * Extracts the addressing mode and value of an operand from the given line
 * and stores the parsed data in the provided variables.
 *
 * @param line Pointer to the input string containing the operand.
 * @param mode Pointer to the variable where the addressing mode will be stored.
 * @param operand Pointer to the variable where the operand value will be stored.
 * @param label Pointer to the buffer where the operand label will be stored.
 * @param file_name Name of the source file (for error reporting).
 * @param line_number Line number of the instruction in the source file.
 * @return int Returns 0 if parsing is successful, otherwise 1.
 */
int parse_operand(char **line, int *mode, int *operand, char *label, char *file_name, int line_number);


/**
 * @brief Validates the addressing modes of an instruction.
 *
 * Checks the addressing modes of the source and destination operands
 * in an instruction to ensure they are valid and compatible.
 *
 * @param inst Pointer to the instruction to validate.
 * @param file_name Name of the source file (for error reporting).
 * @param line_number Line number of the instruction in the source file.
 * @return int Returns 0 if the addressing modes are valid, otherwise 1.
 */
int validate_addressing_modes(Instruction *inst, char *file_name, int line_number);


#endif
