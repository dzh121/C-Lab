#ifndef SECOND_PASS_H
#define SECOND_PASS_H

#include "globals.h"
#include "tables.h"
#include "errors.h"
#include "funcs.h"
#include "code_translation.h"

/**
 * @brief Executes the second pass of the assembler.
 *
 * The second pass processes the instruction list generated during the first pass
 * while updating the data list with the encoded instructions. It resolves labels
 * and calculates relative addresses for instructions. While also handling entries.
 *
 * @param instruction_list A pointer to the instruction list structure.
 * @param data_list A pointer to the data list structure.
 * @param label_head A pointer to the label table.
 * @param file_name The name of the source file being processed.
 * @return int Returns 0 if the second pass completes without errors, otherwise 1.
 */
int second_pass(InstructionList *instruction_list, DataList *data_list, label_table *label_head, char *file_name);


/**
 * @brief Processes an .entry directive and marks the label as an entry.
 *
 * This function extracts the label from the .entry directive, validates it,
 * and ensures it exists in the label table. If the label is found and is not
 * an extern label, it is marked as ENTRY.
 *
 * @param line The input line containing the .entry directive.
 * @param label_head A pointer to the label table.
 * @param file_name The name of the source file (for error reporting).
 * @param line_count The current line number (for error reporting).
 * @return int Returns 0 if the entry is processed correctly, otherwise 1.
 */
int handle_entry(char *line, label_table *label_head, char *file_name, int line_count);

#endif /* SECOND_PASS_H */
