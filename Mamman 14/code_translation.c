#include "code_translation.h"


Instruction operations[] = {
    {"mov", 0, 0, 0, 0, 0, 0, 4},
    {"cmp", 1, 0, 0, 0, 0, 0, 4},
    {"add", 2, 1, 0, 0, 0, 0, 4},
    {"sub", 2, 2, 0, 0, 0, 0, 4},
    {"lea", 4, 0, 0, 0, 0, 0, 4},
    {"clr", 5, 1, 0, 0, 0, 0, 4},
    {"not", 5, 2, 0, 0, 0, 0, 4},
    {"inc", 5, 3, 0, 0, 0, 0, 4},
    {"dec", 5, 4, 0, 0, 0, 0, 4},
    {"jmp", 9, 1, 0, 0, 0, 0, 4},
    {"bne", 9, 2, 0, 0, 0, 0, 4},
    {"jsr", 9, 3, 0, 0, 0, 0, 4},
    {"red", 12, 0, 0, 0, 0, 0, 4},
    {"prn", 13, 0, 0, 0, 0, 0, 4},
    {"rts", 14, 0, 0, 0, 0, 0, 4},
    {"stop", 15, 0, 0, 0, 0, 0, 4},
    {NULL, -1, 0, 0, 0, 0, 0, 0}
};

Instruction *find_operation(char *name,char *file_name, int line_number) {
    int i; /* Loop index */

    for (i = 0; operations[i].name != NULL; i++) {
        if (strcmp(operations[i].name, name) == 0) {
            return &operations[i];
        }
    }

    print_ext_error(ERROR_INSTRUCTION_NOT_FOUND, file_name, line_number);
    return NULL;
}

int calculate_words(Instruction *inst, char *file_name, int line_number) {
    int words = 1; /* Base instruction word is always present */

    /* Calculate additional words for source operand */
    switch (inst->src_mode) {
        case IMMEDIATE:
        case DIRECT:
        case RELATIVE:
            words++;
        break;
        case REGISTER_DIRECT:
        case UNKNOWN: /* Default to skipping for UNKNOWN */
            break;/* No extra words needed */
        default:
            print_ext_error(ERROR_INVALID_SOURCE_ADDRESSING_MODE, file_name, line_number);
        break;
    }

    /* Calculate additional words for destination operand */
    switch (inst->dest_mode) {
        case IMMEDIATE:
        case DIRECT:
        case RELATIVE:
            words++;
        break;
        case REGISTER_DIRECT:
        case UNKNOWN: /* Default to skipping for UNKNOWN */
            break;/* No extra words needed */
        default:
            print_ext_error(ERROR_INVALID_DESTINATION_ADDRESSING_MODE, file_name, line_number);
        break;
    }

    return words;
}

unsigned int build_first_word(Instruction *inst, char *file_name, int line_number) {
    unsigned int word = 0;
    int src_mode_after = inst-> src_mode;
    int dest_mode_after = inst-> dest_mode;
    int src_reg_after = inst-> src_operand;
    int dest_reg_after = inst-> dest_operand;

    /* Validate Instruction Pointer */
    if (!inst) {
        print_ext_error(ERROR_NULL_INSTRUCTION, file_name, line_number);
        return 0;
    }

    /* Validate Opcode Range (6 bits, 0–MAX_OPCODE) */
    if (inst->opcode < 0 || inst->opcode > MAX_OPCODE) {
        print_ext_error(ERROR_INVALID_OPCODE, file_name, line_number);
        return 0;
    }

    /* Validate Funct Range (5 bits, 0–MAX_FUNCT) */
    if (inst->funct < 0 || inst->funct > MAX_FUNCT) {
        print_ext_error(ERROR_INVALID_FUNCT, file_name, line_number);
        return 0;
    }

    /* Validate Source Addressing Mode (2 bits, 0–4) */
    if (inst->src_mode < 0 || inst->src_mode > 4) {
        print_ext_error(ERROR_INVALID_SOURCE_ADDRESSING_MODE, file_name, line_number);
        return 0;
    }

    /* Validate Destination Addressing Mode (2 bits, 0–4) */
    if (inst->dest_mode < 0 || inst->dest_mode > 4) {
        print_ext_error(ERROR_INVALID_DESTINATION_ADDRESSING_MODE, file_name, line_number);
        return 0;
    }

    /* Validate ARE Bits (3 bits, 0–7) */
    if (inst->are < 0 || inst->are > 7) {
        print_ext_error(ERROR_INVALID_ARE_BITS, file_name, line_number);
        return 0;
    }
    if (inst->src_mode != REGISTER_DIRECT) {
        src_reg_after = 0;
    }
    if (inst->dest_mode != REGISTER_DIRECT) {
        dest_reg_after = 0;
    }
    switch (inst->src_mode)
    {
        case IMMEDIATE:
            src_mode_after = 0;
            break;
        case DIRECT:
            src_mode_after = 1;
            break;
        case RELATIVE:
            src_mode_after = 2;
            break;
        case REGISTER_DIRECT:
            src_mode_after = 3;
            break;
    }
    switch (inst->dest_mode)
    {
        case IMMEDIATE:
            dest_mode_after = 0;
            break;
        case DIRECT:
            dest_mode_after = 1;
            break;
        case RELATIVE:
            dest_mode_after = 2;
            break;
        case REGISTER_DIRECT:
            dest_mode_after = 3;
            break;
    }
    word |= (inst->opcode & 0x3F) << 18; /* Opcode (6 bits) */
    word |= (src_mode_after & 0x3) << 16;      /* Source Addressing Mode (2 bits) */
    word |= (src_reg_after & 0x7) << 13;       /* Source Register (3 bits) */
    word |= (dest_mode_after & 0x3) << 11;     /* Destination Addressing Mode (2 bits) */
    word |= (dest_reg_after & 0x7) << 8;       /* Destination Register (3 bits) */
    word |= (inst->funct & 0x1F) << 3;   /* Funct (5 bits) */
    word |= (4 & 0x7);                 /* ARE Bits (3 bits) */
    return word;
}

