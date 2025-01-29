#include "../headers/second_pass.h"

int handle_entry(char *line, label_table *label_head, char *file_name, int line_count) {
    char *token = strtok(line + 7, " "); /* Skip '.entry' and tokenize */
    char *after_label; /* Pointer for checking extra text */
    label_table *label; /* Label for the entry */

    /* Check if there is an entry */
    if (!token) {
        print_ext_error(ERROR_MISSING_ENTRY_NAME, file_name, line_count);
        return FAILURE;
    }

    token[strcspn(token, "\n")] = '\0'; /* Remove newline if present */

    /* Check if the label is valid */
    if (!isValidLabel(token, file_name, line_count)) {
        return FAILURE;
    }

    if (!isspace(*token)) {
        after_label = strtok(NULL, " "); /* Skip the rest of the line */
        /* Check for extra text */
        if (after_label && *after_label != '\0' && !isspace(*after_label)) {
            print_ext_error(ERROR_EXTRA_TEXT, file_name, line_count);
            return FAILURE;
        }
        /* Find the label */
        label = search_label_list(label_head, token);

        /* Check if the label is found */
        if (!label) {
            print_ext_error(ERROR_ENTRY_LABEL_NOT_FOUND, file_name, line_count);
            return FAILURE;
        }
        /* Check if the label is extern */
        if (label->type == EXTERN)
        {
            print_ext_error(ERROR_ENTRY_CANNOT_BE_EXTERN, file_name, line_count);
            return FAILURE;
        }
        /* Set the label as an entry */
        label->type = ENTRY;
        return SUCCESS; /* Success */
    }
    return FAILURE; /* Failure */
}

int second_pass(InstructionList *instruction_list, DataList *data_list, label_table *label_head, char *file_name) {
    FILE *fp; /* File pointer for reading the assembly file */
    char line[MAX_LINE_LENGTH], *after_label; /* Buffer for reading lines and pointer for label extraction */
    int line_count = 0; /* Tracks the current line number */
    InstructionNode *current_inst_node; /* Pointer to traverse the instruction list */
    Instruction *inst; /* Pointer to the current instruction being processed */
    int did_fail = FALSE; /* Flag to indicate if any errors occurred during the second pass */
    int address = INITIAL_IC; /* Initial memory address for instructions */

    /* Open the input file */
    if ((fp = fopen(file_name, "r")) == NULL) {
        print_internal_error(ERROR_FILE_OPEN_SOURCE);
        return FAILURE;
    }

    /* Process and finds entry */
    while (fgets(line, sizeof(line), fp) != NULL) {
        line_count++; /* Increment the line counter */
        after_label = strchr(line, ':'); /*skip : and space*/

        if (after_label != NULL) {
            after_label++; /* Move past ':' */
            if (isspace(*after_label)) after_label++; /* Skip space if present */
        } else {
            after_label = line; /* No label, start from the beginning of the line */
        }

        /* Check for entry */
        if (strncmp(after_label, ".entry", 6) == 0) {
            if (handle_entry(after_label, label_head, file_name, line_count)) {
                print_ext_error(ERROR_ENTRY_FAILED, file_name, line_count);
                did_fail = TRUE;
            }
        }
    }

    current_inst_node = instruction_list->head;
    /* Process instructions */
    while (current_inst_node != NULL) {
        inst = &current_inst_node->instruction; /* Get the current instruction */
        /* Process the instruction while knowing each label*/
        if (process_instruction(inst, file_name, inst->line_number, data_list, label_head, address)) {
            did_fail = TRUE;
        }
        /* Calculate the address for the next instruction */
        address += calculate_words(inst, file_name, inst->line_number);
        /* Move to the next instruction */
        current_inst_node = current_inst_node->next;
    }

    fclose(fp); /* Close the input file */

    return did_fail ? FAILURE : SUCCESS; /* Return the status */
}
