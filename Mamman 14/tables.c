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
    new_label->addr_list = NULL;
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

void add_address_to_label(label_table *node, int addr) {
    if (node == NULL) {
        return; /* Invalid node */
    }

    /* Add the address to the addr_list */
    add_address_node(&node->addr_list, addr);
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
void print_instruction_list(InstructionList *list) {
    InstructionNode *current = list->head;
    printf("\nInstruction List:\n");
    while (current) {
        printf("Instruction: %s, Opcode: %d, Funct: %d, Src Mode: %d, Src Reg: %d, Dest Mode: %d, Dest Reg: %d, ARE: %d, Src Label: %s, Dest Label: %s\n",
               current->instruction.name, current->instruction.opcode, current->instruction.funct,
               current->instruction.src_mode, current->instruction.src_operand, current->instruction.dest_mode,
               current->instruction.dest_operand, current->instruction.are, current->instruction.src_label,
               current->instruction.dest_label);
        current = current->next;
    }
}