unsigned int encode_operand(int operand, int type, char *file_name, int line_number) {
    unsigned int word = 0;

    switch (type) {
        case IMMEDIATE: /* Immediate Value (e.g., #5) */
            word |= 4; /* ARE = A (4), Bits 2–0 */
            word |= ((operand & 0x1FFFFF) << 3); /* Immediate value in Bits 3–23 */
        break;
        case DIRECT: /* Label Address (e.g., 130) */
            word |= 2; /* ARE = R (2), Bits 2–0 */
            word |= ((operand & 0x1FFFFF) << 3); /* Address in Bits 3–23 */
        break;
        case RELATIVE: /* Relative Distance (e.g., &MAIN with distance 5) */
            word |= 4; /* ARE = A (4), Bits 2–0 */
            word |= ((operand & 0x1FFFFF) << 3); /* Distance in Bits 3–23 */
        break;
        case EXTERNAL: /* External Symbol */
            word |= 1; /* ARE = E (1), Bits 2–0 */
            word |= (0 << 3);  /* Value is all zeros */
        break;

        default:
            print_ext_error(ERROR_INVALID_OPERAND_TYPE, file_name, line_number);
        return 0;
    }
    return word;
}

int process_instruction(
    Instruction *inst,
    char *file_name,
    int line_number,
    DataList *data_list,
    label_table *label_head,
    int address) {
    int word_count = 1; /* For the first word */
    unsigned int first_word = 0;
    unsigned int operand_word = 0;
    label_table *label;
    int relative_distance;
    int original_address = address;


    /* Build the First Word */
    first_word = build_first_word(
        inst,
        file_name,
        line_number
    );

    add_data_node(data_list, address++, first_word);

    /* Process Source Operand */
    if (inst->src_mode == IMMEDIATE) {
        operand_word = encode_operand(inst->src_operand, IMMEDIATE, file_name, line_number);
        add_data_node(data_list, address++, operand_word);
        word_count++;
    } else if (inst->src_mode == DIRECT) {
        label = search_label_list(label_head, inst->src_label); /* Find label */
        if (!label) {
            print_ext_error(ERROR_UNDEFINED_SOURCE_LABEL, file_name, line_number);
            return -1;
        }
        if (label->type == EXTERN) {
            operand_word = 1; /* ARE = 1, rest zero for EXTERNAL */
            if (label->type == EXTERN) {
                add_address_to_label(label, address);
            }
        } else {
            operand_word = encode_operand(label->addr, DIRECT, file_name, line_number);
        }
        add_data_node(data_list, address++, operand_word);
        word_count++;
    } else if (inst->src_mode == RELATIVE) {
        label = search_label_list(label_head, inst->src_label); /* Find label */
        if (!label) {
            print_ext_error(ERROR_UNDEFINED_SOURCE_LABEL, file_name, line_number);
            return -1;
        }
        /* Calculate relative distance */
        relative_distance = label->addr - original_address;
        operand_word = encode_operand(relative_distance, RELATIVE, file_name, line_number);
        add_data_node(data_list, address++, operand_word);
        word_count++;
    }

    /* Process Destination Operand */
    if (inst->dest_mode == IMMEDIATE) {
        operand_word = encode_operand(inst->dest_operand, IMMEDIATE, file_name, line_number);
        add_data_node(data_list, address++, operand_word);
        word_count++;
    } else if (inst->dest_mode == DIRECT) {
        label = search_label_list(label_head, inst->dest_label); /* Find label */
        if (!label) {
            print_ext_error(ERROR_UNDEFINED_DEST_LABEL, file_name, line_number);
            return -1;
        }
        if (label->type == EXTERN) {
            operand_word = 1; /* ARE = 1, rest zero for EXTERNAL */
            if (label->type == EXTERN) {
                add_address_to_label(label, address);
            }
        } else {
            operand_word = encode_operand(label->addr, DIRECT, file_name, line_number);
        }
        add_data_node(data_list, address++, operand_word);
        word_count++;
    } else if (inst->dest_mode == RELATIVE) {
        label = search_label_list(label_head, inst->dest_label); /* Find label */
        if (!label) {
            print_ext_error(ERROR_UNDEFINED_DEST_LABEL, file_name, line_number);
            return -1;
        }
        /* Calculate relative distance */
        relative_distance = label->addr - original_address;
        operand_word = encode_operand(relative_distance, RELATIVE, file_name, line_number);
        add_data_node(data_list, address++, operand_word);
        word_count++;
    }


    return word_count; /* Current word count after first word */
}


