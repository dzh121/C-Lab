#include "second_pass.h"

#include "code_translation.h"

int handle_entry(char *line, label_table *label_head, char *file_name, int line_count)
{
	char *token = strtok(line + 7, " "); /* Skip '.entry' and tokenize */
	char *after_label;
	label_table *label;

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
		label = search_label_list(label_head, token); /* Find label */

		if (!label) {
			print_ext_error(ERROR_ENTRY_LABEL_NOT_FOUND, file_name, line_count);
			return 0;
		}
		label->type = ENTRY;
		return 1;
	}
	return 0;
}

int second_pass(InstructionList *instruction_list, DataList *data_list, label_table *label_head, char *file_name)
{
	FILE *fp;
	char line[MAX_LINE_LENGTH], *after_label;
	int line_count = 0;
	InstructionNode *current_inst_node;
	Instruction *inst;
	int did_fail = FALSE;
	int address = 100;

	if((fp = fopen(file_name, "r")) == NULL){
		print_internal_error(ERROR_FILE_OPEN_SOURCE);
		return 0;
	}

	while (fgets(line, sizeof(line), fp) != NULL)
	{
		line_count++;
		after_label = strchr(line, ':'); /*skip : and space*/
		if (after_label != NULL) {
			after_label+=2; /*skip : and space*/
		} else {
			after_label = line; /* No label, start from the beginning of the line */
		}
		if (strncmp(after_label, ".entry", 6) == 0) {
			if (!handle_entry(after_label, label_head, file_name, line_count))
			{
				print_ext_error(ERROR_ENTRY_FAILED, file_name, line_count);
				did_fail = TRUE;
			}
		}
	}
	current_inst_node = instruction_list->head;
	while (current_inst_node != NULL) {
		inst = &current_inst_node->instruction;
		if (!process_instruction(inst, file_name, inst->line_number, data_list, label_head, address))
		{
			did_fail = TRUE;
		}
		address += calculate_words(inst, file_name, inst->line_number);
		current_inst_node = current_inst_node->next;
	}

	fclose(fp);
	if (did_fail)
	{
		return 0;
	}
	return 1;
}
