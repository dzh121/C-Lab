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
	int i; /* looping through the operations list */

	/* go over all operations until I hit NULL */
	for (i = 0; operations[i].name != NULL; i++)
	{
		if (strcmp(operations[i].name, name) == 0)
		{
			return &operations[i]; /* found it, return the pointer */
		}
	}
	return NULL; /* didnt find the operation, return NULL */
}

int calculate_words(Instruction* inst, char* file_name, int line_number)
{
	int words = 1; /* always at least 1 word for the instruction itself */

	/* check if the source operand needs an extra word */
	switch (inst->src_mode)
	{
	case IMMEDIATE:
	case DIRECT:
	case RELATIVE:
		words++; /* these need extra word */
		break;
	case REGISTER_DIRECT:
	case UNKNOWN:
		break; /* no extra word needed */
	default:
		print_ext_error(ERROR_INVALID_SOURCE_ADDRESSING_MODE, file_name, line_number);
		break;
	}

	/* same thing for the destination operand */
	switch (inst->dest_mode)
	{
	case IMMEDIATE:
	case DIRECT:
	case RELATIVE:
		words++; /* also need extra */
		break;
	case REGISTER_DIRECT:
	case UNKNOWN:
		break; /* nope, all good */
	default:
		print_ext_error(ERROR_INVALID_DESTINATION_ADDRESSING_MODE, file_name, line_number);
		break;
	}

	return words; /* return how many words total this instruction will use */
}

unsigned int build_first_word(Instruction* inst, char* file_name, int line_number)
{
	unsigned int word = 0; /* final result, starts at 0 */
	int src_mode_after; /* source addr mode after cleaning it up */
	int dest_mode_after; /* same for dest */
	int src_reg_after; /* source reg, cleaned */
	int dest_reg_after; /* dest reg */

	/* make sure I got a valid instruction to work with */
	if (!inst)
	{
		print_ext_error(ERROR_NULL_INSTRUCTION, file_name, line_number);
		return 0;
	}

	src_reg_after = inst->src_operand; /* take original value */
	dest_reg_after = inst->dest_operand; /* take original value */

	/* opcode must be valid (in range) */
	if (inst->opcode < 0 || inst->opcode > MAX_OPCODE)
	{
		print_ext_error(ERROR_INVALID_OPCODE, file_name, line_number);
		return 0;
	}

	/* funct too, make sure it's not out of range */
	if (inst->funct < 0 || inst->funct > MAX_FUNCT)
	{
		print_ext_error(ERROR_INVALID_FUNCT, file_name, line_number);
		return 0;
	}

	/* check source addr mode range */
	if (inst->src_mode < 0 || inst->src_mode > 4)
	{
		print_ext_error(ERROR_INVALID_SOURCE_ADDRESSING_MODE, file_name, line_number);
		return 0;
	}

	/* and same for destination mode */
	if (inst->dest_mode < 0 || inst->dest_mode > 4)
	{
		print_ext_error(ERROR_INVALID_DESTINATION_ADDRESSING_MODE, file_name, line_number);
		return 0;
	}

	/* ARE has to be 1, 2 or 4 only */
	if (inst->are != 1 && inst->are != 2 && inst->are != 4)
	{
		print_ext_error(ERROR_INVALID_ARE_BITS, file_name, line_number);
		return 0;
	}

	/* if using register, validate its range */
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
		src_reg_after = 0; /* not a reg, set to 0 */
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

	/* now map the addr modes to bits */
	switch (inst->src_mode)
	{
	case UNKNOWN: /* just treat as 0 */
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
	default:
		print_ext_error(ERROR_INVALID_SOURCE_ADDRESSING_MODE, file_name, line_number);
		return 0;
	}

	switch (inst->dest_mode)
	{
	case UNKNOWN:
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
	default:
		print_ext_error(ERROR_INVALID_DESTINATION_ADDRESSING_MODE, file_name, line_number);
		return 0;
	}

	/* now finally build the full 24-bit word */
	word |= (inst->opcode & 0x3F) << 18; /* 6 bits for opcode */
	word |= (src_mode_after & 0x3) << 16; /* 2 bits src addr mode */
	word |= (src_reg_after & 0x7) << 13; /* 3 bits src reg */
	word |= (dest_mode_after & 0x3) << 11; /* 2 bits dest addr mode */
	word |= (dest_reg_after & 0x7) << 8; /* 3 bits dest reg */
	word |= (inst->funct & 0x1F) << 3; /* 5 bits funct */
	word |= (inst->are & 0x7); /* last 3 bits are ARE */

	return word; /* send it back */
}

