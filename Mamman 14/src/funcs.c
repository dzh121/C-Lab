#include "../headers/funcs.h"

const char* registers[] = {
	"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"
};

const char* saved_words[] = {
	"mov", "cmp", "add", "sub", "lea", "clr", "not", "inc",
	"dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop", "data", "string",
	"entry", "extern"
};

void* handle_malloc(long size)
{
	void* ptr = malloc(size);
	if (!ptr)
	{
		print_internal_error(ERROR_MEMORY_ALLOCATION);
		exit(1);
	}
	return ptr;
}

int is_reserved(char* line)
{
	int i;
	/* Check if the word matches any register */
	for (i = 0; i < sizeof(registers) / sizeof(registers[0]); i++)
	{
		if (strcmp(line, registers[i]) == 0)
		{
			return 1; /* Found in registers */
		}
	}

	/* Check if the word matches any command */
	for (i = 0; i < sizeof(saved_words) / sizeof(saved_words[0]); i++)
	{
		if (strcmp(line, saved_words[i]) == 0)
		{
			return 1; /* Found in commands */
		}
	}
	if (strcmp(line, ".data") == 0 || strcmp(line, ".string") == 0 ||
		strcmp(line, ".entry") == 0 || strcmp(line, ".extern") == 0)
	{
		return 1; /* Reserved name */
	}

	return 0; /* Not reserved */
}

void init_data_list(DataList* list)
{
	list->head = NULL;
}

void add_data_node(DataList* list, int address, int value)
{
	DataNode* new_node = (DataNode*)handle_malloc(sizeof(DataNode));
	DataNode* current;
	new_node->address = address;

	new_node->data = value;
	new_node->next = NULL;

	/* Insert in sorted order by address */
	if (!list->head || address < list->head->address)
	{
		new_node->next = list->head;
		list->head = new_node;
	}
	else
	{
		current = list->head;
		while (current->next && current->next->address < address)
		{
			current = current->next;
		}
		new_node->next = current->next;
		current->next = new_node;
	}
}

void print_data_list(DataList* list)
{
	DataNode* current = list->head;
	char *hexValue = (char*)handle_malloc(7);
	while (current)
	{
		sprintf(hexValue, "%06x", current->data & 0xFFFFFF);
		printf("%04d %s\n", current->address, hexValue);
		current = current->next;
	}
	if (hexValue)
	{
		free(hexValue);
	}
}

void free_data_list(DataList* list)
{
	DataNode* current = list->head;
	while (current)
	{
		DataNode* temp = current;
		current = current->next;
		free(temp);
	}
	list->head = NULL;
}

void free_instruction_list(InstructionList* list)
{
	InstructionNode* current = list->head;
	InstructionNode* temp;

	while (current != NULL)
	{
		temp = current;
		current = current->next;
		free(temp);
	}

	list->head = NULL;
	list->tail = NULL;
}

void free_label_list(label_table* head)
{
	label_table* current = head;
	label_table* temp;
	ADDRESS_LIST* addr_current;
	ADDRESS_LIST* addr_temp;
	while (current != NULL)
	{
		temp = current;

		/* Free the address list if it's an EXTERN label */
		addr_current = current->addr_list;
		while (addr_current != NULL)
		{
			addr_temp = addr_current;
			addr_current = addr_current->next;
			free(addr_temp);
		}

		current = current->next;
		free(temp);
	}
}


int isValidLabel(char* label)
{
	char* c;

	if (label == NULL || *label == '\0' || isspace(*label))
	{
		return 0; /* Empty label */
	}

	if (strlen(label) > MAX_LABEL_SIZE)
	{
		return 0; /* Too long */
	}

	if (!isalpha(label[0]))
	{
		return 0; /* Must start with a letter */
	}

	for (c = label; *c != '\0'; c++)
	{
		if (!isalnum(*c))
		{
			return 0; /* Contains invalid character */
		}
	}

	if (is_reserved((char*)label))
	{
		return 0; /* Reserved keyword */
	}

	return 1; /* Valid label */
}

int build_output_files(char* file_name, DataList* data_list, label_table* label_head, int ICF, int DCF)
{
	FILE *ob_fp = NULL, *ent_fp = NULL, *ext_fp = NULL;
	char ob_file_name[MAX_LINE_LENGTH], ent_file_name[MAX_LINE_LENGTH], ext_file_name[MAX_LINE_LENGTH];
	DataNode* current_data = data_list->head;
	label_table* current_label = label_head;
	ADDRESS_LIST* current_address = NULL;
	int has_entries = 0, has_externals = 0;
	char *hexValue = (char*)handle_malloc(7);

	/* Build the output file names */
	sprintf(ob_file_name, "%s.ob", file_name);
	sprintf(ent_file_name, "%s.ent", file_name);
	sprintf(ext_file_name, "%s.ext", file_name);

	/* Open the object file */
	ob_fp = fopen(ob_file_name, "w");
	if (!ob_fp)
	{
		print_internal_error(ERROR_FILE_OPEN_OUTPUT);
		return 0;
	}

	/* Write the object file */
	fprintf(ob_fp, "%7d %d\n", ICF - 100, DCF);
	while (current_data)
	{
		sprintf(hexValue, "%06x", current_data->data & 0xFFFFFF);
		fprintf(ob_fp, "%07d %s\n", current_data->address, hexValue);
		current_data = current_data->next;
	}
	if (hexValue)
	{
		free(hexValue);
	}
	/* Check and write the entry and external files */
	while (current_label)
	{
		if (current_label->type == ENTRY)
		{
			if (!ent_fp)
			{
				ent_fp = fopen(ent_file_name, "w");
				if (!ent_fp)
				{
					print_internal_error(ERROR_FILE_OPEN_OUTPUT);
					fclose(ob_fp);
					return 0;
				}
			}
			fprintf(ent_fp, "%s %07d\n", current_label->name, current_label->addr);
			has_entries = 1;
		}
		else if (current_label->type == EXTERN)
		{
			current_address = current_label->addr_list;
			while (current_address)
			{
				if (!ext_fp)
				{
					ext_fp = fopen(ext_file_name, "w");
					if (!ext_fp)
					{
						print_internal_error(ERROR_FILE_OPEN_OUTPUT);
						fclose(ob_fp);
						if (ent_fp) fclose(ent_fp);
						return 0;
					}
				}
				fprintf(ext_fp, "%s %07d\n", current_label->name, current_address->addr);
				current_address = current_address->next;
				has_externals = 1;
			}
		}
		current_label = current_label->next;
	}

	/* Close the object file */
	fclose(ob_fp);

	/* Close the entry file if it was opened */
	if (ent_fp) fclose(ent_fp);

	/* Close the external file if it was opened */
	if (ext_fp) fclose(ext_fp);

	/* Delete empty files */
	if (!has_entries) remove(ent_file_name);
	if (!has_externals) remove(ext_file_name);

	return 1;
}

/* Add suffix to a filename dynamically */
char* add_suffix(char* file_name, char* suffix)
{
	char* new_name = (char*)handle_malloc(strlen(file_name) + strlen(suffix) + 1);
	strcpy(new_name, file_name);
	strcat(new_name, suffix);
	return new_name;
}

char* trim_whitespace(char* str)
{
	char* end;

	/* Trim leading spaces. */
	while (isspace((unsigned char)*str)) str++;

	if (*str == '\0') return str; /* All spaces. */

	/* Trim trailing spaces. */
	end = str + strlen(str) - 1;
	while (end > str && isspace((unsigned char)*end)) end--;

	/* Null-terminate. */
	*(end + 1) = '\0';

	return str;
}
