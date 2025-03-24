#include "../headers/first_pass.h"

int encode_string(char* line, int* DC, DataList* data_list, char* file_name, int line_count)
{
	char *start, *end; /* used to find the quotes */
	char* after_string; /* to check if someone wrote junk after the string */

	if (!line)
	{
		print_ext_error(ERROR_STRING_MISSING_QUOTES, file_name, line_count);
		return FAILURE;
	}

	start = strchr(line, '"'); /* look for the first " */
	if (!start)
	{
		print_ext_error(ERROR_STRING_MISSING_QUOTES, file_name, line_count);
		return FAILURE;
	}

	start++; /* skip past the opening quote */
	end = strchr(start, '"'); /* look for the second " */

	if (!end)
	{
		print_ext_error(ERROR_STRING_MISSING_QUOTES, file_name, line_count);
		return FAILURE;
	}

	/* loop through each char and store it */
	while (start < end)
	{
		if (add_data_node(data_list, *DC, (int)(*start), file_name))
		{
			return FAILURE;
		}
		(*DC)++;
		start++;
	}

	/* make sure no extra text after closing quote */
	after_string = end + 1;
	if (after_string && after_string[0] != '\0' && after_string[0] != '\n')
	{
		print_ext_error(ERROR_EXTRA_TEXT, file_name, line_count);
		return FAILURE;
	}

	/* add null terminator for string */
	if (add_data_node(data_list, *DC, 0, file_name))
	{
		return FAILURE;
	}
	(*DC)++;

	return SUCCESS;
}

int handle_extern(char* line, label_table** label_head, char* file_name, int line_count)
{
	char* token; /* the extern label */
	char* after_label; /* check for extra junk */

	if (!line)
	{
		print_ext_error(ERROR_MISSING_EXTERNAL_NAME, file_name, line_count);
		return FAILURE;
	}

	token = strtok(line + EXTERN_DIRECTIVE_OFFSET, " "); /* grab label after .extern */
	if (!token)
	{
		print_ext_error(ERROR_MISSING_EXTERNAL_NAME, file_name, line_count);
		return FAILURE;
	}

	token[strcspn(token, "\n")] = '\0'; /* remove newline if there is one */

	if (!isValidLabel(token, file_name, line_count))
	{
		return FAILURE;
	}

	after_label = strtok(NULL, " "); /* see if there’s extra text */
	if (after_label)
	{
		print_ext_error(ERROR_EXTRA_TEXT, file_name, line_count);
		return FAILURE;
	}

	/* add the label to the list */
	if (add_label_list(label_head, token, 0, line_count, EXTERN, file_name))
	{
		return FAILURE;
	}

	return SUCCESS;
}

int encode_data(char* line, int* DC, DataList* data_list, char* file_name, int line_count)
{
	int value; /* the number */
	char* ptr; /* for checking the number */
	char* token = strtok(line + DATA_DIRECTIVE_OFFSET, ","); /* grab the first number */

	while (token)
	{
		token = trim_whitespace(token); /* clean it */

		if (token[0] == '\0')
		{
			/* empty token */
			print_ext_error(ERROR_INVALID_DATA, file_name, line_count);
			return FAILURE;
		}

		for (ptr = token; *ptr != '\0'; ptr++)
		{
			if (!isdigit(*ptr) && *ptr != '-' && *ptr != '+')
			{
				/* not a number */
				print_ext_error(ERROR_INVALID_NUMBER, file_name, line_count);
				return FAILURE;
			}
		}

		/* check if it’s a valid number */
		if (strlen(token) > 8)
		{
			print_ext_error(ERROR_NUMBER_OVERFLOW, file_name, line_count);
			return FAILURE;
		}

		/* convert to int */
		if (sscanf(token, "%d", &value) != 1)
		{
			print_ext_error(ERROR_INVALID_NUMBER, file_name, line_count);
			return FAILURE;
		}

		/* check if it’s in range */
		if (value > MAX_DATA_VALUE || value < MIN_DATA_VALUE)
		{
			print_ext_error(ERROR_NUMBER_OVERFLOW, file_name, line_count);
			return FAILURE;
		}

		/* add the number to the data list */
		if (add_data_node(data_list, *DC, value, file_name))
		{
			return FAILURE;
		}
		/* increment the DC */
		(*DC)++;

		/* grab the next number */
		token = strtok(NULL, ",");
	}

	return SUCCESS;
}

