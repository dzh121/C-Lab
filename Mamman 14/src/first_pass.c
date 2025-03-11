#include "../headers/first_pass.h"

int encode_string(char *line, int *DC, int IC, DataList *data_list, char *file_name, int line_count) {
    char *start, *end; /* Pointers for the start and end of the string */
    char *after_string; /* Pointer for checking extra text */

    if (!line) {
        /* No line found */
        print_ext_error(ERROR_STRING_MISSING_QUOTES, file_name, line_count);
        return FAILURE;
    }

    start = strchr(line, '"'); /* Find the first quote */
    if (!start) {
        /* No opening quote found */
        print_ext_error(ERROR_STRING_MISSING_QUOTES, file_name, line_count);
        return FAILURE;
    }

    start++; /* Move past the opening quote */
    end = strchr(start, '"'); /* Find the closing quote */

    if (!end) {
        /* No closing quote found */
        print_ext_error(ERROR_STRING_MISSING_QUOTES, file_name, line_count);
        return FAILURE;
    }

    /* Encode each character in the string and add to the data list */
    while (start < end) {
        /* Add ASCII value to the list with DC address */
        add_data_node(data_list, *DC, (int) (*start));
        (*DC)++; /* Increment the data counter */
        start++; /* Move to the next character */

        /* Check for memory overflow */
        if (*DC + IC > MAX_MEMORY_SIZE) {
            print_internal_error(ERROR_MEMORY_OVERFLOW);
            return FAILURE;
        }
    }

    after_string = end + 1; /* Move past the closing quote */
    /* Check for extra text after the string */
    if (after_string && after_string[0] != '\0' && after_string[0] != '\n') {
        /* Extra text found */
        print_ext_error(ERROR_EXTRA_TEXT, file_name, line_count);
        return FAILURE;
    }

    /* Add null terminator to data */
    add_data_node(data_list, *DC, 0);
    (*DC)++;

    /* Check for memory overflow */
    if (*DC + IC > MAX_MEMORY_SIZE) {
        print_internal_error(ERROR_MEMORY_OVERFLOW);
        return FAILURE;
    }

    return SUCCESS; /* Success */
}

int handle_extern(char *line, label_table **label_head, char *file_name, int line_count) {
    char *token; /* Token for the external label */
    char *after_label; /* Pointer for checking extra text */

    if (!line) {
        /* No line found */
        print_ext_error(ERROR_MISSING_EXTERNAL_NAME, file_name, line_count);
        return FAILURE;
    }

    token = strtok(line + EXTERN_DIRECTIVE_OFFSET, " "); /* Get the first token */
    if (!token) {
        /* No token found */
        print_ext_error(ERROR_MISSING_EXTERNAL_NAME, file_name, line_count);
        return FAILURE;
    }

    /* Remove newline if present */
    token[strcspn(token, "\n")] = '\0';
    if (!isValidLabel(token, file_name, line_count)) {
        /* Invalid label */
        return FAILURE;
    }

    /* Check for extra text */
    after_label = strtok(NULL, " ");
    if (after_label) {
        /* Extra text found */
        print_ext_error(ERROR_EXTRA_TEXT, file_name, line_count);
        return FAILURE;
    }

    /* Add the external label to the label table */
    if (add_label_list(label_head, token, 0, line_count, EXTERN, file_name)) {
        /* Failed to add label */
        return FAILURE;
    }

    return SUCCESS; /* Success */
}

