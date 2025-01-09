#include "tables.h"
#include "funcs.h"
#include "string.h"
#include "errors.h"

ADDRESS_LIST *build_address_node(int addr) {
    ADDRESS_LIST *node = (ADDRESS_LIST *)handle_malloc(sizeof(ADDRESS_LIST));
    node->addr = addr;
    node->next = NULL;
    return node;
}

void add_address_node(ADDRESS_LIST **head, int addr) {
    ADDRESS_LIST *new_node = build_address_node(addr);
    ADDRESS_LIST *temp = *head;

    if (*head == NULL) {
        *head = new_node;
    } else {
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = new_node;
    }
}

label_table *search_label_list(label_table *head, char *name) {
    while (head != NULL) {
        if (strcmp(head->name, name) == 0) {
            return head; /* Label already exists */
        }
        head = head->next;
    }
    return NULL; /* Label not found */
}

label_table *build_label(char *name, int addr, int line, LabelType type) {
    label_table *new_label = (label_table *)handle_malloc(sizeof(label_table));

    strcpy(new_label->name, name);
    new_label->addr = addr;
    new_label->line = line;
    new_label->type = type;
    new_label->next = NULL;

    return new_label;
}


int add_label_list(label_table **head, char *name, int addr, int line, LabelType type, char *file_name) {
    label_table *temp, *new_node;

    temp = search_label_list(*head, name);
    if (temp != NULL) {
        print_ext_error(ERROR_LABEL_DUP, file_name, line);
        return 0;
    }

    new_node = build_label(name, addr, line, type);
    if (*head == NULL) {
        *head = new_node;
    } else {
        temp = *head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = new_node;
    }

    return 1;
}


entry_ext_table *search_entry_ext_list(entry_ext_table *head, char *name) {
    while (head != NULL) {
        if (strcmp(head->name, name) == 0) {
            return head; /* Entry/Extern already exists */
        }
        head = head->next;
    }
    return NULL; /* Entry/Extern not found */
}

entry_ext_table *build_entry_ext(char *name, LabelType type) {
    entry_ext_table *new_entry_ext = (entry_ext_table *)handle_malloc(sizeof(entry_ext_table));

    strcpy(new_entry_ext->name, name);
    new_entry_ext->addr_list = NULL;
    new_entry_ext->final_addr = 0;
    new_entry_ext->type = type;
    new_entry_ext->next = NULL;

    return new_entry_ext;
}


int add_entry_ext_list(entry_ext_table **head, char *name, int addr, int IC, LabelType type, char *file_name, int line) {
    entry_ext_table *temp, *new_node;

    temp = search_entry_ext_list(*head, name);
    if (temp != NULL) {
        if (type == EXTERN) {
            add_address_node(&temp->addr_list, IC);
        } else if (type == ENTRY) {
            temp->final_addr = addr;
        }
        return 1;
    }

    new_node = build_entry_ext(name, type);
    if (type == EXTERN) {
        add_address_node(&new_node->addr_list, IC);
    } else if (type == ENTRY) {
        new_node->final_addr = addr;
    }

    if (*head == NULL) {
        *head = new_node;
    } else {
        temp = *head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = new_node;
    }

    return 1;
}
/* Initialize an empty instruction list */
void init_instruction_list(InstructionList *list) {
    list->head = NULL;
    list->tail = NULL;
}

/* Add an instruction to the list */
void add_instruction(InstructionList *list, Instruction *inst) {
    InstructionNode *new_node = handle_malloc(sizeof(InstructionNode));

    /* Copy the instruction data */
    memcpy(&new_node->instruction, inst, sizeof(Instruction));
    new_node->next = NULL;
    /* Append to the list */
    if (!list->head) {
        list->head = new_node;
        list->tail = new_node;
    } else {
        list->tail->next = new_node;
        list->tail = new_node;
    }
}

/* Print the instruction list for debugging */
void print_instruction_list(const InstructionList *list) {
    const InstructionNode *current = list->head;
    printf("\nInstruction List:\n");
    while (current) {
        printf("Instruction: %s, Opcode: %d, Funct: %d, Src Mode: %d, Src Reg: %d, Dest Mode: %d, Dest Reg: %d, ARE: %d, Src Label: %s, Dest Label: %s\n",
               current->instruction.name, current->instruction.opcode, current->instruction.funct,
               current->instruction.src_mode, current->instruction.src_reg, current->instruction.dest_mode,
               current->instruction.dest_reg, current->instruction.are, current->instruction.src_label,
               current->instruction.dest_label);
        current = current->next;
    }
}
