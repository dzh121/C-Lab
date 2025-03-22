#include "../headers/funcs.h"

/* Register names */
const char *registers[] = {
    "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"
};

/* Saved words */
const char *saved_words[] = {
    "mov", "cmp", "add", "sub", "lea", "clr", "not", "inc",
    "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop", "data", "string",
    "entry", "extern"
};

void *handle_malloc(long size) {
    /* Allocate memory and handle allocation failure */
    void *ptr = malloc(size);
    if (!ptr) {
        print_internal_error(ERROR_MEMORY_ALLOCATION);
    }
    return ptr;
}

char *add_suffix(char *file_name, char *suffix) {
    /* Allocate memory for the new name */
    char *new_name = (char *) handle_malloc(strlen(file_name) + strlen(suffix) + 1);
    if (!new_name) {
        return NULL;
    }
    /* Copy the file name */
    strcpy(new_name, file_name);
    strcat(new_name, suffix);
    /* Return the new name */
    return new_name;
}

char *trim_whitespace(char *str) {
    char *end; /* Pointer to the end of the string */

    /* Trim leading spaces. */
    while (isspace((unsigned char) *str)) str++;

    if (*str == '\0') return str; /* All spaces. */

    /* Trim trailing spaces. */
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char) *end)) end--;

    /* Null-terminate. */
    *(end + 1) = '\0';

    return str;
}

int is_reserved(char *line) {
    int i; /* Loop index */
    /* Check if the word matches any register */
    for (i = 0; i < REGISTER_COUNT; i++) {
        if (strcmp(line, registers[i]) == 0) {
            return 1; /* Found in registers */
        }
    }

    /* Check if the word matches any command */
    for (i = 0; i < SAVED_WORDS_COUNT; i++) {
        if (strcmp(line, saved_words[i]) == 0) {
            return 1; /* Found in commands */
        }
    }

    /* Check if the word matches any directive */
    if (strcmp(line, ".data") == 0 || strcmp(line, ".string") == 0 ||
        strcmp(line, ".entry") == 0 || strcmp(line, ".extern") == 0) {
        return 1; /* Reserved name */
    }

    return 0; /* Not reserved */
}

void init_data_list(DataList *list) {
    /* Initialize the data list */
    list->head = NULL;
}

int add_data_node(DataList *list, int address, int value, char *file_name) {
    /* Allocate memory for the new node */
    DataNode *new_node = (DataNode *) handle_malloc(sizeof(DataNode));
    DataNode *current;

    if (!new_node) {
        return FAILURE;
    }
    /* Create a temporary pointer */

    if (address > MAX_MEMORY_SIZE) {
        handle_memory_overflow(file_name, address);
        free(new_node);
        return FAILURE;
    }

    new_node->address = address; /* Set the address */

    new_node->data = value; /* Set the data */
    new_node->next = NULL; /* Set the next pointer to NULL */

    /* Insert in sorted order by address */
    if (!list->head || address < list->head->address) {
        new_node->next = list->head; /* Set the next pointer to the head */
        list->head = new_node; /* Set the head to the new node */
    } else {
        current = list->head; /* Set the current pointer to the head */
        while (current->next && current->next->address < address) {
            current = current->next; /* Move to the next node */
        }
        new_node->next = current->next; /* Set the next pointer to the current next */
        current->next = new_node; /* Set the current next to the new node */
    }
    return SUCCESS;
}

void free_data_list(DataList *list) {
    /* Free the data list */
    DataNode *current = list->head; /* Set the current pointer to the head */
    while (current) {
        DataNode *temp = current; /* Set the temporary pointer to the current */
        current = current->next; /* Move to the next node */
        free(temp); /* Free the temporary node */
    }
    list->head = NULL; /* Set the head to NULL */
}

void free_instruction_list(InstructionList *list) {
    InstructionNode *current = list->head; /* Set the current pointer to the head */
    InstructionNode *temp; /* Temporary pointer */

    while (current != NULL) {
        temp = current; /* Set the temporary pointer to the current */
        current = current->next; /* Move to the next node */
        free(temp); /* Free the temporary node */
    }

    list->head = NULL; /* Set the head to NULL */
    list->tail = NULL; /* Set the tail to NULL */
}

void free_label_list(label_table *head) {
    label_table *current = head; /* Set the current pointer to the head */
    label_table *temp; /* Temporary pointer */
    ADDRESS_LIST *addr_current; /* Address list current pointer */
    ADDRESS_LIST *addr_temp; /* Address list temporary pointer */

    while (current != NULL) {
        temp = current; /* Set the temporary pointer to the current */

        /* Free the address list if it's an EXTERN label */
        addr_current = current->addr_list;
        while (addr_current != NULL) {
            addr_temp = addr_current; /* Set the temporary address pointer to the current */
            addr_current = addr_current->next; /* Move to the next address */
            free(addr_temp); /* Free the temporary address node */
        }

        current = current->next; /* Move to the next node */
        free(temp); /* Free the temporary node */
    }
}

