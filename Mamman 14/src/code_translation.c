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

Instruction* find_operation(char* name)
{
	int i; /* Loop index */

	/* Search for the operation by name */
	for (i = 0; operations[i].name != NULL; i++)
	{
		if (strcmp(operations[i].name, name) == 0)
		{
			return &operations[i]; /* Return the operation */
		}
	}
	return NULL; /* Operation not found */
}

int calculate_words(Instruction* inst, char* file_name, int line_number)
{
	int words = 1; /* Base instruction word is always present */

	/* Calculate additional words for source operand */
	switch (inst->src_mode)
	{
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
	switch (inst->dest_mode)
	{
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

unsigned int build_first_word(Instruction* inst, char* file_name, int line_number)
{
	unsigned int word = 0; /* Initialize the word to zero */
	int src_mode_after; /* Source addressing mode after encoding */
	int dest_mode_after; /* Destination addressing mode after encoding */
	int src_reg_after; /* Source register after encoding */
	int dest_reg_after; /* Destination register after encoding */

	/* Validate Instruction Pointer */
	if (!inst)
	{
		print_ext_error(ERROR_NULL_INSTRUCTION, file_name, line_number);
		return 0;
	}

	src_reg_after = inst->src_operand; /* Default to original values */
	dest_reg_after = inst->dest_operand; /* Default to original values */

	/* Validate Opcode Range (6 bits, 0–MAX_OPCODE) */
	if (inst->opcode < 0 || inst->opcode > MAX_OPCODE)
	{
		print_ext_error(ERROR_INVALID_OPCODE, file_name, line_number);
		return 0;
	}

	/* Validate Funct Range (5 bits, 0–MAX_FUNCT) */
	if (inst->funct < 0 || inst->funct > MAX_FUNCT)
	{
		print_ext_error(ERROR_INVALID_FUNCT, file_name, line_number);
		return 0;
	}

	/* Validate Source Addressing Mode (2 bits, 0–4) */
	if (inst->src_mode < 0 || inst->src_mode > 4)
	{
		print_ext_error(ERROR_INVALID_SOURCE_ADDRESSING_MODE, file_name, line_number);
		return 0;
	}

	/* Validate Destination Addressing Mode (2 bits, 0–4) */
	if (inst->dest_mode < 0 || inst->dest_mode > 4)
	{
		print_ext_error(ERROR_INVALID_DESTINATION_ADDRESSING_MODE, file_name, line_number);
		return 0;
	}

	/* Validate ARE Bits (1, 2 ,4) */
	if (inst->are != 1 && inst->are != 2 && inst->are != 4)
	{
		print_ext_error(ERROR_INVALID_ARE_BITS, file_name, line_number);
		return 0;
	}
	/* Normalize the source and destination registers */
	if (inst->src_mode == REGISTER_DIRECT)
	{
		if (inst->src_operand < 0 || inst->src_operand > 7)
		{
			print_ext_error(ERROR_INVALID_SOURCE_REGISTER, file_name, line_number);
			return 0;
		}
	}
	else
	{
		src_reg_after = 0;
	}

	if (inst->dest_mode == REGISTER_DIRECT)
	{
		if (inst->dest_operand < 0 || inst->dest_operand > 7)
		{
			print_ext_error(ERROR_INVALID_DESTINATION_REGISTER, file_name, line_number);
			return 0;
		}
	}
	else
	{
		dest_reg_after = 0;
	}

	/* Normalize the source and destination addressing modes */
	switch (inst->src_mode)
	{
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
	switch (inst->dest_mode)
	{
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

unsigned int encode_operand(int operand, int type, char* file_name, int line_number)
{
	unsigned int word = 0; /* Initialize the word to zero */

	switch (type)
	{
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

int process_instruction(Instruction* inst, char* file_name, int line_number, DataList* data_list,
                        label_table* label_head, int address)
{
	unsigned int first_word = 0; /* Initialize the first word to zero */
	unsigned int operand_word = 0; /* Initialize the operand word to zero */
	label_table* label; /* Label pointer */
	int relative_distance; /* Relative distance between labels */
	int original_address = address; /* Original address for relative distance */

	if (!inst)
	{
		print_ext_error(ERROR_NULL_INSTRUCTION, file_name, line_number);
		return FAILURE;
	}

	if (validate_addressing_modes(inst, file_name, line_number))
	{
		return FAILURE;
	}
	/* Build the First Word */
	first_word = build_first_word(inst, file_name, line_number);

	if (add_data_node(data_list, address++, (int)first_word, file_name)) {
		return FAILURE;
	}
	/* If either source or destination is UNKNOWN, we skip */
	/* Process Source Operand */
	if (inst->src_mode == IMMEDIATE)
	{
		/* Encode Immediate Value */
		operand_word = encode_operand(inst->src_operand, IMMEDIATE, file_name, line_number);
		/* Add the encoded operand to the data list */
		if (add_data_node(data_list, address++, (int)operand_word, file_name)) {
			return FAILURE;
		}
	}
	else if (inst->src_mode == DIRECT)
	{
		/* We default as direct we will check if it is external */
		if (strlen(inst->src_label) > MAX_LABEL_SIZE)
		{
			print_ext_error(ERROR_LABEL_TOO_LONG, file_name, line_number);
			return FAILURE;
		}
		label = search_label_list(label_head, inst->src_label); /* Find label */

		if (!label)
		{
			print_ext_error(ERROR_UNDEFINED_SOURCE_LABEL, file_name, line_number);
			return FAILURE;
		}
		if (label->type == EXTERN)
		{
			operand_word = 1; /* ARE = 1, rest zero for EXTERNAL */
			add_address_to_label(label, address); /* Add the address to the label */
		}
		else
		{
			/* Encode the label address */
			operand_word = encode_operand(label->addr, DIRECT, file_name, line_number);
		}
		if (add_data_node(data_list, address++, (int)operand_word, file_name)) {
			return FAILURE;
		}
	}
	else if (inst->src_mode == RELATIVE)
	{
		label = search_label_list(label_head, inst->src_label); /* Find label */
		if (!label)
		{
			print_ext_error(ERROR_UNDEFINED_SOURCE_LABEL, file_name, line_number);
			return FAILURE;
		}
		/* Calculate relative distance */
		relative_distance = label->addr - original_address;
		/* Encode the relative distance */
		operand_word = encode_operand(relative_distance, RELATIVE, file_name, line_number);
		/* Add the encoded operand to the data list */
		if (add_data_node(data_list, address++, (int)operand_word, file_name)) {
			return FAILURE;
		}
	}

	/* Process Destination Operand */
	if (inst->dest_mode == IMMEDIATE)
	{
		/* Encode Immediate Value */
		operand_word = encode_operand(inst->dest_operand, IMMEDIATE, file_name, line_number);
		/* Add the encoded operand to the data list */
		if (add_data_node(data_list, address++, (int)operand_word, file_name)) {
			return FAILURE;
		}
	}
	else if (inst->dest_mode == DIRECT)
	{
		/* Check if the label is too long */
		if (strlen(inst->dest_label) > MAX_LABEL_SIZE)
		{
			print_ext_error(ERROR_LABEL_TOO_LONG, file_name, line_number);
			return FAILURE;
		}
		/* We default as direct we will check if it is external */
		label = search_label_list(label_head, inst->dest_label); /* Find label */

		if (!label)
		{
			print_ext_error(ERROR_UNDEFINED_DEST_LABEL, file_name, line_number);
			return FAILURE;
		}
		if (label->type == EXTERN)
		{
			operand_word = 1; /* ARE = 1, rest zero for EXTERNAL */
			add_address_to_label(label, address);
		}
		else
		{
			/* Encode the label address */
			operand_word = encode_operand(label->addr, DIRECT, file_name, line_number);
		}
		if (add_data_node(data_list, address++, (int)operand_word, file_name)) {
			return FAILURE;
		}
	}
	else if (inst->dest_mode == RELATIVE)
	{
		label = search_label_list(label_head, inst->dest_label); /* Find label */
		if (!label)
		{
			print_ext_error(ERROR_UNDEFINED_DEST_LABEL, file_name, line_number);
			return FAILURE;
		}
		/* Calculate relative distance */
		relative_distance = label->addr - original_address;
		/* Encode the relative distance */
		operand_word = encode_operand(relative_distance, RELATIVE, file_name, line_number);
		/* Add the encoded operand to the data list */
		if (add_data_node(data_list, address++, (int)operand_word, file_name)) {
			return FAILURE;
		}
	}

	return SUCCESS;
}


int parse_instruction(char* line, InstructionList* instruction_list, char* file_name, int line_number)
{
	char name[MAX_LINE_LENGTH]; /* Instruction Name */
	Instruction* inst = handle_malloc(sizeof(Instruction)); /* Allocate memory for the instruction */
	Instruction* found_inst; /* Found instruction */

	/* Extract the Instruction Name */
	line = getWord(line, name);

	/* Find and copy the instruction */
	found_inst = find_operation(name);
	if (!found_inst)
	{
		print_ext_error(ERROR_INSTRUCTION_NOT_FOUND, file_name, line_number);
		free(inst);
		return FAILURE;
	}

	/* Copy the instruction data */
	memcpy(inst, found_inst, sizeof(Instruction)); /* Copy the instruction data */
	inst->line_number = line_number; /* Set the line number */
	memset(inst->src_label, 0, MAX_LINE_LENGTH); /* Clear the source label */
	memset(inst->dest_label, 0, MAX_LINE_LENGTH); /* Clear the destination label */
	inst->src_mode = UNKNOWN; /* Set the source mode to UNKNOWN */
	inst->dest_mode = UNKNOWN; /* Set the destination mode to UNKNOWN */

	/* Check for illegal comma at start */
	if (*line == ',')
	{
		print_ext_error(ERROR_ILLEGAL_COMMA, file_name, line_number);
		free(inst);
		return FAILURE;
	}

	/* Skip spaces */
	line = skipSpaces(line);

	switch (inst->opcode)
	{
	/* Instructions with No Operands */
	case OPCODE_RTS:
	case OPCODE_STOP:
		/* Check for extraneous text */
		EXTRANEOUS_TEXT(*line, file_name, line_number);
		break;

	/* Instructions with One Operand */
	case OPCODE_CLR:
	case OPCODE_JMP:
	case OPCODE_RED:
	case OPCODE_PRN:
		/* Parse the operand */
		if (parse_operand(&line, &inst->dest_mode, &inst->dest_operand, inst->dest_label, file_name, line_number) ==
			FAILURE)
		{
			free(inst);
			return FAILURE;
		}
		/* Check for extraneous text */
		line = skipSpaces(line);
		EXTRANEOUS_TEXT(*line, file_name, line_number);
		break;

	/* Instructions with Two Operands */
	default:
		/* Parse the source operand */
		if (parse_operand(&line, &inst->src_mode, &inst->src_operand, inst->src_label, file_name, line_number) ==
			FAILURE)
		{
			/* Free the instruction */
			free(inst);
			return FAILURE;
		}
		/* Skip spaces */
		line = skipSpaces(line);

		if (*line != ',')
		{
			/* Check for missing comma */
			print_ext_error(ERROR_MISSING_COMMA, file_name, line_number);
			free(inst);
			return FAILURE;
		}
		/* Skip the comma */
		line = skipSpaces(line + 1);

		/* Parse the destination operand */
		if (parse_operand(&line, &inst->dest_mode, &inst->dest_operand, inst->dest_label, file_name, line_number) ==
			FAILURE)
		{
			/* Free the instruction */
			free(inst);
			return FAILURE;
		}
		/* Skip spaces */
		line = skipSpaces(line);
		/* Check for extraneous text */
		EXTRANEOUS_TEXT(*line, file_name, line_number);
		break;
	}

	/* Add the instruction to the instruction list */
	add_instruction(instruction_list, inst);
	free(inst);
	return SUCCESS;	/* Return success */
}

int parse_operand(char** line, int* mode, int* operand, char* label, char* file_name, int line_number)
{
	int number_length = 0; /* Length of the number */
	char temp_label[MAX_LINE_LENGTH]; /* Temporary label */
	int reg_num; /* Register number */

	/* Check for NULL pointers */
	if (!(*line)[0])
	{
		/* Missing operand */
		print_ext_error(ERROR_MISSING_OPERAND, file_name, line_number);
		return FAILURE;
	}

	if ((*line)[0] == ',')
	{
		/* Illegal comma */
		print_ext_error(ERROR_ILLEGAL_COMMA, file_name, line_number);
		return FAILURE;
	}
	if ((*line)[0] == '#')
	{
		/* Immediate Value */
		*mode = IMMEDIATE;
		number_length = getNum(*line + 1, operand, file_name, line_number);
		if (number_length == 0) return FAILURE;
		*line += (1 + number_length);
	}
	else if ((*line)[0] == '&')
	{
		/* Relative Address */
		*mode = RELATIVE;
		/* Skip the '&' */
		(*line)++;
		/* Parse the label */
		*line = getWord(*line, temp_label);
		/* Copy the label */
		strncpy(label, temp_label, MAX_LINE_LENGTH - 1);
		/* Null-terminate the label */
		label[MAX_LINE_LENGTH - 1] = '\0';
	}
	else if ((*line)[0] == 'r' && isdigit((*line)[1]))
	{
		/* Register Direct or Direct */
		reg_num = atoi(*line + 1);
		if (reg_num > 7)
		{
			/* Not a Register but a Direct */
			*mode = DIRECT;
			/* Parse the label */
			*line = getWord(*line, temp_label);
			/* Copy the label */
			strncpy(label, temp_label, MAX_LINE_LENGTH - 1);
			/* Null-terminate the label */
			label[MAX_LINE_LENGTH - 1] = '\0';
		}
		else
		{
			/* Register Direct */
			*mode = REGISTER_DIRECT;
			/* Set the operand */
			*operand = reg_num;
			/* Skip the r and  number */
			*line += 2;
		}
	}
	else if (isalpha((*line)[0])) /* Label */
	{
		/* Direct Address */
		*mode = DIRECT;
		/* Parse the label */
		*line = getWord(*line, temp_label);
		/* Copy the label */
		strncpy(label, temp_label, MAX_LINE_LENGTH - 1);
		/* Null-terminate the label */
		label[MAX_LINE_LENGTH - 1] = '\0';
	}
	else
	{
		/* Invalid operand */
		print_ext_error(ERROR_INVALID_OPERAND, file_name, line_number);
		return FAILURE;
	}

	return SUCCESS;
}

int validate_addressing_modes(Instruction* inst, char* file_name, int line_number)
{
	if (!inst)
	{
		print_ext_error(ERROR_NULL_INSTRUCTION, file_name, line_number);
		return FAILURE;
	}

	/* Validate opcode-based addressing mode constraints */
	switch (inst->opcode)
	{
	case 1: /* cmp */
		if (!(inst->src_mode == IMMEDIATE || inst->src_mode == DIRECT || inst->src_mode == REGISTER_DIRECT) ||
			!(inst->dest_mode == IMMEDIATE || inst->dest_mode == DIRECT || inst->dest_mode == REGISTER_DIRECT))
		{
			print_ext_error(ERROR_INVALID_OPERAND_ADDRESSING_MODE, file_name, line_number);
			return FAILURE;
		}
		break;
	case 0: /* mov */
	case 2: /* add, sub */
		if (!(inst->src_mode == IMMEDIATE || inst->src_mode == DIRECT || inst->src_mode == REGISTER_DIRECT) ||
			!(inst->dest_mode == DIRECT || inst->dest_mode == REGISTER_DIRECT))
		{
			print_ext_error(ERROR_INVALID_OPERAND_ADDRESSING_MODE, file_name, line_number);
			return FAILURE;
		}
		break;
	case 4: /* lea */
		if (!(inst->src_mode == DIRECT) || !(inst->dest_mode == DIRECT || inst->dest_mode == REGISTER_DIRECT))
		{
			print_ext_error(ERROR_INVALID_OPERAND_ADDRESSING_MODE, file_name, line_number);
			return FAILURE;
		}
		break;
	case 5: /* clr, not, inc, dec */
	case 12: /* red */
		if (!(inst->dest_mode == DIRECT || inst->dest_mode == REGISTER_DIRECT))
		{
			print_ext_error(ERROR_INVALID_OPERAND_ADDRESSING_MODE, file_name, line_number);
			return FAILURE;
		}
		break;
	case 9: /* jmp, bne, jsr */
		if (!(inst->dest_mode == DIRECT || inst->dest_mode == RELATIVE))
		{
			print_ext_error(ERROR_INVALID_OPERAND_ADDRESSING_MODE, file_name, line_number);
			return FAILURE;
		}
		break;
	case 13: /* prn */
		if (!(inst->dest_mode == IMMEDIATE || inst->dest_mode == DIRECT || inst->dest_mode == REGISTER_DIRECT))
		{
			print_ext_error(ERROR_INVALID_OPERAND_ADDRESSING_MODE, file_name, line_number);
			return FAILURE;
		}
		break;
	case 14: /* rts */
	case 15: /* stop */
		if (inst->src_mode != UNKNOWN || inst->dest_mode != UNKNOWN)
		{
			print_ext_error(ERROR_EXTRA_OPERANDS, file_name, line_number);
			return FAILURE;
		}
		break;
	default:
		print_ext_error(ERROR_INVALID_OPCODE, file_name, line_number);
		return FAILURE;
	}

	return SUCCESS; /* Valid addressing modes */
}

char* getWord(char* line, char* word)
{
	int i = 0; /* Index for the word */

	/* Check for NULL pointers */
	if (!line || !word)
	{
		return NULL;
	}

	line = skipSpaces(line); /* Skip leading spaces */

	/* Extract word until space, comma, or end of string */
	while (*line != ' ' && *line != ',' && *line != '\0' && *line != '\n' && i < MAX_LINE_LENGTH - 1)
	{
		word[i++] = *line++;
	}
	word[i] = '\0'; /* Null-terminate the word */

	return line; /* Return pointer to the next part of the string */
}

int getNum(char* line, int* num, char* file_name, int line_number)
{
	char buffer[MAX_LINE_LENGTH] = {0}; /* Buffer to store the number */
	int i = 0; /* Index for the buffer */
	int length = 0; /* Length of the number */
	char* endptr; /* Pointer to the end of the number */
	int tempNum; /* Temporary number */

	if (!line || !num)
	{
		print_ext_error(ERROR_INVALID_NUMBER, file_name, line_number);
		return 0;
	}

	line = skipSpaces(line);

	/* Check if the first character is '-' or '+' for negative/positive numbers */
	if (*line == '-' || *line == '+')
	{
		buffer[i++] = *line++;
		length++;
	}

	/* Parse digits until ',' or end of string */
	while (*line != ',' && *line != '\0' && !isspace(*line))
	{
		if (!isdigit(*line))
		{
			/* Invalid character in number */
			print_ext_error(ERROR_INVALID_NUMBER, file_name, line_number);
			return 0;
		}
		buffer[i++] = *line++;
		length++;
	}

	buffer[i] = '\0'; /* Null-terminate the buffer */

	/* Check if buffer contains only a valid number */
	if (i == 0 || (i == 1 && (buffer[0] == '-' || buffer[0] == '+')))
	{
		/* No valid digits after sign or empty buffer */
		print_ext_error(ERROR_INVALID_NUMBER, file_name, line_number);
		return 0;
	}

	if (strlen(buffer) > 8) /* Check if the number is too long */
	{
		print_ext_error(ERROR_NUMBER_OVERFLOW, file_name, line_number);
		return 0;
	}

	/* Convert the extracted string to an integer */
	tempNum = strtol(buffer, &endptr, 10);

	/* Check for invalid range */
	if (tempNum > MAX_IMMEDIATE_VALUE || tempNum < MIN_IMMEDIATE_VALUE)
	{
		print_ext_error(ERROR_NUMBER_OVERFLOW, file_name, line_number);
		return 0;
	}

	*num = tempNum; /* Store the parsed number */

	/* Ensure full parsing without leftover characters */
	if (*endptr != '\0')
	{
		print_ext_error(ERROR_INVALID_NUMBER, file_name, line_number);
		return 0;
	}

	return length; /* success */
}

char* skipSpaces(char* line)
{
	/* Skip leading spaces */
	while (isspace(*line))
	{
		line++;
	}
	return line;
}

void removeSpaces(char* p)
{
	char* write = p; /* Pointer to write to */
	char* read = p; /* Pointer to read from */

	while (*read != '\0')
	{
		/* Copy non-space characters */
		if (!isspace(*read))
		{
			*write = *read; /* Copy the character */
			write++; /* Move the write pointer */
		}
		read++; /* Move the read pointer */
	}
	*write = '\0'; /* Null-terminate the modified string */
}
