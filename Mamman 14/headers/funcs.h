#ifndef FUNCS_H
#define FUNCS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "globals.h"
#include "errors.h"

#define REGISTER_COUNT 8 /* Number of registers */
#define SAVED_WORDS_COUNT 20 /* Number of saved words */

/**
 * @brief Allocates memory dynamically and checks for allocation failures.
 *
 * This function attempts to allocate memory of the given size.
 * If allocation fails, an error is reported and the program exits.
 *
 * @param size The amount of memory (in bytes) to allocate.
 * @return void* Pointer to the allocated memory, or NULL if allocation fails.
 */
void *handle_malloc(long size);

/**
 * @brief Checks if a given identifier is a reserved word or register name.
 *
 * This function verifies whether the provided identifier is a reserved assembler word
 * (such as `.data`, `.string`, `.entry`, `.extern`) or a register name.
 *
 * @param line The identifier to check.
 * @return int Returns 1 if the identifier is reserved, otherwise 0.
 */
int is_reserved(char *line);

/**
 * @brief Adds a new data node to the data list in sorted order.
 *
 * This function inserts a new data node into the `DataList` in ascending order
 * based on the given memory address.
 *
 * @param list Pointer to the data list.
 * @param address The memory address for the data node.
 * @param value The value to store at the specified address.
 * @param file_name The name of the file for error reporting.
 * @return int Returns 0 on success, otherwise 1 if an error occurs.
 */
int add_data_node(DataList *list, int address, int value, char *file_name);

/**
 * @brief Frees all memory allocated for the data list.
 *
 * This function releases all nodes in the `DataList`, preventing memory leaks.
 *
 * @param list Pointer to the data list.
 */
void free_data_list(DataList *list);

/**
 * @brief Frees all memory allocated for the label list.
 *
 * This function iterates through the label list and frees each label entry.
 *
 * @param head Pointer to the head of the label list.
 */
void free_label_list(label_table *head);

/**
 * @brief Frees all memory allocated for the instruction list.
 *
 * This function iterates through the instruction list and frees each instruction node.
 *
 * @param list Pointer to the instruction list.
 */
void free_instruction_list(InstructionList *list);

/**
 * @brief Initializes an empty data list.
 *
 * Sets the head of the `DataList` to NULL, ensuring it starts as an empty list.
 *
 * @param list Pointer to the data list to initialize.
 */
void init_data_list(DataList *list);


/**
 * @brief Checks if a given label is valid.
 *
 * A label is valid if it starts with a letter, contains only alphanumeric characters,
 * does not exceed the maximum label length, and is not a reserved word.
 *
 * @param label The label to validate.
 * @param file_name The name of the file for error reporting.
 * @param line_number The line number in the source file.
 * @return int Returns 1 if the label is valid, otherwise 0.
 */
int isValidLabel(char *label, char *file_name, int line_number);

/**
 * @brief Generates output files for the assembler.
 *
 * This function creates and writes to `.ob` (object), `.ent` (entry), and `.ext` (external) files.
 * It encodes the assembled program into the correct format.
 *
 * @param file_name The base file name.
 * @param data_list Pointer to the data list.
 * @param label_head Pointer to the head of the label list.
 * @param ICF Final instruction counter value.
 * @param DCF Final data counter value.
 * @return int Returns 0 on success, otherwise 1 if an error occurs.
 */
int build_output_files(char *file_name, DataList *data_list, label_table *label_head, int ICF, int DCF);

/**
 * @brief Adds a suffix to a file name.
 *
 * This function appends a given suffix (such as ".ob", ".ent", ".ext") to a base file name.
 * The returned string must be freed by the caller.
 *
 * @param file_name The base file name.
 * @param suffix The suffix to append.
 * @return char* A newly allocated string with the suffix added.
 */
char *add_suffix(char *file_name, char *suffix);


/**
 * @brief Trims leading and trailing whitespace from a string.
 *
 * This function removes any spaces or tabs at the beginning and end of a string.
 *
 * @param str The input string to trim.
 * @return char* A pointer to the trimmed string.
 */
char *trim_whitespace(char *str);

/**
 * @brief Prints all data stored in the data list.
 *
 * Iterates through the `DataList` and prints each node’s address and value.
 * for debugging purposes.
 *
 * @param list Pointer to the data list.
 */
void print_data_list(DataList *list);

#endif /* FUNCS_H */
