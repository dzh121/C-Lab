#include "../headers/code_translation.h"


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

Instruction *find_operation(char *name) {
    int i; /* Loop index */

    /* Search for the operation by name */
    for (i = 0; operations[i].name != NULL; i++) {
        if (strcmp(operations[i].name, name) == 0) {
            return &operations[i]; /* Return the operation */
        }
    }
    return NULL; /* Operation not found */
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
            break; /* No extra words needed */
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
            break; /* No extra words needed */
        default:
            print_ext_error(ERROR_INVALID_DESTINATION_ADDRESSING_MODE, file_name, line_number);
            break;
    }

    return words;
}

unsigned int build_first_word(Instruction *inst, char *file_name, int line_number) {
    unsigned int word = 0; /* Initialize the word to zero */
    int src_mode_after; /* Source addressing mode after encoding */
    int dest_mode_after; /* Destination addressing mode after encoding */
    int src_reg_after; /* Source register after encoding */
    int dest_reg_after; /* Destination register after encoding */

    /* Validate Instruction Pointer */
    if (!inst) {
        print_ext_error(ERROR_NULL_INSTRUCTION, file_name, line_number);
        return 0;
    }

    src_reg_after = inst->src_operand; /* Default to original values */
    dest_reg_after = inst->dest_operand; /* Default to original values */

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

    /* Validate ARE Bits (1, 2 ,4) */
    if (inst->are != 1 && inst->are != 2 && inst->are != 4) {
        print_ext_error(ERROR_INVALID_ARE_BITS, file_name, line_number);
        return 0;
    }
    /* Normalize the source and destination registers */
    if (inst->src_mode == REGISTER_DIRECT) {
        if (inst->src_operand < 0 || inst->src_operand > 7) {
            print_ext_error(ERROR_INVALID_SOURCE_REGISTER, file_name, line_number);
            return 0;
        }
    } else {
        src_reg_after = 0;
    }

    if (inst->dest_mode == REGISTER_DIRECT) {
        if (inst->dest_operand < 0 || inst->dest_operand > 7) {
            print_ext_error(ERROR_INVALID_DESTINATION_REGISTER, file_name, line_number);
            return 0;
        }
    } else {
        dest_reg_after = 0;
    }

    /* Normalize the source and destination addressing modes */
    switch (inst->src_mode) {
        case UNKNOWN: /* Unknown set to ZERO */
        case IMMEDIATE:
            src_mode_after = 0; /* Immediate Addressing Mode */
            break;
        case DIRECT:
            src_mode_after = 1; /* Direct Addressing Mode */
            break;
        case RELATIVE:
            src_mode_after = 2; /* Relative Addressing Mode */
            break;
        case REGISTER_DIRECT:
            src_mode_after = 3; /* Register Direct Addressing Mode */
            break;
        default:
            print_ext_error(ERROR_INVALID_SOURCE_ADDRESSING_MODE, file_name, line_number);
            return 0;
    }
    switch (inst->dest_mode) {
        case UNKNOWN: /* Unknown set to ZERO */
        case IMMEDIATE:
            dest_mode_after = 0; /* Immediate Addressing Mode */
            break;
        case DIRECT:
            dest_mode_after = 1; /* Direct Addressing Mode */
            break;
        case RELATIVE:
            dest_mode_after = 2; /* Relative Addressing Mode */
            break;
        case REGISTER_DIRECT:
            dest_mode_after = 3; /* Register Direct Addressing Mode */
            break;
        default:
            print_ext_error(ERROR_INVALID_DESTINATION_ADDRESSING_MODE, file_name, line_number);
            return 0;
    }
    word |= (inst->opcode & 0x3F) << 18; /* Opcode (6 bits) */
    word |= (src_mode_after & 0x3) << 16; /* Source Addressing Mode (2 bits) */
    word |= (src_reg_after & 0x7) << 13; /* Source Register (3 bits) */
    word |= (dest_mode_after & 0x3) << 11; /* Destination Addressing Mode (2 bits) */
    word |= (dest_reg_after & 0x7) << 8; /* Destination Register (3 bits) */
    word |= (inst->funct & 0x1F) << 3; /* Funct (5 bits) */
    word |= (inst->are & 0x7); /*(3 bits) ARE bits */
    return word;
}

