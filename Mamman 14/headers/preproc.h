#ifndef PREPROC_H
#define PREPROC_H

#include "globals.h"
#include "funcs.h"
#include "errors.h"
#include "macro_data_structure.h"

/**
 * @brief Preprocesses an assembly file by expanding macros and reformatting the code.
 *
 * This function reads the input `.as` file, processes any macros
 * defined within it, and writes the expanded output to the `.am` file.
 * It also removed any comments and empty lines from the input file,
 * replaces multiple consecutive spaces with a single space and
 * removes leading and trailing spaces from each line.
 *
 * @param file_as The path to the input assembly file (.as).
 * @param file_am The path to the output preprocessed file (.am).
 * @return int Returns 0 on success, or 1 on failure.
 */
int preproc(char* file_as, char* file_am);

/**
 * @brief Checks if a macro name is valid.
 *
 * Validates that the macro name is not empty, too long,
 * starts with a letter, and is not a reserved keyword.
 *
 * @param name The macro name to validate.
 * @param line The line number where the macro is defined (for error reporting).
 * @param file_name The name of the file being processed (for error reporting).
 * @return int Returns 1 if the name is valid, or 0 if it is invalid.
 */
int isValidName(char* name, int line, char* file_name);

/**
 * @brief Removes unnecessary spaces and comments from a line.
 *
 * Processes a single line of assembly code by removing leading,
 * trailing, and consecutive spaces, as well as comments (starting with ';').
 * Quoted strings are preserved.
 *
 * @param line The input line to process.
 * @return char* A dynamically allocated string containing the cleaned line.
 *               Returns NULL if the input line is empty.
 */
char* remove_white_spaces(char* line);

#endif /* PREPROC_H */
