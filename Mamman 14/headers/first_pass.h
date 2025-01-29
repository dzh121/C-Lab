#ifndef FIRST_PASS_H
#define FIRST_PASS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "globals.h"
#include "tables.h"
#include "code_translation.h"
#include "funcs.h"
#include "errors.h"

#define DATA_DIRECTIVE_OFFSET 5 /* Offset for the data directive */
#define EXTERN_DIRECTIVE_OFFSET 7 /* Offset for the extern directive */

/**
 * @brief Runs the first pass of the assembler.
 *
 * The first pass scans the input assembly file to parse labels,
 * identify directives (.data, .string, .extern), and collect
 * instructions for further processing. It builds the label table,
 * stores data in the data list, and stores instructions in the
 * instruction list for the second pass.
 *
 * @param file_name The name of the input assembly file.
 * @param data_list A pointer to the data list structure for storing memory data.
 * @param instruction_list A pointer to the instruction list structure.
 * @param label_head A pointer to the head of the label table.
 * @param ICF A pointer to store the final value of the instruction counter.
 * @param DCF A pointer to store the final value of the data counter.
 * @return int Returns 0 on success, or 1 on failure.
 */
int first_pass(char* file_name, DataList* data_list, InstructionList* instruction_list, label_table** label_head,
               int* ICF, int* DCF);

/**
 * @brief Processes the .extern directive.
 *
 * This function parses the .extern directive to add external labels
 * to the label table. It validates the label name and ensures no extra
 * text follows the directive, also it validates the label name.
 *
 * @param line The input line containing the .extern directive.
 * @param label_head A pointer to the head of the label table.
 * @param file_name The name of the input file for error reporting.
 * @param line_count The current line number for error reporting.
 * @return int Returns 0 on success, or 1 on failure.
 */
int handle_extern(char* line, label_table** label_head, char* file_name, int line_count);

/**
 * @brief Processes the .data directive.
 *
 * Parses the .data directive to store integers in the data list. It
 * validates that all the parameters are integers and separated by commas.
 *
 * @param line The input line containing the .data directive.
 * @param DC A pointer to the data counter.
 * @param IC The current instruction counter value.
 * @param data_list A pointer to the data list structure.
 * @param file_name The name of the input file for error reporting.
 * @param line_count The current line number for error reporting.
 * @return int Returns 0 on success, or 1 on failure.
 */
int encode_data(char* line, int* DC, int IC, DataList* data_list, char* file_name, int line_count);

/**
 * @brief Processes the .string directive.
 *
 * Parses the .string directive to store a null-terminated string in
 * the data list. It validates the string format and ensures no extra
 * text follows the directive.
 *
 * @param line The input line containing the .string directive.
 * @param DC A pointer to the data counter.
 * @param IC The current instruction counter value.
 * @param data_list A pointer to the data list structure.
 * @param file_name The name of the input file for error reporting.
 * @param line_count The current line number for error reporting.
 * @return int Returns 0 on success, or 1 on failure.
 */
int encode_string(char* line, int* DC, int IC, DataList* data_list, char* file_name, int line_count);

#endif
