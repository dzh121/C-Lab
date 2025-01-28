#ifndef DATA_STRUCTURE_H
#define DATA_STRUCTURE_H

#include "globals.h"
#include "funcs.h"
#include "errors.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Linked list */
typedef struct macro_node {
	char *name; /* Name of the macro */
	char *content; /* Content of the macro */
	int line; /* Line number of the macro */
	struct macro_node *next; /* Pointer to the next macro */
} macro_node;

macro_node *build_node(char *name, char *content, int line);
macro_node *search_list(macro_node *head, char *name);
int add_node(macro_node **head, char *name, char *content, int line, char *file_name);
void free_macro_node(macro_node *node);
void free_macro_list(macro_node *head);

#endif
