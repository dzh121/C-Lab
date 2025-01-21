#include "first_pass.h"
#include "errors.h"
#include <string.h>
#include "tables.h"

int encode_string(char *line, int *DC, int IC, DataList *data_list, char *file_name, int line_count) {
    char *start, *end;
    if (!line) {
        print_ext_error(ERROR_STRING_MISSING_QUOTES, file_name, line_count);
        return 0; /* Missing opening quote */
    }
    start = strchr(line, '"'); /* Find the first quote */
    if (!start) {
        print_ext_error(ERROR_STRING_MISSING_QUOTES, file_name, line_count);
        return 0; /* Missing opening quote */
    }

    start++; /* Move past the opening quote */
    end = strchr(start, '"'); /* Find the closing quote */

    if (!end) {
        print_ext_error(ERROR_STRING_MISSING_QUOTES, file_name, line_count);
        return 0; /* Missing closing quote */
    }

    /* Encode each character in the string and add to the data list */
    while (start < end) {
        add_data_node(data_list, *DC, (int)(*start)); /* Add ASCII value to the list with DC address */
        (*DC)++;
        start++;
    }

    /* Add null terminator */
    add_data_node(data_list, *DC, 0); /* Null terminator is 0 */
    (*DC)++;

    return 1; /* Success */
}

int handle_extern(char *line, label_table **label_head, char *file_name, int line_count) {
    char *token;
    char *after_label;
    if (!line) {
        print_ext_error(ERROR_MISSING_EXTERNAL_NAME, file_name, line_count);
        return 0;
    }
    token = strtok(line + 7, " "); /* Get the first token */
    if (!token) {
        print_ext_error(ERROR_MISSING_EXTERNAL_NAME, file_name, line_count);
        return 0;
    }
    token[strcspn(token, "\n")] = '\0'; /* Remove newline if present */
    if (!isValidLabel(token))
    {
        print_ext_error(ERROR_LABEL_INVALID, file_name, line_count);
        return 0;
    }

    /* Check for extra text */
    after_label = strtok(NULL, " ");
    if (after_label) {
        print_ext_error(ERROR_EXTRA_TEXT, file_name, line_count);
        return 0;
    }

    add_label_list(label_head, token, 0, line_count, EXTERN, file_name);
    return 1;
}

int encode_data(char *line, int *DC, int IC, DataList *data_list, char *file_name, int line_count) {
    char *token = strtok(line + 5, ", "); /* Skip '.data' and tokenize */
    int value;

    while (token) {
        if (sscanf(token, "%d", &value) != 1) {
            return 0; /* Invalid data value */
        }

        add_data_node(data_list, *DC, value); /* Add data value to list with DC address */
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
    int did_fail = FALSE, label_length;
    label_table *label_dummy = NULL;
    DataNode *current_data;
    Instruction *inst;

    if((fp = fopen(file_name, "r")) == NULL){
        print_internal_error(ERROR_FILE_OPEN_SOURCE);
        return 0;
    }
    memset(label, '\0', sizeof(label));
    while (fgets(line, sizeof(line), fp) != NULL && IC+DC < MAX_MEMORY_SIZE) {
        line_count++;

        after_label = strchr(line, ':'); /*skip : and space*/
        if (after_label != NULL) {

            label_length = after_label - line; /* Calculate label length  dont count : */
            if (label_length >= MAX_LABEL_SIZE) {
                print_ext_error(ERROR_LABEL_TOO_LONG, file_name, line_count);
                did_fail = TRUE;
                continue;
            }
            strncpy(label, line, label_length);
            label[label_length] = '\0';

            if (!isValidLabel(label))
            {
                did_fail = TRUE;
                print_ext_error(ERROR_LABEL_INVALID, file_name, line_count);
                continue;
            }

            after_label+=2; /*skip : and space*/
        } else {
            memset(label, '\0', sizeof(label));
            after_label = line; /* No label, start from the beginning of the line */
        }

        if (strncmp(after_label, ".data", 5) == 0) {
            if (label[0] != '\0') {
                add_label_list(label_head, label, DC, line_count, DATA, file_name);
            }
            if (!encode_data(after_label, &DC, IC, data_list, file_name, line_count)) {
                did_fail = TRUE;
                continue;
            }
        } else if (strncmp(after_label, ".string", 7) == 0) {
            if (label[0] != '\0') {
                add_label_list(label_head, label, DC, line_count, DATA, file_name);
            }
            if (!encode_string(after_label, &DC, IC, data_list, file_name, line_count)) {
                did_fail = TRUE;
                continue;
            }
        } else if (strncmp(after_label, ".extern", 7) == 0) {
            if (label[0] != '\0' && label[0] != '\n') {
                print_ext_warning(WARNING_LABEL_AT_START_EXTERN, file_name, line_count);
            }
            if (!handle_extern(after_label, label_head, file_name, line_count))
            {
                did_fail = TRUE;
                continue;
            }
        } else if (strncmp(after_label, ".entry", 6) == 0) {
            if (label[0] != '\0' && label[0] != '\n') {
                print_ext_warning(WARNING_LABEL_AT_START_ENTRY, file_name, line_count);
            }
            continue; /* Ignore .entry directive we will address that in second pass*/
        } else {
            if (label[0] != '\0') {
                add_label_list(label_head, label, IC, line_count, CODE, file_name);
            }

            if (parse_and_process_instruction(after_label, instruction_list, file_name, line_count) != 0) {
                did_fail = TRUE;
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
        did_fail = TRUE;
    }
    if (did_fail)
    {
        fclose(fp);
        return 0;
    }
    *ICF = IC;
    *DCF = DC;
    /* print_label_list(label_head); */
    /*printf("ICF: %d, DCF: %d\n", *ICF, *DCF); */

    /* Update data addresses */
    current_data = data_list->head;
    while (current_data != NULL) {
        current_data->address += *ICF;
        current_data = current_data->next;
    }

    /* Update label addresses */
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
