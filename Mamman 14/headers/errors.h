#ifndef ERRORS_H
#define ERRORS_H

#include <stdio.h>

/* File Handling Errors */
#define ERROR_FILE_OPEN_SOURCE "Could not open source file"
#define ERROR_FILE_OPEN_OUTPUT "Could not open output file"
#define ERROR_FAILED_REMOVAL "Failed to remove file"

/* Memory Handling Errors */
#define ERROR_MEMORY_ALLOCATION "Memory allocation failed"
#define ERROR_MEMORY_REALLOCATION "Memory reallocation failed"

/* Macro Handling Errors */
#define ERROR_MACRO_NAME_DUP "Macro name already exist"
#define ERROR_MACRO_NOT_CLOSED "Macro started but not closed with 'mcroend'"
#define ERROR_MACRO_NO_NAME "No macro name"
#define ERROR_MACRO_TOO_LONG "Macro name is too long"
#define ERROR_MACRO_NAME_INVALID "Macro name is invalid"
#define ERROR_MACRO_NAME_RESERVED "Macro name is reserved"
#define ERROR_EXTRANEOUS_TEXT_MCROEND "Extraneous text after 'mcroend'"
#define ERROR_EXTRANEOUS_TEXT_MCRONAME "Extraneous text after macro name"
#define ERROR_EMPTY_FILE "Empty file"
#define ERROR_MACRO_NOT_OPENED "Macro not opened"

/* First Pass Errors */
#define ERROR_LABEL_TOO_LONG "Label is too long"
#define ERROR_LABEL_INVALID "Label is invalid"
#define ERROR_LABEL_RESERVED "Label is reserved"
#define ERROR_LABEL_DUP "Label already exists"
#define ERROR_INVALID_INSTRUCTION "Invalid instruction"
#define ERROR_LABEL_WITH_NO_DIRECTIVE_OR_INSTRUCTION "Label with no directive or instruction"
#define ERROR_INVALID_DATA "Invalid data directive"
#define ERROR_MISSING_WHITE_SPACE "Missing white space after label"
#define ERROR_MISSING_COLON "Missing colon after label"

/* Code Translation Errors */
#define ERROR_INSTRUCTION_NOT_FOUND "Instruction not found"
#define ERROR_INVALID_SOURCE_ADDRESSING_MODE "Invalid source addressing mode"
#define ERROR_INVALID_DESTINATION_ADDRESSING_MODE "Invalid destination addressing mode"
#define ERROR_NULL_INSTRUCTION "Instruction is NULL"
#define ERROR_INVALID_OPCODE "Invalid opcode"
#define ERROR_INVALID_FUNCT "Invalid funct"
#define ERROR_INVALID_ARE_BITS "Invalid ARE bits"
#define ERROR_INVALID_SOURCE_REGISTER "Invalid source register"
#define ERROR_INVALID_OPERAND_TYPE "Invalid operand type"
#define ERROR_ILLEGAL_COMMA "Illegal comma"
#define ERROR_INVALID_NUMBER "Invalid parameter not a number"
#define ERROR_MISSING_OPERAND "Missing operand"
#define ERROR_MULTIPLE_COMMA "Multiple consecutive commas"
#define ERROR_EXTRA_TEXT "Extraneous text after end of command"
#define ERROR_MISSING_COMMA "Missing comma"
#define ERROR_MISSING_EXTERNAL_NAME "Missing extern name"
#define ERROR_STRING_MISSING_QUOTES "Missing quotes in string"
#define ERROR_INVALID_DESTINATION_REGISTER "Invalid destination register"
#define ERROR_INVALID_OPERAND_ADDRESSING_MODE "Invalid operand addressing mode"
#define ERROR_EXTRA_OPERANDS "Extra operands"

/* Second Pass Errors */
#define ERROR_UNDEFINED_SOURCE_LABEL "Undefined source label"
#define ERROR_UNDEFINED_DEST_LABEL "Undefined destination label"
#define ERROR_ENTRY_LABEL_NOT_FOUND "Entry label not found"
#define ERROR_MISSING_ENTRY_NAME "Missing entry name"
#define ERROR_ENTRY_CANNOT_BE_EXTERN "Entry cannot be extern"
#define ERROR_INVALID_OPERAND "Invalid operand"

/* General warnings */
#define WARNING_LABEL_AT_START_ENTRY "Label at the start of .entry is meaningless"
#define WARNING_LABEL_AT_START_EXTERN "Label at the start of .extern is meaningless"

/* General Errors */
#define ERROR_BUILD_OUTPUT_FAILED "Failed to build output files"
#define ERROR_LINE_TOO_LONG "Line is too long"
#define ERROR_NUMBER_OVERFLOW "Number is too large"

/**
 * @brief Prints an internal error message.
 *
 * This function is used for reporting internal program errors that
 * are not related to user input, such as memory allocation failures
 * or unexpected conditions.
 *
 * @param error_message The error message to display.
 */
void print_internal_error(char *error_message);

/**
 * @brief Handles memory overflow errors.
 *
 * This function is called when a memory overflow error is detected.
 * It logs the error details, including the file name where the overflow
 * occurred and the address at which the overflow was detected.
 *
 * @param file_name The name of the file where the memory overflow occurred.
 * @param overflow_address The address at which the memory overflow was detected.
 */
void handle_memory_overflow(char* file_name, int overflow_address);

/**
 * @brief Prints an error message related to a specific file and line.
 *
 * This function reports errors encountered while processing an
 * assembly file. It includes the file name and line number.
 * If the line number is set to -1, the error
 * applies to the entire file rather than a specific line.
 *
 * @param error_message The error message to display.
 * @param file_name The name of the file where the error occurred.
 * @param line_number The line number where the error was found (-1 if the error applies to the entire file).
 */
void print_ext_error(char *error_message, char *file_name, int line_number);

/**
 * @brief Prints a warning message related to a specific file and line.
 *
 * This function is used to display warnings that do not stop assembly
 * but may indicate potential issues in the source code.
 *
 * @param warning_message The warning message to display.
 * @param file_name The name of the file where the warning occurred.
 * @param line_number The line number where the warning was found.
 */
void print_ext_warning(char *warning_message, char *file_name, int line_number);


#endif