int parse_and_process_instruction(
    char *line,
    InstructionList *instruction_list,
    char *file_name,
    int line_number
) {
    char name[MAX_LINE_LENGTH];
    char label[MAX_LINE_LENGTH];
    int number_length = 0;

    Instruction *inst = handle_malloc(sizeof(Instruction));
    Instruction *found_inst;

    if (!inst) {
        perror("ERROR: Memory allocation failed for Instruction");
        return -1;
    }


    /* Extract the Instruction Name */
    line = getWord(line, name);

    found_inst = find_operation(name, file_name, line_number);
    if (!found_inst) {
        printf("ERROR: Instruction '%s' not found at line %d\n", name, line_number);
        free(inst);
        return -1;
    }

    memcpy(inst, found_inst, sizeof(Instruction));

    if (*line == ',') {
        print_ext_error(ERROR_ILLEGAL_COMMA, file_name, line_number);
        free(inst);
        return -1;
    }

    line = skipSpaces(line);

    memset(inst->src_label, 0, MAX_LINE_LENGTH); /* Clear previous labels */
    memset(inst->dest_label, 0, MAX_LINE_LENGTH); /* Clear previous labels */
    inst->src_mode = UNKNOWN; /* Default to UNKNOWN */
    inst->dest_mode = UNKNOWN; /* Default to UNKNOWN */
    inst->line_number = line_number;
    switch (inst->opcode) {
        /* Instructions with No Operands */
        case 14: /* rts */
        case 15: /* stop */
            EXTRANEOUS_TEXT(*line, file_name, line_number);
            add_instruction(instruction_list, inst);
            return 0;

        /* Instructions with One Operand */
        case 5: /* clr */
        case 9: /* jmp */
        case 12: /* red */
        case 13: /* prn */
            MISSING_PARM(line[0], file_name, line_number);

            if (line[0] == ',') {
                print_ext_error(ERROR_ILLEGAL_COMMA, file_name, line_number);
                free(inst);
                return -1;
            }

            if (line[0] == '#') {
                inst->dest_mode = IMMEDIATE;

                number_length = getNum(line + 1, &inst->dest_operand, file_name, line_number);
                if (number_length == 0) {
                    free(inst);
                    return -1;
                }
                line += 1 + number_length;
            } else if (line[0] == '&') {
                inst->dest_mode = RELATIVE;
                line++;
                line = getWord(line, label);
                strncpy(inst->dest_label, label, MAX_LINE_LENGTH - 1);
            } else if (line[0] == 'r') {
                inst->dest_mode = REGISTER_DIRECT;
                if (!isdigit(line[1])) {
                    print_ext_error(ERROR_INVALID_SOURCE_REGISTER, file_name, line_number);
                    free(inst);
                    return -1;
                }
                inst->dest_operand = atoi(line + 1);
                line += 2;
            } else {
                inst->dest_mode = DIRECT;
                line = getWord(line, label);
                strncpy(inst->dest_label, label, MAX_LINE_LENGTH - 1);
            }
            line = skipSpaces(line);
            EXTRANEOUS_TEXT(*line, file_name, line_number);
            break;

        /* Instructions with Two Operands */
        default:
            MISSING_PARM(line[0], file_name, line_number);

            /* Parse Source Operand */
            if (line[0] == '#') {
                inst->src_mode = IMMEDIATE;
                number_length = getNum(line + 1, &inst->src_operand, file_name, line_number);
                if (number_length == 0) {
                    free(inst);
                    return -1;
                }
                line += 1 + number_length;
            } else if (line[0] == '&') {
                inst->src_mode = RELATIVE;
                line++;
                line = getWord(line, label);
                strncpy(inst->src_label, label, MAX_LINE_LENGTH - 1);
            } else if (line[0] == 'r') {
                inst->src_mode = REGISTER_DIRECT;
                inst->src_operand = atoi(line + 1);
                line += 2;
            } else {
                inst->src_mode = DIRECT;
                line = getWord(line, label);
                strncpy(inst->src_label, label, MAX_LINE_LENGTH - 1);
            }

            if (*line != ',') {
                print_ext_error(ERROR_MISSING_COMMA, file_name, line_number);
                free(inst);
                return -1;
            }
            line = skipSpaces(line + 1);

            /* Parse Destination Operand */
            MISSING_PARM(line[0], file_name, line_number);

            if (line[0] == '#') {
                inst->dest_mode = IMMEDIATE;
                number_length = getNum(line + 1, &inst->dest_operand, file_name, line_number);
                if (number_length == 0) {
                    free(inst);
                    return -1;
                }
                line += 1 + number_length;
            } else if (line[0] == '&') {
                inst->dest_mode = RELATIVE;
                line++;
                line = getWord(line, label);
                strncpy(inst->dest_label, label, MAX_LINE_LENGTH - 1);
            } else if (line[0] == 'r') {
                inst->dest_mode = REGISTER_DIRECT;
                inst->dest_operand = atoi(line + 1);
                line += 2;
            } else {
                inst->dest_mode = DIRECT;
                line = getWord(line, label);
                strncpy(inst->dest_label, label, MAX_LINE_LENGTH - 1);
            }

            EXTRANEOUS_TEXT(*line, file_name, line_number);
            break;
    }

    add_instruction(instruction_list, inst);
    return 0;
}