int encode_data(char *line, int *DC, int IC, DataList *data_list, char *file_name, int line_count) {
    int value; /* Parsed integer value */
    char *ptr; /* Pointer for checking the token */
    char *token = strtok(line + DATA_DIRECTIVE_OFFSET, ","); /* Skip '.data' and tokenize by ',' */

    while (token) {
        /* Trim leading and trailing spaces from the token. */
        token = trim_whitespace(token);

        /* Ensure token is not empty after trimming. */
        if (token[0] == '\0') {
            print_ext_error(ERROR_INVALID_DATA, file_name, line_count);
            return FAILURE;
        }

        /* Validate that token contains only an integer value. */
        for (ptr = token; *ptr != '\0'; ptr++) {
            /* Check for invalid characters in the token. */
            if (!isdigit(*ptr) && *ptr != '-' && *ptr != '+') {
                print_ext_error(ERROR_INVALID_NUMBER, file_name, line_count);
                return FAILURE;
            }
        }

        if (strlen(token) > 8) { /* Check if the number is too long */
            print_ext_error(ERROR_NUMBER_OVERFLOW, file_name, line_count);
            return FAILURE;
        }

        /* Check if the value can be parsed correctly. */
        if (sscanf(token, "%d", &value) != 1) {
            print_ext_error(ERROR_INVALID_NUMBER, file_name, line_count);
            return FAILURE;
        }

        /* Check if the value is within the valid range. */
        if (value > MAX_DATA_VALUE || value < MIN_DATA_VALUE) {
            print_ext_error(ERROR_NUMBER_OVERFLOW, file_name, line_count);
            return FAILURE;
        }

        /* Add the parsed integer to the data list. */
        add_data_node(data_list, *DC, value);
        (*DC)++;

        /* Check for memory overflow */
        if (*DC + IC >= MAX_MEMORY_SIZE) {
            print_internal_error(ERROR_MEMORY_OVERFLOW);
            return FAILURE;
        }

        /* Get the next token. */
        token = strtok(NULL, ",");
    }

    return SUCCESS; /* Success */
}


