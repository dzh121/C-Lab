#ifndef TABLES_H
#define TABLES_H

#include "globals.h"

typedef struct ADDRESS_LIST{
    int addr; /* address */
    struct ADDRESS_LIST *next; /* pointer to the next node */
} ADDRESS_LIST;

/* Label Table for storing defined labels */
typedef struct label_table {
    char name[MAX_LABEL_SIZE]; /* Label name */
    int addr; /* Address (0 if unresolved, resolved during second pass) */
    int line; /* Line number */
    LabelType type; /* Type of label (CODE, DATA, ENTRY, EXTERN) */
    ADDRESS_LIST *addr_list; /* List of all usage addresses for EXTERN */
    struct label_table *next; /* Pointer to the next node */
} label_table;


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
