#include "../headers/tables.h"

ADDRESS_LIST *build_address_node(int addr) {
    /* Allocate memory for the new node */
    ADDRESS_LIST *node = (ADDRESS_LIST *)handle_malloc(sizeof(ADDRESS_LIST));
    node->addr = addr; /* Set the address */
    node->next = NULL; /* Set the next pointer to NULL */
    return node; /* Return the new node */
}

void add_address_node(ADDRESS_LIST **head, int addr) {
    ADDRESS_LIST *new_node = build_address_node(addr); /* Create a new node */
    ADDRESS_LIST *temp = *head; /* Create a temporary pointer */

    /* Check if the head is NULL */
    if (*head == NULL) {
        *head = new_node; /* set the head to the new node */
    } else {
        /* Loop through the list to find the last node */
        while (temp->next != NULL) {
            temp = temp->next;
        }
        /* Add the new node to the end of the list */
        temp->next = new_node;
    }
}

label_table *search_label_list(label_table *head, char *name) {
    while (head != NULL) {
        /* Check if the label name matches */
        if (strcmp(head->name, name) == 0) {
            return head; /* Label already exists */
        }
        /* Move to the next node */
        head = head->next;
    }
    return NULL; /* Label not found */
}

label_table *build_label(char *name, int addr, int line, LabelType type) {
    /* Allocate memory for the new label */
    label_table *new_label = (label_table *)handle_malloc(sizeof(label_table));
    /* Copy the name */
    strcpy(new_label->name, name);
    new_label->addr = addr; /* Set the address */
    new_label->line = line; /* Set the line number */
    new_label->type = type; /* Set the label type */
    new_label->addr_list = NULL; /* Initialize the address list */
    new_label->next = NULL; /* Set the next pointer to NULL */

    return new_label; /* Return the new label */
}


int add_label_list(label_table **head, char *name, int addr, int line, LabelType type, char *file_name) {
    label_table *temp, *new_node; /* Temporary and new label pointers */

    /* Check if the label name is valid */
    temp = search_label_list(*head, name);
    if (temp != NULL) {
        print_ext_error(ERROR_LABEL_DUP, file_name, line);
        return FAILURE;
    }

    /* Build the new label */
    new_node = build_label(name, addr, line, type);
    if (*head == NULL) {
        *head = new_node; /* Set the head to the new label */
    } else {
        /* Add the new label to the end of the list */
        temp = *head;
        /* Loop through the list to find the last node */
        while (temp->next != NULL) {
            temp = temp->next;
        }
        /* Add the new label to the end of the list */
        temp->next = new_node;
    }

    return SUCCESS;
}

void add_address_to_label(label_table *node, int addr) {
    if (node == NULL) {
        return; /* Invalid node */
    }

    /* Add the address to the addr_list */
    add_address_node(&node->addr_list, addr);
}

void init_instruction_list(InstructionList *list) {
    /* Initialize the list */
    list->head = NULL;
    list->tail = NULL;
}

/* Add an instruction to the list */
void add_instruction(InstructionList *list, Instruction *inst) {
    InstructionNode *new_node = handle_malloc(sizeof(InstructionNode)); /* Allocate memory for the new node */

    /* Copy the instruction data */
    memcpy(&new_node->instruction, inst, sizeof(Instruction));
    new_node->next = NULL;
    /* Append to the list */
    if (!list->head) {
        list->head = new_node; /* Set the head to the new node */
        list->tail = new_node; /* Set the tail to the new node */
    } else {
        list->tail->next = new_node; /* Add the new node to the end of the list */
        list->tail = new_node; /* Set the tail to the new node */
    }
}

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
