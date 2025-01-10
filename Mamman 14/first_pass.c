#include "first_pass.h"
#include "errors.h"
#include <string.h>
#include "tables.h"
int valid_label(char *label) {
    int i, length = strlen(label);

    if (length == 0 || length >= MAX_LABEL_SIZE) return 0; /* Invalid Length */

    if (!isalpha(label[0])) return 0; /* Must start with a letter */
    for (i = 1; i < length; i++) {
        if (!isalnum(label[i])) return 0; /* Must contain only letters and digits */
    }
    return 1;
}

int encode_string(char *line, int *DC, int IC, DataList *data_list, const char *file_name, int line_count) {
    char *start, *end;
    start = strchr(line, '"'); /* Find the first quote */

    if (!start) {
        /* print_ext_error(ERROR_STRING_MISSING_QUOTES, file_name, line_count); */
        return 0; /* Missing opening quote */
    }

    start++; /* Move past the opening quote */
    end = strchr(start, '"'); /* Find the closing quote */

    if (!end) {
        /* print_ext_error(ERROR_STRING_MISSING_QUOTES, file_name, line_count); */
        return 0; /* Missing closing quote */
    }

    /* Encode each character in the string and add to the data list */
    while (start < end) {
        add_data_node(data_list, IC + *DC, (int)(*start)); /* Add ASCII value to the list with IC + DC address */
        (*DC)++;
        start++;
    }

    /* Add null terminator */
    add_data_node(data_list, IC + *DC, 0); /* Null terminator is 0 */
    (*DC)++;

    return 1; /* Success */
}

int handle_extern(char *line, label_table **label_head, const char *file_name, int line_count) {
    char *token = strtok(line + 7, " "); /* Skip '.extern' and tokenize */
    char *after_label;
    token[strcspn(token, "\n")] = '\0'; /* Remove newline if present */
    if (!isValidLabel(token))
    {
        print_ext_error(ERROR_LABEL_INVALID, file_name, line_count);
        return 0;
    }

    if (token && !isspace(*token) && token != '\0') {
        token[strcspn(token, "\n")] = '\0'; /* Remove newline if present */
        after_label = strtok(NULL, " "); /* Skip the rest of the line */
        if (after_label && after_label != '\0' && !isspace(after_label))
        {
            print_ext_error(ERROR_EXTRA_TEXT, file_name, line_count);
            return 0;
        }
        add_label_list(label_head, token, 0, line_count, EXTERN, file_name);
        return 1;
    }
    print_ext_error(ERROR_MISSING_EXTERNAL_NAME, file_name, line_count);
    return 0;
}

int encode_data(char *line, int *DC, int IC, DataList *data_list, const char *file_name, int line_count) {
    char *token = strtok(line + 5, ", "); /* Skip '.data' and tokenize */
    int value;

    while (token) {
        if (sscanf(token, "%d", &value) != 1) {
            return 0; /* Invalid data value */
        }

        add_data_node(data_list, IC + *DC, value); /* Add data value to list with IC + DC address */
        (*DC)++;

        token = strtok(NULL, ", ");
    }

    return 1; /* Success */
}


void print_label_list(label_table *label_head) {
    label_table *current = label_head;
    printf("\nLabel List:\n");
    while (current) {
        printf("Label: %s, Address: %d, Type: %d\n", current->name, current->addr, current->type);
        current = current->next;
    }
}

int first_pass(char *file_name, DataList *data_list, InstructionList *instruction_list, label_table **label_head, int* ICF, int* DCF) {
    int IC = INITIAL_IC, DC = INITIAL_DC, line_count = 0;
    FILE *fp;
    char line[MAX_LINE_LENGTH], *after_label, label[MAX_LABEL_SIZE];
    int DID_FAIL = FALSE, label_length;
    label_table *label_dummy = NULL;
    Instruction *inst;

    if((fp = fopen(file_name, "r")) == NULL){
        print_internal_error(ERROR_FILE_OPEN_SOURCE);
        return 0;
    }

    while (fgets(line, sizeof(line), fp) != NULL && IC+DC < MAX_MEMORY_SIZE) {
        line_count++;

        after_label = strchr(line, ':'); /*skip : and space*/
        if (after_label != NULL) {

            label_length = after_label - line; /* Calculate label length  dont count : */
            if (label_length >= MAX_LABEL_SIZE) {
                print_ext_error(ERROR_LABEL_TOO_LONG, file_name, line_count);
                DID_FAIL = TRUE;
                continue;
            }
            strncpy(label, line, label_length);
            label[label_length] = '\0';

            if (!isValidLabel(label))
            {
                DID_FAIL = TRUE;
                print_ext_error(ERROR_LABEL_INVALID, file_name, line_count);
                continue;
            }

            after_label+=2; /*skip : and space*/
        } else {
            after_label = line; /* No label, start from the beginning of the line */
        }

        if (strncmp(after_label, ".data", 5) == 0) {
            if (label[0] != '\0') {
                add_label_list(label_head, label, DC, line_count, DATA, file_name);
            }
            if (!encode_data(after_label, &DC, IC, data_list, file_name, line_count)) {
                DID_FAIL = TRUE;
                continue;
            }
        } else if (strncmp(after_label, ".string", 7) == 0) {
            if (label[0] != '\0') {
                add_label_list(label_head, label, DC, line_count, DATA, file_name);
            }
            if (!encode_string(after_label, &DC, IC, data_list, file_name, line_count)) {
                DID_FAIL = TRUE;
                continue;
            }
        } else if (strncmp(after_label, ".extern", 7) == 0) {
            if (!handle_extern(after_label, label_head, file_name, line_count))
            {
                DID_FAIL = TRUE;
                continue;
            }
        } else if (strncmp(after_label, ".entry", 6) == 0) {
            continue; /* Ignore .entry directive we will address that in second pass*/
        } else {
            if (label[0] != '\0') {
                add_label_list(label_head, label, IC, line_count, CODE, file_name);
            }

            if (parse_and_process_instruction(after_label, instruction_list, file_name, line_count) != 0) {
                DID_FAIL = TRUE;
                continue;
            }

            inst = &instruction_list->tail->instruction;
            IC += calculate_words(inst, file_name, line_count);
        }
        memset(label, '\0', sizeof(label));
        /* printf("IC: %d, DC: %d\n", IC, DC); */
    }
    if (IC + DC >= MAX_MEMORY_SIZE) {
        print_internal_error(ERROR_MEMORY_OVERFLOW);
        DID_FAIL = TRUE;
    }
    if (DID_FAIL)
    {
        print_internal_error("First pass failed");
        return 0;
    }
    *ICF = IC;
    *DCF = DC;
    /* print_label_list(label_head); */
    printf("ICF: %d, DCF: %d\n", *ICF, *DCF);
    /* Update data addresses */
    label_dummy = *label_head;
    while (label_dummy != NULL)
    {
        if (label_dummy->type == DATA)
        {
            label_dummy->addr = label_dummy->addr + *ICF;
        }
        label_dummy = label_dummy->next;
    }
    /*print_data_list(data_list);
    print_label_list(label_head);
    print_instruction_list(instruction_list);
    test_func(data_list); */
    fclose(fp);
    return 1;
}