unsigned int encode_operand(int operand, int type, char* file_name, int line_number)
{
	unsigned int word = 0; /* starting with 0 */

	switch (type)
	{
	case IMMEDIATE:
		/* immediate value like #5 */
		word |= 4; /* ARE is 4 for absolute */
		word |= ((operand & 0x1FFFFF) << 3); /* shove the number into the upper bits */
		break;
	case DIRECT:
		/* like a label that already got an address */
		word |= 2; /* ARE = 2 for relocatable */
		word |= ((operand & 0x1FFFFF) << 3); /* store the address */
		break;
	case RELATIVE:
		/* label but calculated from here, like &MAIN */
		word |= 4; /* still absolute */
		word |= ((operand & 0x1FFFFF) << 3); /* store the distance */
		break;
	case EXTERNAL:
		/* external label from another file */
		word |= 1; /* ARE = 1 for external */
		word |= (0 << 3); /* rest is zero for externals */
		break;
	default:
		print_ext_error(ERROR_INVALID_OPERAND_TYPE, file_name, line_number);
		return 0;
	}
	return word; /* send it back */
}

int process_instruction(Instruction* inst, char* file_name, int line_number, DataList* data_list,
                        label_table* label_head, int address)
{
	unsigned int first_word = 0; /* main instruction word */
	unsigned int operand_word = 0; /* will be used for each operand if needed */
	label_table* label; /* to hold pointer to label we find */
	int relative_distance; /* used when doing &label things */
	const int original_address = address; /* save for relative calcs */
	int i; /* loop counter */
	int mode; /* current addressing mode */
	int operand; /* current operand value */
	char* label_name; /* current label name */

	/* make sure I got a valid instruction to work with */
	if (!inst)
	{
		print_ext_error(ERROR_NULL_INSTRUCTION, file_name, line_number);
		return FAILURE;
	}

	/* validate the addressing modes */
	if (validate_addressing_modes(inst, file_name, line_number))
	{
		return FAILURE;
	}

	/* build the first word of the instruction */
	first_word = build_first_word(inst, file_name, line_number);

	/* add it to the data list */
	if (add_data_node(data_list, address++, (int)first_word, file_name))
	{
		return FAILURE;
	}

	/* go over both src and dest operands */
	for (i = 0; i < 2; i++)
	{
		/* pick the right mode and operand */
		mode = (i == 0) ? inst->src_mode : inst->dest_mode;
		operand = (i == 0) ? inst->src_operand : inst->dest_operand;
		label_name = (i == 0) ? inst->src_label : inst->dest_label;

		if (mode == UNKNOWN || mode == REGISTER_DIRECT)
			continue; /* skip if nothing to encode or register handled in first word */


		if (mode == IMMEDIATE)
		{
			/* immediate value like #5 build word*/
			operand_word = encode_operand(operand, IMMEDIATE, file_name, line_number);
		}
		else if (mode == DIRECT)
		{
			/* like a label that already got an address */
			if (strlen(label_name) > MAX_LABEL_SIZE)
			{
				/* label too long */
				print_ext_error(ERROR_LABEL_TOO_LONG, file_name, line_number);
				return FAILURE;
			}
			/* find the label in the list */
			label = search_label_list(label_head, label_name);
			/* check if label exists */
			if (!label)
			{
				/* label not found */
				print_ext_error(i == 0 ? ERROR_UNDEFINED_SOURCE_LABEL : ERROR_UNDEFINED_DEST_LABEL, file_name, line_number);
				return FAILURE;
			}

			/* label found, encode it */
			if (label->type == EXTERN)
			{
				/* external label */
				operand_word = encode_operand(0, EXTERNAL, file_name, line_number);
				add_address_to_label(label, address);
			}
			else
			{
				/* regular label */
				operand_word = encode_operand(label->addr, DIRECT, file_name, line_number);
			}
		}
		else if (mode == RELATIVE)
		{
			/* label but calculated from here, like &MAIN */
			/* find the label in the list */
			label = search_label_list(label_head, label_name);

			/* check if label exists */
			if (!label)
			{
				print_ext_error(i == 0 ? ERROR_UNDEFINED_SOURCE_LABEL : ERROR_UNDEFINED_DEST_LABEL, file_name, line_number);
				return FAILURE;
			}

			/* label found, calculate the distance */
			relative_distance = label->addr - original_address;
			/* encode it */
			operand_word = encode_operand(relative_distance, RELATIVE, file_name, line_number);
		}

		/* add the operand to the data list */
		if (add_data_node(data_list, address++, (int)operand_word, file_name))
		{
			return FAILURE;
		}
	}

	return SUCCESS; /* all good */
}

