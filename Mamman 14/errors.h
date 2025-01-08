#ifndef ERRORS_H
#define ERRORS_H

/* File Handling Errors */
#define ERROR_FILE_OPEN_SOURCE "Could not open source file."
#define ERROR_FILE_OPEN_OUTPUT "Could not open output file."
#define ERROR_FAILED_REMOVAL "Failed to remove file."

/* Memory Handling Errors */
#define ERROR_MEMORY_ALLOCATION "Memory allocation failed."
#define ERROR_MEMORY_REALLOCATION "Memory reallocation failed."

/* Macro Handling Errors */
#define ERROR_MACRO_NAME_DUP "Macro name already exist."
#define ERROR_MACRO_NOT_CLOSED "Macro started but not closed with 'mcroend'."
#define ERROR_MACRO_NO_NAME "No macro name."
#define ERROR_MACRO_TOO_LONG "Macro name is too long."
#define ERROR_MACRO_NAME_INVALID "Macro name is invalid."
#define ERROR_MACRO_NAME_RESERVED "Macro name is reserved."
#define ERROR_EXTRANEOUS_TEXT_MCROEND "Extraneous text after 'mcroend'."
#define ERROR_EXTRANEOUS_TEXT_MCRONAME "Extraneous text after macro name."
#define ERROR_PREPROC_FAILED "Preprocessor failed."
/* First Pass Errors */
#define ERROR_LABEL_TOO_LONG "Label is too long."
#define ERROR_LABEL_INVALID "Label is invalid."
#define ERROR_LABEL_RESERVED "Label is reserved."
#define ERROR_LABEL_DUP "Label already exists."

/* Code Translation Errors */
#define ERROR_INSTRUCTION_NOT_FOUND "Instruction not found."
#define ERROR_INVALID_SOURCE_ADDRESSING_MODE "Invalid source addressing mode."
#define ERROR_INVALID_DESTINATION_ADDRESSING_MODE "Invalid destination addressing mode."
#define ERROR_NULL_INSTRUCTION "Instruction is NULL."
#define ERROR_INVALID_OPCODE "Invalid opcode."
#define ERROR_INVALID_FUNCT "Invalid funct."
#define ERROR_INVALID_ADDRESSING_MODE "Invalid addressing mode."
#define ERROR_INVALID_ARE_BITS "Invalid ARE bits."
#define ERROR_INVALID_SOURCE_REGISTER "Invalid source register."
#define ERROR_INVALID_DESTINATION_REGISTER "Invalid destination register."
#define ERROR_INVALID_OPERAND_TYPE "Invalid operand type."
#define ERROR_UNIDENTIFIED_VAR "Unidentified variable"
#define ERROR_ILLEGAL_COMMA "Illegal comma"
#define ERROR_UNDEFINED_COMMAND "Undefined command name"
#define ERROR_INVALID_NUMBER "Invalid parameter - not a number"
#define ERROR_MISSING_OPERAND "Missing operand"
#define ERROR_MULTIPLE_COMMA "Multiple consecutive commas"
#define ERROR_EXTRA_TEXT "Extraneous text after end of command"
#define ERROR_MISSING_COMMA "Missing comma"

void print_internal_error(char *error_message);
void print_ext_error(char *error_message, char *file_name, int line_number);

#endif
