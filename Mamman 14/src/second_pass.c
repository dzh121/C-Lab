#include "../headers/second_pass.h"

int handle_entry(char *line, label_table *label_head, char *file_name, int line_count) {
    char *token = strtok(line + 7, " "); /* skip over '.entry' and grab the label */
    char *after_label; /* to check if there's extra stuff after label */
    label_table *label; /* for looking up the label */

    /* make sure user actually gave a label */
    if (!token) {
        print_ext_error(ERROR_MISSING_ENTRY_NAME, file_name, line_count);
        return FAILURE;
    }

    token[strcspn(token, "\n")] = '\0'; /* clean up newline at the end if there is */

    /* make sure label is valid */
    if (!isValidLabel(token, file_name, line_count)) {
        return FAILURE;
    }

    if (!isspace(*token)) {
        after_label = strtok(NULL, " "); /* check for stuff after the label */

        /* if there is anything extra that shouldn't be there */
        if (after_label && *after_label != '\0' && !isspace(*after_label)) {
            print_ext_error(ERROR_EXTRA_TEXT, file_name, line_count);
            return FAILURE;
        }

        /* look for the label in the table */
        label = search_label_list(label_head, token);

        if (!label) {
            print_ext_error(ERROR_ENTRY_LABEL_NOT_FOUND, file_name, line_count);
            return FAILURE;
        }

        /* can't mark extern as entry */
        if (label->type == EXTERN) {
            print_ext_error(ERROR_ENTRY_CANNOT_BE_EXTERN, file_name, line_count);
            return FAILURE;
        }

        if (label->type == ENTRY) {
            print_ext_warning(WARNING_ENTRY_ALREADY_DEFINED, file_name, line_count);
        }

        /* mark it as entry */
        label->type = ENTRY;
        return SUCCESS;
    }

    return FAILURE;
}

int second_pass(InstructionList *instruction_list, DataList *data_list, label_table *label_head, char *file_name) {
    FILE *fp; /* file for reading the .am file again */
    char line[MAX_LINE_LENGTH], *after_label; /* line buffer and pointer to get past label if it exists */
    int line_count = 0; /* line counter */
    InstructionNode *current_inst_node; /* loop over instructions */
    Instruction *inst; /* shortcut to the current instruction */
    int did_fail = FALSE; /* track if we had errors */
    int address = INITIAL_IC; /* starting memory address */

    /* try to open file again to check for .entry directives */
    if ((fp = fopen(file_name, "r")) == NULL) {
        print_internal_error(ERROR_FILE_OPEN_SOURCE);
        return FAILURE;
    }

    /* go over file line by line */
    while (fgets(line, sizeof(line), fp) != NULL) {
        line_count++; /* increment line counter */

        after_label = strchr(line, ':'); /* see if there is a label */
        if (after_label != NULL) {
            after_label++; /* skip the ':' */
            if (isspace(*after_label)) after_label++; /* also skip space */
        } else {
            after_label = line; /* no label, just use the whole line */
        }

        /* look for entry directive */
        if (strncmp(after_label, ".entry", 6) == 0) {
            /* handle the entry directive */
            if (handle_entry(after_label, label_head, file_name, line_count)) {
                did_fail = TRUE;
            }
        }
    }
    /* close the file */
    fclose(fp);

    /* start at the head of the instruction list */
    current_inst_node = instruction_list->head;

    /* now go over the instructions and encode them */
    while (current_inst_node != NULL) {
        inst = &current_inst_node->instruction;

        /* process the instruction */
        if (process_instruction(inst, file_name, inst->line_number, data_list, label_head, address)) {
            did_fail = TRUE;
        }

        address += calculate_words(inst, file_name, inst->line_number); /* move to next address */
        current_inst_node = current_inst_node->next; /* next instruction */
    }

    /* return if we had any errors */
    return did_fail ? FAILURE : SUCCESS;
}