int parse_instruction(char* line, InstructionList* instruction_list, char* file_name, int line_number)
{
	char name[MAX_LINE_LENGTH]; /* gonna store the instruction name here */
	Instruction* inst = handle_malloc(sizeof(Instruction)); /* need space for a new instruction */
	Instruction* found_inst; /* to hold the matching operation from list */

	if (!inst)
	{
		return FAILURE; /* couldn’t allocate, just fail */
	}

	memset(inst, 0, sizeof(Instruction)); /* zero out everything just in case */

	line = getWord(line, name); /* grab the instruction name from the line */

	found_inst = find_operation(name); /* try to find it in our list */
	if (!found_inst)
	{
		print_ext_error(ERROR_INSTRUCTION_NOT_FOUND, file_name, line_number);
		free(inst);
		return FAILURE;
	}

	memcpy(inst, found_inst, sizeof(Instruction)); /* copy the operation data into our instance */
	inst->line_number = line_number; /* save the line it came from */
	memset(inst->src_label, 0, MAX_LINE_LENGTH); /* clear any junk */
	memset(inst->dest_label, 0, MAX_LINE_LENGTH);
	inst->src_mode = UNKNOWN; /* don’t know the addressing mode yet */
	inst->dest_mode = UNKNOWN;

	if (*line == ',')
	{
		/* line starts with a comma? not allowed */
		print_ext_error(ERROR_ILLEGAL_COMMA, file_name, line_number);
		free(inst);
		return FAILURE;
	}

	line = skipSpaces(line); /* skip to the real stuff */

	switch (inst->opcode)
	{
	case OPCODE_RTS:
	case OPCODE_STOP:
		/* these have no operands at all */
		EXTRANEOUS_TEXT(*line, file_name, line_number);
		break;

	case OPCODE_CLR:
	case OPCODE_JMP:
	case OPCODE_RED:
	case OPCODE_PRN:
		/* just one operand for these */
		if (parse_operand(&line, &inst->dest_mode, &inst->dest_operand, inst->dest_label, file_name, line_number) == FAILURE)
		{
			free(inst);
			return FAILURE;
		}
		line = skipSpaces(line); /* check if anything left after */
		EXTRANEOUS_TEXT(*line, file_name, line_number);
		break;

	default:
		/* two operands version */
		if (parse_operand(&line, &inst->src_mode, &inst->src_operand, inst->src_label, file_name, line_number) == FAILURE)
		{
			free(inst);
			return FAILURE;
		}

		line = skipSpaces(line);

		if (*line != ',')
		{
			print_ext_error(ERROR_MISSING_COMMA, file_name, line_number);
			free(inst);
			return FAILURE;
		}

		line = skipSpaces(line + 1); /* skip comma */

		if (parse_operand(&line, &inst->dest_mode, &inst->dest_operand, inst->dest_label, file_name, line_number) == FAILURE)
		{
			free(inst);
			return FAILURE;
		}

		line = skipSpaces(line);
		EXTRANEOUS_TEXT(*line, file_name, line_number);
		break;
	}

	add_instruction(instruction_list, inst); /* push it to list */
	free(inst); /* no need to keep local copy */
	return SUCCESS;
}

