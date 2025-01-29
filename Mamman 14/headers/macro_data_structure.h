#ifndef MACRO_DATA_STRUCTURE_H
#define MACRO_DATA_STRUCTURE_H

#include "globals.h"
#include "funcs.h"
#include "errors.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Structure representing a macro in the linked list.
 *
 * This structure holds a macro's name, content, the line number
 * where it was defined, and a pointer to the next macro in the list.
 */
typedef struct macro_node {
    char *name; /* Name of the macro */
    char *content; /* Content of the macro */
    int line; /* Line number of the macro */
    struct macro_node *next; /* Pointer to the next macro */
} macro_node;

/**
 * @brief Creates a new macro node.
 *
 * Allocates memory for a new macro node and initializes it with the given name, content, and line number.
 *
 * @param name The name of the macro.
 * @param content The content of the macro.
 * @param line The line number where the macro is defined.
 * @return macro_node* A pointer to the newly created macro node.
 */
macro_node *build_node(char *name, char *content, int line);


/**
 * @brief Searches for a macro in the linked list.
 *
 * Iterates through the linked list to find a macro by its name.
 *
 * @param head The head of the macro list.
 * @param name The macro name to search for.
 * @return macro_node* A pointer to the found macro node or NULL if not found.
 */
macro_node *search_list(macro_node *head, char *name);


/**
 * @brief Adds a new macro to the linked list.
 *
 * Checks if the macro name already exists before adding it to the list.
 *
 * @param head A pointer to the head of the macro list.
 * @param name The name of the macro.
 * @param content The content of the macro.
 * @param line The line number where the macro is defined.
 * @param file_name The name of the file where the macro is defined.
 * @return int Returns 0 if the macro is added, otherwise 1.
 */
int add_node(macro_node **head, char *name, char *content, int line, char *file_name);

/**
 * @brief Frees a single macro node.
 *
 * Free the memory associated with a macro node.
 *
 * @param node A pointer to the macro node to be freed.
 */
void free_macro_node(macro_node *node);


/**
 * @brief Frees the entire macro list.
 *
 * Iterates through the list and frees each macro node.
 *
 * @param head A pointer to the head of the macro list.
 */
void free_macro_list(macro_node *head);

#endif /* MACRO_DATA_STRUCTURE_H */
