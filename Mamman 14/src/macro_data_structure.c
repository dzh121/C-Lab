#include "../headers/macro_data_structure.h"


macro_node *build_node(char *name, char *content, int line) {
    /* Allocate memory for the new node */
    macro_node *new_node = (macro_node *) handle_malloc(sizeof(macro_node));

    /* Allocate memory and copy name */
    new_node->name = (char *) handle_malloc(strlen(name) + 1); /* +1 for '\0' */

    /* Copy the name string */
    strcpy(new_node->name, name);

    /* Allocate memory and copy content */
    new_node->content = (char *) handle_malloc(strlen(content) + 1); /* +1 for '\0' */

    /* Copy the content string */
    strcpy(new_node->content, content);

    /* Set line and next pointer */
    new_node->line = line;
    new_node->next = NULL;

    return new_node; /* Return the new node */
}

macro_node *search_list(macro_node *head, char *name) {
    /* Define the current node */
    macro_node *current = head;

    /* Loop through the list */
    while (current != NULL) {
        /* Check if the current node name is equal to the name */
        if (current->name != NULL && strcmp(current->name, name) == 0) {
            /* If the names match, return the current node */
            return current;
        }
        /* Move to the next node */
        current = current->next;
    }
    /* If the name was not found, return NULL */
    return NULL;
}

int add_node(macro_node **head, char *name, char *content, int line, char *file_name) {
    /* Define the current node */
    macro_node *temp, *new_node;

    /* Check if the name and content are valid */
    if (!name || !content) {
        print_ext_error(ERROR_MACRO_NO_NAME, file_name, line);
        return FAILURE;
    }

    /* search for the name in the list */
    temp = search_list(*head, name);
    if (temp != NULL) {
        /* If the name already exists, print an error message and exit */
        print_ext_error(ERROR_MACRO_NAME_DUP, file_name, line);
        return FAILURE;
    }

    /* Build the new node */
    new_node = build_node(name, content, line);
    if (*head == NULL) {
        /* If the list is empty, set the head to the new node */
        *head = new_node;
        return SUCCESS;
    }

    /* Add to the end of the list */
    temp = *head;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = new_node;
    return SUCCESS;
}

void free_macro_node(macro_node *node) {
    /* Check if the node is valid */
    if (!node) return;

    /* Free the name and content */
    if (node->name) free(node->name);
    if (node->content) free(node->content);
    free(node);
}

void free_macro_list(macro_node *head) {
    /* Define the current and next nodes */
    macro_node *current = head;
    macro_node *next;

    while (current != NULL) {
        /* Save the next node */
        next = current->next;
        /* Free the current node */
        current->next = NULL;
        /* Free the current node */
        free_macro_node(current);
        /* Move to the next node */
        current = next;
    }
}