int parse_operand(char** line, int* mode, int* operand, char* label, char* file_name, int line_number)
{
	int number_length = 0; /* for counting digits */
	char temp_label[MAX_LINE_LENGTH]; /* just a buffer to hold parsed label */
	int reg_num; /* register number if its a register */

	/* check for NULL pointers */
	if (!(*line)[0])
	{
		print_ext_error(ERROR_MISSING_OPERAND, file_name, line_number);
		return FAILURE;
	}

	if ((*line)[0] == ',')
	{
		/* got a comma, not allowed here */
		print_ext_error(ERROR_ILLEGAL_COMMA, file_name, line_number);
		return FAILURE;
	}

	if ((*line)[0] == '#')
	{
		/* got an immediate value */
		*mode = IMMEDIATE;
		number_length = getNum(*line + 1, operand, file_name, line_number);

		if (number_length == 0) return FAILURE; /* error with the number */

		*line += (1 + number_length); /* skip the whole thing */
	}
	else if ((*line)[0] == '&')
	{
		/* relative label like &SOMETHING */
		*mode = RELATIVE;
		(*line)++;
		*line = getWord(*line, temp_label); /* get the label */
		strncpy(label, temp_label, MAX_LINE_LENGTH - 1); /* copy it */
		label[MAX_LINE_LENGTH - 1] = '\0'; /* make sure it's null-terminated */
	}
	else if ((*line)[0] == 'r' && isdigit((*line)[1]))
	{
		reg_num = atoi(*line + 1); /* get the number after 'r' */

		if (reg_num > 7)
		{
			/* not a real register, probably a label like r9XYZ */
			*mode = DIRECT;
			*line = getWord(*line, temp_label); /* get the label */
			strncpy(label, temp_label, MAX_LINE_LENGTH - 1); /* copy it */
			label[MAX_LINE_LENGTH - 1] = '\0'; /* make sure it's null-terminated */
		}
		else
		{
			/* it's a real register */
			*mode = REGISTER_DIRECT;
			*operand = reg_num;
			*line += 2;
		}
	}
	else if (isalpha((*line)[0]))
	{
		/* got a label like MAIN */
		*mode = DIRECT;
		*line = getWord(*line, temp_label);
		strncpy(label, temp_label, MAX_LINE_LENGTH - 1);
		label[MAX_LINE_LENGTH - 1] = '\0';
	}
	else
	{
		print_ext_error(ERROR_INVALID_OPERAND, file_name, line_number);
		return FAILURE;
	}

	return SUCCESS;
}

int validate_addressing_modes(Instruction* inst, char* file_name, int line_number)
{
	if (!inst)
	{
		/* no instruction? can’t check anything */
		print_ext_error(ERROR_NULL_INSTRUCTION, file_name, line_number);
		return FAILURE;
	}

	/* check if this opcode actually allows the modes we got */
	switch (inst->opcode)
	{
	case 1: /* cmp */
		/* cmp allows anything */
		if (!(inst->src_mode == IMMEDIATE || inst->src_mode == DIRECT || inst->src_mode == REGISTER_DIRECT) ||
			!(inst->dest_mode == IMMEDIATE || inst->dest_mode == DIRECT || inst->dest_mode == REGISTER_DIRECT))
		{
			print_ext_error(ERROR_INVALID_OPERAND_ADDRESSING_MODE, file_name, line_number);
			return FAILURE;
		}
		break;

	case 0: /* mov */
	case 2: /* add, sub */
		/* cant have immediate as destination */
		if (!(inst->src_mode == IMMEDIATE || inst->src_mode == DIRECT || inst->src_mode == REGISTER_DIRECT) ||
			!(inst->dest_mode == DIRECT || inst->dest_mode == REGISTER_DIRECT))
		{
			print_ext_error(ERROR_INVALID_OPERAND_ADDRESSING_MODE, file_name, line_number);
			return FAILURE;
		}
		break;

	case 4: /* lea */
		/* lea only works with direct as source */
		if (!(inst->src_mode == DIRECT) || !(inst->dest_mode == DIRECT || inst->dest_mode == REGISTER_DIRECT))
		{
			print_ext_error(ERROR_INVALID_OPERAND_ADDRESSING_MODE, file_name, line_number);
			return FAILURE;
		}
		break;

	case 5: /* clr, not, inc, dec */
	case 12: /* red */
		/* dest cannot be immediate */
		if (!(inst->dest_mode == DIRECT || inst->dest_mode == REGISTER_DIRECT))
		{
			print_ext_error(ERROR_INVALID_OPERAND_ADDRESSING_MODE, file_name, line_number);
			return FAILURE;
		}
		break;

	case 9: /* jmp, bne, jsr */
		/* these jump to a label, can be relative */
		if (!(inst->dest_mode == DIRECT || inst->dest_mode == RELATIVE))
		{
			print_ext_error(ERROR_INVALID_OPERAND_ADDRESSING_MODE, file_name, line_number);
			return FAILURE;
		}
		break;

	case 13: /* prn */
		/* prn allows all */
		if (!(inst->dest_mode == IMMEDIATE || inst->dest_mode == DIRECT || inst->dest_mode == REGISTER_DIRECT))
		{
			print_ext_error(ERROR_INVALID_OPERAND_ADDRESSING_MODE, file_name, line_number);
			return FAILURE;
		}
		break;

	case 14: /* rts */
	case 15: /* stop */
		/* no operands allowed */
		if (inst->src_mode != UNKNOWN || inst->dest_mode != UNKNOWN)
		{
			print_ext_error(ERROR_EXTRA_OPERANDS, file_name, line_number);
			return FAILURE;
		}
		break;

	default:
		/* if opcode unknown, bail */
		print_ext_error(ERROR_INVALID_OPCODE, file_name, line_number);
		return FAILURE;
	}

	return SUCCESS; /* looks good */
}

