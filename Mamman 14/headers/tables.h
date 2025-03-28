#ifndef TABLES_H
#define TABLES_H

#include "globals.h"
#include "funcs.h"
#include "string.h"
#include "errors.h"

/**
 * @brief Creates a new label entry.
 *
 * Allocates memory for a new label, assigns the provided name, address,
 * line number, and type. Initializes the address list and next pointer.
 *
 * @param name The name of the label.
 * @param addr The memory address associated with the label.
 * @param line The line number where the label was defined.
 * @param type The type of label (CODE, DATA, ENTRY, EXTERN).
 * @return label_table* A pointer to the newly created label node.
 */
label_table *build_label(char *name, int addr, int line, LabelType type);

/**
 * @brief Adds a label to the label table.
 *
 * Checks if the label already exists in the label table. If not, creates a new
 * label node and appends it to the linked list.
 *
 * @param head A pointer to the head of the label table.
 * @param name The name of the label.
 * @param addr The memory address associated with the label.
 * @param line The line number where the label was defined.
 * @param type The type of label (CODE, DATA, ENTRY, EXTERN).
 * @param file_name The name of the file for error reporting.
 * @return int Returns 0 if the label is added, otherwise 1.
 */
int add_label_list(label_table **head, char *name, int addr, int line, LabelType type, char *file_name);

/**
 * @brief Searches for a label in the label table.
 *
 * Iterates through the label table to find a label by its name.
 *
 * @param head A pointer to the head of the label table.
 * @param name The name of the label to search for.
 * @return label_table* A pointer to the found label node, or NULL if not found.
 */
label_table *search_label_list(label_table *head, char *name);

/**
 * @brief Creates a new address node.
 *
 * Allocates memory for an address node and initializes it with the provided address.
 *
 * @param addr The memory address to store.
 * @return ADDRESS_LIST* A pointer to the newly created address node.
 */
ADDRESS_LIST *build_address_node(int addr);

/**
 * @brief Adds an address to an address list.
 *
 * Creates a new address node and appends it to the address list.
 *
 * @param head A pointer to the head of the address list.
 * @param addr The memory address to add.
 */
void add_address_node(ADDRESS_LIST **head, int addr);

/**
 * @brief Adds an address to a label's address list.
 *
 * Appends an address to the label's address list, used for EXTERN labels.
 *
 * @param node The label node to which the address should be added.
 * @param addr The memory address to associate with the label.
 */
void add_address_to_label(label_table *node, int addr);


/**
 * @brief Initializes an empty instruction list.
 *
 * Sets the head and tail pointers of the instruction list to NULL.
 *
 * @param list A pointer to the instruction list to initialize.
 */
void init_instruction_list(InstructionList *list);

/**
 * @brief Adds an instruction to the instruction list.
 *
 * Allocates memory for a new instruction node, copies the instruction data,
 * and appends it to the end of the instruction list.
 *
 * @param list A pointer to the instruction list.
 * @param inst A pointer to the instruction to be added.
 */
void add_instruction(InstructionList *list, Instruction *inst);

#endif
