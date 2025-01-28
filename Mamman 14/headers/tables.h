#ifndef TABLES_H
#define TABLES_H

#include "globals.h"
#include "funcs.h"
#include "string.h"
#include "errors.h"

/* Function Prototypes */
label_table *build_label(char *name, int addr, int line, LabelType type);
int add_label_list(label_table **head, char *name, int addr, int line, LabelType type, char *file_name);
label_table *search_label_list(label_table *head, char *name);
ADDRESS_LIST *build_address_node(int addr);
void add_address_node(ADDRESS_LIST **head, int addr);
void add_address_to_label(label_table *node, int addr);
void init_instruction_list(InstructionList *list);
void add_instruction(InstructionList *list, Instruction *inst);
void print_instruction_list(InstructionList *list);

#endif