char *getWord(char *line, char *word) {
    int i = 0;

    /* Skip leading spaces */
    while (*line == ' ') {
        line++;
    }

    /* Extract word until space, comma, or end of string */
    while (*line != ' ' && *line != ',' && *line != '\0' && *line != '\n' && i < MAX_LINE_LENGTH - 1) {
        word[i++] = *line++;
    }
    word[i] = '\0'; /* Null-terminate the word */

    return line; /* Return pointer to the next part of the string */
}

int getNum(char *line, int *num, char *file_name, int line_number) {
    char buffer[MAX_LINE_LENGTH] = {0}; /* Buffer to store the number */
    int i = 0;
    int length = 0;
    char *endptr;

    if (!line || !num) {
        print_ext_error(ERROR_INVALID_NUMBER, file_name, line_number);
        return 0;
    }

    /* Skip leading spaces */
    while (isspace(*line)) {
        line++;
    }

    /* Check if the first character is '-' or '+' for negative/positive numbers */
    if (*line == '-' || *line == '+') {
        buffer[i++] = *line++;
        length++;
    }

    /* Parse digits until ',' or end of string */
    while (*line != ',' && *line != '\0' && !isspace(*line)) {
        if (!isdigit(*line)) {
            /* Invalid character in number */
            print_ext_error(ERROR_INVALID_NUMBER, file_name, line_number);
            return 0; /* Indicate an error occurred */
        }
        buffer[i++] = *line++;
        length++;
    }

    buffer[i] = '\0'; /* Null-terminate the buffer */

    /* Check if buffer contains only a valid number */
    if (i == 0 || (i == 1 && (buffer[0] == '-' || buffer[0] == '+'))) {
        /* No valid digits after sign or empty buffer */
        print_ext_error(ERROR_INVALID_NUMBER, file_name, line_number);
        return 0; /* Indicate an error occurred */
    }

    /* Convert the extracted string to an integer */
    *num = strtol(buffer, &endptr, 10);

    /* Ensure full parsing without leftover characters */
    if (*endptr != '\0') {
        print_ext_error(ERROR_INVALID_NUMBER, file_name, line_number);
        return 0;
    }

    return length; /* Indicate success */
}

char *skipSpaces(char *line) {
    while (isspace(*line)) {
        line++;
    }
    return line;
}
void removeSpaces(char *p) {
    char *write = p; /* Pointer to write to */
    char *read = p;  /* Pointer to read from */

    while (*read != '\0') {
        /* Copy non-space characters */
        if (!isspace(*read)) {
            *write = *read; /* Copy the character */
            write++; /* Move the write pointer */
        }
        read++; /* Move the read pointer */
    }
    *write = '\0'; /* Null-terminate the modified string */
}