char* getWord(char* line, char* word)
{
	int i = 0; /* index into the word buffer */

	/* check for NULL pointers */
	if (!line || !word)
	{
		return NULL;
	}

	line = skipSpaces(line); /* skip any leading spaces */

	/* grab characters till we hit space, comma, newline etc */
	while (*line != ' ' && *line != ',' && *line != '\0' && *line != '\n' && i < MAX_LINE_LENGTH - 1)
	{
		word[i++] = *line++;
	}
	word[i] = '\0'; /* end it properly */

	return line; /* return */
}

int getNum(char* line, int* num, char* file_name, int line_number)
{
	char buffer[MAX_LINE_LENGTH] = {0}; /* hold the number text */
	int i = 0; /* for indexing */
	int length = 0; /* how long is the number */
	char* endptr; /* strtol will use this */
	int tempNum; /* final result before storing */

	if (!line || !num)
	{
		print_ext_error(ERROR_INVALID_NUMBER, file_name, line_number);
		return 0;
	}

	line = skipSpaces(line); /* ignore leading spaces */

	/* could start with sign */
	if (*line == '-' || *line == '+')
	{
		buffer[i++] = *line++;
		length++;
	}

	/* now parse digits */
	while (*line != ',' && *line != '\0' && !isspace(*line))
	{
		if (!isdigit(*line))
		{
			print_ext_error(ERROR_INVALID_NUMBER, file_name, line_number);
			return 0;
		}
		buffer[i++] = *line++;
		length++;
	}

	buffer[i] = '\0'; /* close the string */

	/* make sure we actually got a number */
	if (i == 0 || (i == 1 && (buffer[0] == '-' || buffer[0] == '+')))
	{
		print_ext_error(ERROR_INVALID_NUMBER, file_name, line_number);
		return 0;
	}

	/* check if the number is too long (8 digits is more than allowed range) */
	if (strlen(buffer) > 8)
	{
		print_ext_error(ERROR_NUMBER_OVERFLOW, file_name, line_number);
		return 0;
	}

	tempNum = strtol(buffer, &endptr, 10); /* turn it into an int */

	/* out of range? */
	if (tempNum > MAX_IMMEDIATE_VALUE || tempNum < MIN_IMMEDIATE_VALUE)
	{
		print_ext_error(ERROR_NUMBER_OVERFLOW, file_name, line_number);
		return 0;
	}

	*num = tempNum; /* store the result */

	/* if strtol didn't parse all of it fail */
	if (*endptr != '\0')
	{
		print_ext_error(ERROR_INVALID_NUMBER, file_name, line_number);
		return 0;
	}

	return length; /* return how many chars we moved past */
}

char* skipSpaces(char* line)
{
	/* walk past all the spaces */
	while (isspace(*line))
	{
		line++;
	}
	return line;
}

void removeSpaces(char* p)
{
	char* write = p; /* write head */
	char* read = p; /* read head */

	while (*read != '\0')
	{
		/* only copy non-spaces */
		if (!isspace(*read))
		{
			*write = *read;
			write++;
		}
		read++;
	}
	*write = '\0'; /* end the new string */
}