int isValidLabel(char *label, char *file_name, int line_number) {
    char *c; /* Character pointer */

    /* Check if not empty */
    if (label == NULL || *label == '\0' || isspace(*label)) {
        print_ext_error(ERROR_LABEL_INVALID, file_name, line_number);
        return 0; /* Empty label */
    }

    /* Check if too long */
    if (strlen(label) > MAX_LABEL_SIZE) {
        print_ext_error(ERROR_LABEL_TOO_LONG, file_name, line_number);
        return 0; /* Too long */
    }

    /* Check if starts with a letter or _ */
    if (!isalpha(label[0])) {
        print_ext_error(ERROR_LABEL_INVALID, file_name, line_number);
        return 0; /* Must start with a letter */
    }

    /* Check if contains only alphanumeric characters */
    for (c = label; *c != '\0'; c++) {
        if (!isalnum(*c)) {
            print_ext_error(ERROR_LABEL_INVALID, file_name, line_number);
            return 0; /* Contains invalid character */
        }
    }

    /* Check if reserved */
    if (is_reserved((char *) label)) {
        print_ext_error(ERROR_LABEL_RESERVED, file_name, line_number);
        return 0; /* Reserved keyword */
    }

    return 1; /* Valid label */
}

int build_output_files(char *file_name, DataList *data_list, label_table *label_head, int ICF, int DCF) {
    FILE *ob_fp = NULL, *ent_fp = NULL, *ext_fp = NULL; /* File pointers */
    char *ob_file_name, *ent_file_name, *ext_file_name; /* File names */
    DataNode *current_data = data_list->head; /* Set the current data pointer to the head */
    label_table *current_label = label_head; /* Set the current label pointer to the head */
    ADDRESS_LIST *current_address = NULL; /* Address list current pointer */
    int has_entries = FALSE, has_externals = FALSE; /* Flags for entry and external files */
    char *hexValue = (char *) handle_malloc(7); /* Hexadecimal value buffer */
    if (!hexValue) {
        return FAILURE;
    }
    /* Build the output file names */
    ob_file_name = add_suffix(file_name, ".ob");

    /* Open the object file */
    ob_fp = fopen(ob_file_name, "w");
    if (!ob_fp) {
        print_internal_error(ERROR_FILE_OPEN_OUTPUT);
        free(ob_file_name); /* Free the object file name */
        return FAILURE;
    }

    ent_file_name = add_suffix(file_name, ".ent");
    ext_file_name = add_suffix(file_name, ".ext");


    /* Write the object file */
    fprintf(ob_fp, "%7d %d\n", ICF - INITIAL_IC, DCF); /* Write the ICF and DCF */
    while (current_data) {
        /* Write the data to the object file */
        sprintf(hexValue, "%06x", current_data->data & 0xFFFFFF); /* Convert to hexadecimal */
        fprintf(ob_fp, "%07d %s\n", current_data->address, hexValue); /* Write the address and data */
        current_data = current_data->next; /* Move to the next node */
    }
    if (hexValue) {
        free(hexValue); /* Free the hexadecimal buffer */
    }
    /* Check and write the entry and external files */
    while (current_label) {
        if (current_label->type == ENTRY) {
            /* Write the entry to the entry file */
            if (!ent_fp) {
                ent_fp = fopen(ent_file_name, "w");
                if (!ent_fp) {
                    print_internal_error(ERROR_FILE_OPEN_OUTPUT);
                    fclose(ob_fp);
                    if (ent_fp) fclose(ent_fp);
                    if (ob_file_name) free(ob_file_name); /* Free the object file name */
                    if (ent_file_name) free(ent_file_name); /* Free the entry file name */
                    if (ext_file_name) free(ext_file_name); /* Free the external file name */
                    return FAILURE;
                }
            }
            /* Write the entry to the file */
            fprintf(ent_fp, "%s %07d\n", current_label->name, current_label->addr);
            has_entries = TRUE; /* Set the entry flag */
        } else if (current_label->type == EXTERN) {
            /* Write the external to the external file */
            current_address = current_label->addr_list;
            /* Loop through the address list */
            while (current_address) {
                /* Write the external to the file */
                if (!ext_fp) {
                    ext_fp = fopen(ext_file_name, "w");
                    if (!ext_fp) {
                        print_internal_error(ERROR_FILE_OPEN_OUTPUT);
                        fclose(ob_fp);
                        if (ent_fp) fclose(ent_fp);
                        if (ob_file_name) free(ob_file_name); /* Free the object file name */
                        if (ent_file_name) free(ent_file_name); /* Free the entry file name */
                        if (ext_file_name) free(ext_file_name); /* Free the external file name */
                        return FAILURE;
                    }
                }
                /* Write the external to the file */
                fprintf(ext_fp, "%s %07d\n", current_label->name, current_address->addr);
                /* Move to the next address */
                current_address = current_address->next;
                has_externals = TRUE; /* Set the external flag */
            }
        }
        /* Move to the next label */
        current_label = current_label->next;
    }

    /* Close the object file */
    fclose(ob_fp);

    /* Close the entry file if it was opened */
    if (ent_fp) fclose(ent_fp);

    /* Close the external file if it was opened */
    if (ext_fp) fclose(ext_fp);

    if (ob_file_name) free(ob_file_name); /* Free the object file name */
    if (ent_file_name) free(ent_file_name); /* Free the entry file name */
    if (ext_file_name) free(ext_file_name); /* Free the external file name */

    /* Delete empty files */
    if (!has_entries && ent_fp) remove(ent_file_name);
    if (!has_externals && ext_fp) remove(ext_file_name);

    return SUCCESS; /* Success */
}

void print_data_list(DataList *list) {
    DataNode *current = list->head;
    char *hexValue = (char *) handle_malloc(7);
    if (!hexValue) {
        return;
    }
    while (current) {
        sprintf(hexValue, "%06x", current->data & 0xFFFFFF);
        printf("%04d %s\n", current->address, hexValue);
        current = current->next;
    }
    free(hexValue);
}
