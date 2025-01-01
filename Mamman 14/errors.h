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


void print_internal_error(char *error_message);
void print_ext_error(char *error_message, char *file_name, int line_number);

#endif