int first_pass(char* file_name, DataList* data_list, InstructionList* instruction_list, label_table** label_head,
               int* ICF, int* DCF)
{
	/* set up some counters and stuff we going to need */
	int IC = INITIAL_IC, DC = INITIAL_DC, line_count = 0, did_fail = FALSE, label_length;
	FILE* fp; /* file pointer for reading the .am file */
	char line[MAX_LINE_LENGTH], *after_label, label[MAX_LABEL_SIZE]; /* line buffer and label holder */
	label_table* label_dummy = NULL; /* used to update label addresses later */
	DataNode* current_data = NULL; /* loop over data nodes to update addresses */
	Instruction* inst = NULL; /* current instruction */
	/* whitespace is loose in this assembler - extra spaces don't matter */

	/* try opening the file */
	if ((fp = fopen(file_name, "r")) == NULL)
	{
		print_internal_error(ERROR_FILE_OPEN_SOURCE);
		return FAILURE;
	}

	/* go line by line */
	while (fgets(line, sizeof(line), fp) != NULL)
	{
		line_count++;

		memset(label, '\0', sizeof(label)); /* clear label each line */

		after_label = strchr(line, ':'); /* is there a label? */
		if (after_label != NULL)
		{
			label_length = after_label - line; /* get label length (not including :) */
			strncpy(label, line, label_length);
			label[label_length] = '\0';

			/* check if the label is valid */
			if (!isValidLabel(label, file_name, line_count))
			{
				did_fail = TRUE;
				memset(label, '\0', sizeof(label));
				continue;
			}

			/* check if there’s extra text after the label */
			if (*after_label != ':')
			{
				did_fail = TRUE;
				print_ext_error(ERROR_MISSING_COLON, file_name, line_count);
				continue;
			}

			/* skip ':' */
			after_label++; /* skip ':' */

			/* check if there’s a space after the label */
			if (!isspace(*after_label))
			{
				did_fail = TRUE;
				print_ext_error(ERROR_MISSING_WHITE_SPACE, file_name, line_count);
				continue;
			}

			after_label++; /* skip space after label */

			/* check if there’s a directive or instruction after the label */
			if (!after_label || !*after_label || *after_label == '\n')
			{
				print_ext_error(ERROR_LABEL_WITH_NO_DIRECTIVE_OR_INSTRUCTION, file_name, line_count);
				did_fail = TRUE;
				continue;
			}
		}
		else
		{
			after_label = line; /* no label, start from beginning */
		}

		/* check for .data */
		if (strncmp(after_label, ".data", 5) == 0 && isspace(after_label[5]))
		{
			/* check if there’s a label */
			if (label[0] != '\0')
			{
				/* add the label to the list */
				if (add_label_list(label_head, label, DC, line_count, DATA, file_name))
				{
					did_fail = TRUE;
					continue;
				}
			}
			/* encode the data */
			if (encode_data(after_label, &DC, data_list, file_name, line_count))
			{
				did_fail = TRUE;
				continue;
			}
		}
		/* check for .string */
		else if (strncmp(after_label, ".string", 7) == 0 && isspace(after_label[7]))
		{
			/* check if there’s a label */
			if (label[0] != '\0' && label[0] != '\n')
			{
				if (add_label_list(label_head, label, DC, line_count, DATA, file_name))
				{
					did_fail = TRUE;
					continue;
				}
			}
			/* encode the string */
			if (encode_string(after_label, &DC, data_list, file_name, line_count))
			{
				did_fail = TRUE;
				continue;
			}
		}
		/* check for .extern */
		else if (strncmp(after_label, ".extern", 7) == 0 && isspace(after_label[7]))
		{
			/* check if there’s a label */
			if (label[0] != '\0')
			{
				print_ext_warning(WARNING_LABEL_AT_START_EXTERN, file_name, line_count);
			}

			/* handle the extern */
			if (handle_extern(after_label, label_head, file_name, line_count))
			{
				did_fail = TRUE;
				continue;
			}
		}
		/* .entry is ignored here */
		else if (strncmp(after_label, ".entry", 6) == 0 && isspace(after_label[6]))
		{
			if (label[0] != '\0')
			{
				print_ext_warning(WARNING_LABEL_AT_START_ENTRY, file_name, line_count);
			}
			continue;
		}
		/* if it starts with a dot but isn't one of the valid ones */
		else if (strncmp(after_label, ".", 1) == 0)
		{
			print_ext_error(ERROR_INVALID_INSTRUCTION, file_name, line_count);
			did_fail = TRUE;
			continue;
		}
		else
		{
			/* must be an instruction */
			if (label[0] != '\0')
			{
				/* add the label to the list */
				if (add_label_list(label_head, label, IC, line_count, CODE, file_name))
				{
					did_fail = TRUE;
					continue;
				}
			}

			/* parse the instruction */
			if (parse_instruction(after_label, instruction_list, file_name, line_count))
			{
				did_fail = TRUE;
				continue;
			}
			/* increment the IC */
			inst = &instruction_list->tail->instruction;
			/* add the instruction length to the IC */
			IC += calculate_words(inst, file_name, line_count);
		}

		/* done with this line */
		memset(label, '\0', sizeof(label));
	}

	/* save the final counter values */
	*ICF = IC;
	*DCF = DC;

	/* Add ICF to every data address (the data address only contains data and not instructions for now) */
	current_data = data_list->head;
	while (current_data != NULL)
	{
		/* check if the address is in range */
		if (current_data->address + *ICF > MAX_MEMORY_SIZE)
		{
			handle_memory_overflow(file_name, current_data->address + *ICF);
			did_fail = TRUE;
			/* we will assume if it cannot fit in the memory, it will be ignored because the .ob will not be built*/
		}
		else
		{
			/* add ICF to the address */
			current_data->address += *ICF;
		}
		/* move to the next node */
		current_data = current_data->next;
	}

	/* same for labels of type DATA */
	label_dummy = *label_head;
	while (label_dummy != NULL)
	{
		if (label_dummy->type == DATA)
		{
			label_dummy->addr += *ICF;
		}
		label_dummy = label_dummy->next;
	}

	fclose(fp);

	/* return SUCCESS only if everything went well */
	return did_fail ? FAILURE : SUCCESS;
}