int first_pass(char *file_name, DataList *data_list, InstructionList *instruction_list, label_table **label_head,
               int *ICF, int *DCF) {
    /* Initialize counters, pointers, and buffers for the first pass */
    int IC = INITIAL_IC, DC = INITIAL_DC, line_count = 0, did_fail = FALSE, label_length;
    FILE *fp; /* File pointer for the input file */
    char line[MAX_LINE_LENGTH], *after_label, label[MAX_LABEL_SIZE]; /* Buffers and label handling */
    label_table *label_dummy = NULL; /* Temporary pointer for label processing */
    DataNode *current_data = NULL; /* Pointer for traversing data nodes */
    Instruction *inst = NULL; /* Pointer to the current instruction */
    /* For the rest of this assembler we know there is only tailing or leading white spaces and if there are multiple white spaces they are treated as one */

    /* Open the input file */
    if ((fp = fopen(file_name, "r")) == NULL) {
        print_internal_error(ERROR_FILE_OPEN_SOURCE);
        return FAILURE;
    }

    while (fgets(line, sizeof(line), fp) != NULL) {
        line_count++; /* Increment the line counter */

        memset(label, '\0', sizeof(label)); /* Clear the label buffer */

        after_label = strchr(line, ':'); /*skip : and space*/
        if (after_label != NULL) {
            label_length = after_label - line; /* Calculate label length  dont count : */

            /* Copy the label to the buffer */
            strncpy(label, line, label_length);
            /* Ensure null-termination */
            label[label_length] = '\0';

            /* Check if the label is valid */
            if (!isValidLabel(label, file_name, line_count)) {
                did_fail = TRUE;
                memset(label, '\0', sizeof(label)); /* Clear the label buffer */
                continue;
            }
            if (*after_label != ':')
            {
                did_fail = TRUE;
                print_ext_error(ERROR_MISSING_COLON, file_name, line_count);
                continue;
            }

            after_label++; /* Move past ':' */

            /* Check for white space after the label */
            if (!isspace(*after_label))
            {
                did_fail = TRUE;
                print_ext_error(ERROR_MISSING_WHITE_SPACE, file_name, line_count);
                continue;
            }

            after_label++; /* Move past the white space */

            /* Check if there is a label with no directive or instruction */
            if (!after_label || !*after_label || *after_label == '\n'){
                print_ext_error(ERROR_LABEL_WITH_NO_DIRECTIVE_OR_INSTRUCTION, file_name, line_count);
                did_fail = TRUE;
                continue;
            }
        } else {
            after_label = line; /* No label, start from the beginning of the line */
        }

        if (strncmp(after_label, ".data", 5) == 0 && isspace(after_label[5])) {
            /* Encode the data */
            if (label[0] != '\0') {
                /* Add the label to the label table */
                if (add_label_list(label_head, label, DC, line_count, DATA, file_name)) {
                    did_fail = TRUE;
                    continue;
                }
            }
            /* Encode the data */
            if (encode_data(after_label, &DC, IC, data_list, file_name, line_count)) {
                did_fail = TRUE;
                continue;
            }
        } else if (strncmp(after_label, ".string", 7) == 0 && isspace(after_label[7])) {
            /* Check if there is a label */
            if (label[0] != '\0' && label[0] != '\n') {
                /* Add the label to the label table */
                if (add_label_list(label_head, label, DC, line_count, DATA, file_name)) {
                    did_fail = TRUE;
                    continue;
                }
            }
            /* Encode the string */
            if (encode_string(after_label, &DC, IC, data_list, file_name, line_count)) {
                did_fail = TRUE;
                continue;
            }
        } else if (strncmp(after_label, ".extern", 7) == 0 && isspace(after_label[7])) {
            /* Check if there is a label */
            if (label[0] != '\0' && label[0] != '\n') {
                /* Warn about label at the start of the line */
                print_ext_warning(WARNING_LABEL_AT_START_EXTERN, file_name, line_count);
            }

            /* Handle the extern directive */
            if (handle_extern(after_label, label_head, file_name, line_count)) {
                did_fail = TRUE;
                continue;
            }
        } else if (strncmp(after_label, ".entry", 6) == 0 && isspace(after_label[6])) {
            /* Check if there is a label */
            if (label[0] != '\0' && label[0] != '\n') {
                /* Warn about label at the start of the line */
                print_ext_warning(WARNING_LABEL_AT_START_ENTRY, file_name, line_count);
            }
            continue; /* Ignore .entry directive we will address that in second pass */
        } else if (strncmp(after_label, ".", 1) == 0) {
            /* Invalid directive (not .data .string .entry or .extern) */
            print_ext_error(ERROR_INVALID_INSTRUCTION, file_name, line_count);
            did_fail = TRUE;
            continue;
        } else {
            /* Check if there is a label */
            if (label[0] != '\0') {
                /* Add the label to the label table */
                if (add_label_list(label_head, label, IC, line_count, CODE, file_name)) {
                    did_fail = TRUE;
                    continue;
                }
            }

            /* Parse the instruction */
            if (parse_instruction(after_label, instruction_list, file_name, line_count)) {
                did_fail = TRUE;
                continue;
            }
            /* Get the last instruction */
            inst = &instruction_list->tail->instruction;
            /* Calculate the number of words for the instruction */
            IC += calculate_words(inst, file_name, line_count);
        }
        /* Reset the label buffer */
        memset(label, '\0', sizeof(label));

        /* Check if the memory size is exceeded */
        if (IC + DC > MAX_MEMORY_SIZE) {
            print_internal_error(ERROR_MEMORY_OVERFLOW);
            did_fail = TRUE;
            break;
        }
    }

    /* Check if there was an error during the loop */
    if (did_fail) {
        fclose(fp);
        return FAILURE;
    }
    /* Set the final values of ICF and DCF */
    *ICF = IC;
    *DCF = DC;

    /* Update data addresses */
    current_data = data_list->head;
    while (current_data != NULL) {
        /* Add the ICF to the data address */
        current_data->address += *ICF;
        current_data = current_data->next;
    }

    /* Update label addresses */
    label_dummy = *label_head;
    while (label_dummy != NULL) {
        if (label_dummy->type == DATA) {
            /* Add the ICF to the data address */
            label_dummy->addr = label_dummy->addr + *ICF;
        }
        label_dummy = label_dummy->next;
    }
    /* Close the input file */
    fclose(fp);

    return SUCCESS; /* Success */
}