unsigned int encode_operand(int operand, int type, char *file_name, int line_number) {
    unsigned int word = 0; /* Initialize the word to zero */

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
            word |= (0 << 3); /* Value is all zeros */
            break;
        default:
            print_ext_error(ERROR_INVALID_OPERAND_TYPE, file_name, line_number);
            return 0;
    }
    return word;
}

int process_instruction(Instruction *inst, char *file_name, int line_number, DataList *data_list,
                        label_table *label_head, int address) {
    unsigned int first_word = 0; /* Initialize the first word to zero */
    unsigned int operand_word = 0; /* Initialize the operand word to zero */
    label_table *label; /* Label pointer */
    int relative_distance; /* Relative distance between labels */
    int original_address = address; /* Original address for relative distance */

    if (!inst) {
        print_ext_error(ERROR_NULL_INSTRUCTION, file_name, line_number);
        return FAILURE;
    }

    /* Build the First Word */
    first_word = build_first_word(inst, file_name, line_number);

    add_data_node(data_list, address++, (int) first_word);

    /* If either source or destination is UNKNOWN, we skip */
    /* Process Source Operand */
    if (inst->src_mode == IMMEDIATE) {
        /* Encode Immediate Value */
        operand_word = encode_operand(inst->src_operand, IMMEDIATE, file_name, line_number);
        /* Add the encoded operand to the data list */
        add_data_node(data_list, address++, (int) operand_word);
    } else if (inst->src_mode == DIRECT) {
        /* We default as direct we will check if it is external */
        label = search_label_list(label_head, inst->src_label); /* Find label */

        if (!label) {
            print_ext_error(ERROR_UNDEFINED_SOURCE_LABEL, file_name, line_number);
            return FAILURE;
        }
        if (label->type == EXTERN) {
            operand_word = 1; /* ARE = 1, rest zero for EXTERNAL */
            add_address_to_label(label, address); /* Add the address to the label */
        } else {
            /* Encode the label address */
            operand_word = encode_operand(label->addr, DIRECT, file_name, line_number);
        }
        add_data_node(data_list, address++, operand_word);
    } else if (inst->src_mode == RELATIVE) {
        label = search_label_list(label_head, inst->src_label); /* Find label */
        if (!label) {
            print_ext_error(ERROR_UNDEFINED_SOURCE_LABEL, file_name, line_number);
            return FAILURE;
        }
        /* Calculate relative distance */
        relative_distance = label->addr - original_address;
        /* Encode the relative distance */
        operand_word = encode_operand(relative_distance, RELATIVE, file_name, line_number);
        /* Add the encoded operand to the data list */
        add_data_node(data_list, address++, operand_word);
    }

    /* Process Destination Operand */
    if (inst->dest_mode == IMMEDIATE) {
        /* Encode Immediate Value */
        operand_word = encode_operand(inst->dest_operand, IMMEDIATE, file_name, line_number);
        /* Add the encoded operand to the data list */
        add_data_node(data_list, address++, operand_word);
    } else if (inst->dest_mode == DIRECT) {
        /* We default as direct we will check if it is external */
        label = search_label_list(label_head, inst->dest_label); /* Find label */

        if (!label) {
            print_ext_error(ERROR_UNDEFINED_DEST_LABEL, file_name, line_number);
            return FAILURE;
        }
        if (label->type == EXTERN) {
            operand_word = 1; /* ARE = 1, rest zero for EXTERNAL */
            add_address_to_label(label, address);
        } else {
            /* Encode the label address */
            operand_word = encode_operand(label->addr, DIRECT, file_name, line_number);
        }
        add_data_node(data_list, address++, operand_word);
    } else if (inst->dest_mode == RELATIVE) {
        label = search_label_list(label_head, inst->dest_label); /* Find label */
        if (!label) {
            print_ext_error(ERROR_UNDEFINED_DEST_LABEL, file_name, line_number);
            return FAILURE;
        }
        /* Calculate relative distance */
        relative_distance = label->addr - original_address;
        /* Encode the relative distance */
        operand_word = encode_operand(relative_distance, RELATIVE, file_name, line_number);
        /* Add the encoded operand to the data list */
        add_data_node(data_list, address++, operand_word);
    }

    return SUCCESS;
}


