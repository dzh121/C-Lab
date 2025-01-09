#ifndef TABLES_H
#define TABLES_H

#include "assembler.h"

typedef enum {
    CODE,
    DATA,
    ENTRY,
    EXTERN,
} LabelType;


typedef struct {
    int addr; /* address */
    struct ADDRESS_LIST *next; /* pointer to the next node */
} ADDRESS_LIST;

/* Label Table for storing defined labels */
typedef struct label_table {
    char name[MAX_LABEL_SIZE]; /* Label name */
    int addr; /* Address (0 if unresolved, resolved during second pass) */
    int line; /* Line number */
    LabelType type; /* Type of label (CODE, DATA, ENTRY, EXTERN) */
    struct label_table *next; /* Pointer to the next node */
} label_table;

/* Entry/Extern Table for tracking entry and extern symbols */
typedef struct entry_ext_table {
    char name[MAX_LABEL_SIZE]; /* Label name */
    ADDRESS_LIST *addr_list; /* List of all usage addresses (extern) or final address (entry) */
    int final_addr; /* Final defined address for entries (placeholder 0 initially) */
    LabelType type; /* Type of label (ENTRY or EXTERN) */
    struct entry_ext_table *next; /* Pointer to the next node */
} entry_ext_table;


/* Function Prototypes */
label_table *build_label(char *name, int addr, int line, LabelType type);
int add_label_list(label_table **head, char *name, int addr, int line, LabelType type, char *file_name);
label_table *search_label_list(label_table *head, char *name);
ADDRESS_LIST *build_address_node(int addr);
void add_address_node(ADDRESS_LIST **head, int addr);
entry_ext_table *build_entry_ext(char *name, LabelType type);
int add_entry_ext_list(entry_ext_table **head, char *name, int addr, int IC, LabelType type, char *file_name, int line);
entry_ext_table *search_entry_ext_list(entry_ext_table *head, char *name);
void init_instruction_list(InstructionList *list);
void add_instruction(InstructionList *list, Instruction *inst);
void print_instruction_list(const InstructionList *list);

#endif