int parse_instruction(char *line, InstructionList *instruction_list, char *file_name, int line_number) {
    char name[MAX_LINE_LENGTH]; /* Instruction Name */
    char label[MAX_LINE_LENGTH]; /* Label Name */
    int number_length = 0; /* Length of the number */
    Instruction *inst = handle_malloc(sizeof(Instruction)); /* Allocate memory for the instruction */
    Instruction *found_inst; /* Found instruction */

    /* Extract the Instruction Name */
    line = getWord(line, name); /* Get the instruction name */

    /* Find the instruction */
    found_inst = find_operation(name);
    if (!found_inst) {
        print_ext_error(ERROR_INSTRUCTION_NOT_FOUND, file_name, line_number);
        free(inst);
        inst = NULL;
        return FAILURE;
    }

    /* Copy the found instruction */
    memset(inst, 0, sizeof(Instruction));
    memcpy(inst, found_inst, sizeof(Instruction));

    /* Check for illegal comma */
    if (*line == ',') {
        print_ext_error(ERROR_ILLEGAL_COMMA, file_name, line_number);
        free(inst);
        return FAILURE;
    }

    /* Skip spaces */
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
            break;
        /* Instructions with One Operand */
        case 5: /* clr */
        case 9: /* jmp */
        case 12: /* red */
        case 13: /* prn */
            /* Parse Source Operand */
            MISSING_PARM(line[0], file_name, line_number);

        /* Check for illegal comma */
            if (line[0] == ',') {
                print_ext_error(ERROR_ILLEGAL_COMMA, file_name, line_number);
                free(inst);
                inst = NULL;
                return FAILURE;
            }

        /* Parse Source Operand */
            if (line[0] == '#') {
                /* Immediate Value */
                inst->dest_mode = IMMEDIATE;

                /* Parse the number */
                number_length = getNum(line + 1, &inst->dest_operand, file_name, line_number);
                if (number_length == 0) {
                    free(inst);
                    inst = NULL;
                    return FAILURE;
                }
                /* Move to the next part of the string */
                line += (1 + number_length);
            } else if (line[0] == '&') {
                /* Relative Address */
                inst->dest_mode = RELATIVE;
                line++; /* Move past the '&' */
                /* Get the label */
                line = getWord(line, label);
                /* Copy the label */
                strncpy(inst->dest_label, label, MAX_LINE_LENGTH - 1);
            } else if (line[0] == 'r') {
                inst->dest_mode = REGISTER_DIRECT;
                if (!isdigit(line[1])) {
                    print_ext_error(ERROR_INVALID_SOURCE_REGISTER, file_name, line_number);
                    free(inst);
                    return FAILURE;
                }
                if (line[2] && isdigit(line[2])) {
                    print_ext_error(ERROR_INVALID_SOURCE_REGISTER, file_name, line_number);
                    free(inst);
                    return FAILURE;
                }
                inst->dest_operand = atoi(line + 1);
                line += 2;
            } else if (isdigit(line[0])) {
                print_ext_error(ERROR_INVALID_SOURCE_REGISTER, file_name, line_number);
                free(inst);
                inst = NULL;
                return FAILURE;
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
                /* Immediate Value */
                inst->src_mode = IMMEDIATE;
                /* Parse the number */
                number_length = getNum(line + 1, &inst->src_operand, file_name, line_number);
                if (number_length == 0) {
                    free(inst);
                    return FAILURE;
                }
                /* Move to the next part of the string */
                line += 1 + number_length;
            } else if (line[0] == '&') {
                /* Relative Address */
                inst->src_mode = RELATIVE;
                /* Move past the '&' */
                line++;
                /* Get the label */
                line = getWord(line, label);
                /* Copy the label */
                strncpy(inst->src_label, label, MAX_LINE_LENGTH - 1);
            } else if (line[0] == 'r') {
                inst->src_mode = REGISTER_DIRECT;
                if (!isdigit(line[1])) {
                    print_ext_error(ERROR_INVALID_SOURCE_REGISTER, file_name, line_number);
                    free(inst);
                    return FAILURE;
                }
                if (line[2] && isalnum(line[2])) {
                    print_ext_error(ERROR_INVALID_SOURCE_REGISTER, file_name, line_number);
                    free(inst);
                    return FAILURE;
                }
                inst->src_operand = atoi(line + 1);
                line += 2;
            } else if (isalpha(line[0]) || line[0] == '_') {
                /* Check if the first character is a letter or underscore */
                /* Direct Address */
                inst->src_mode = DIRECT;
                /* Get the label */
                line = getWord(line, label);
                /* Copy the label*/
                strncpy(inst->src_label, label, MAX_LINE_LENGTH - 1);
            }else {
                if (line[0] == ',')
                    print_ext_error(ERROR_ILLEGAL_COMMA, file_name, line_number);
                else
                    print_ext_error(ERROR_INVALID_SOURCE_OPERAND, file_name, line_number);
                free(inst);
                inst = NULL;
                return FAILURE;
            }
             line = skipSpaces(line);
             /* check for ',' */
            if (*line != ',') {
                print_ext_error(ERROR_MISSING_COMMA, file_name, line_number);
                free(inst);
                return FAILURE;
            }
            /* Move to the next part of the string */
            line = skipSpaces(line + 1);

            /* Check for missing parm */
            MISSING_PARM(line[0], file_name, line_number);

            /* Parse Destination Operand */
            if (line[0] == '#') {
                /* Immediate Value */
                inst->dest_mode = IMMEDIATE;
                /* Parse the number */
                number_length = getNum(line + 1, &inst->dest_operand, file_name, line_number);
                /* Check for invalid number */
                if (number_length == 0) {
                    free(inst);
                    return FAILURE;
                }
                /* Move to the next part of the string */
                line += 1 + number_length;
            } else if (line[0] == '&') {
                /* Relative Address */
                inst->dest_mode = RELATIVE;
                /* Move past the '&' */
                line++;
                /* Get the label */
                line = getWord(line, label);
                /* Copy the label */
                strncpy(inst->dest_label, label, MAX_LINE_LENGTH - 1);
            } else if (line[0] == 'r') {
                /* Register Direct */
                inst->dest_mode = REGISTER_DIRECT;
                /* Check if the register is valid */
                if (!isdigit(line[1])) {
                    print_ext_error(ERROR_INVALID_DESTINATION_REGISTER, file_name, line_number);
                    free(inst);
                    return FAILURE;
                }
                /* Check for invalid register */
                if (line[2] && isalnum(line[2])) {
                    print_ext_error(ERROR_INVALID_DESTINATION_REGISTER, file_name, line_number);
                    free(inst);
                    return FAILURE;
                }
                /* Get the register number */
                inst->dest_operand = atoi(line + 1);
                line += 2;
            } else if (isalpha(line[0]) || line[0] == '_') {
                /* Check if the first character is a letter or underscore */
                /* Direct Address */
                inst->dest_mode = DIRECT;
                /* Get the label */
                line = getWord(line, label);
                /* Copy the label*/
                strncpy(inst->dest_label, label, MAX_LINE_LENGTH - 1);
            } else {
                if (line[0] == ',')
                    print_ext_error(ERROR_ILLEGAL_COMMA, file_name, line_number);
                else
                    print_ext_error(ERROR_INVALID_DESTINATION_OPERAND, file_name, line_number);
                free(inst);
                inst = NULL;
                return FAILURE;
            }
            line = skipSpaces(line);
            /* Check for extraneous text */
            EXTRANEOUS_TEXT(*line, file_name, line_number);
            break;
    }
    /* Add the instruction to the list */
    add_instruction(instruction_list, inst);
    /* Free the instruction */
    free(inst);
    return SUCCESS;
}


char *getWord(char *line, char *word) {
    int i = 0; /* Index for the word */

    /* Check for NULL pointers */
    if (!line || !word) {
        return NULL;
    }

    line = skipSpaces(line); /* Skip leading spaces */

    /* Extract word until space, comma, or end of string */
    while (*line != ' ' && *line != ',' && *line != '\0' && *line != '\n' && i < MAX_LINE_LENGTH - 1) {
        word[i++] = *line++;
    }
    word[i] = '\0'; /* Null-terminate the word */

    return line; /* Return pointer to the next part of the string */
}

int getNum(char *line, int *num, char *file_name, int line_number) {
    char buffer[MAX_LINE_LENGTH] = {0}; /* Buffer to store the number */
    int i = 0; /* Index for the buffer */
    int length = 0; /* Length of the number */
    char *endptr; /* Pointer to the end of the number */

    if (!line || !num) {
        print_ext_error(ERROR_INVALID_NUMBER, file_name, line_number);
        return 0;
    }

   line = skipSpaces(line);

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
    /* Skip leading spaces */
    while (isspace(*line)) {
        line++;
    }
    return line;
}

void removeSpaces(char *p) {
    char *write = p; /* Pointer to write to */
    char *read = p; /* Pointer to read from */

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
