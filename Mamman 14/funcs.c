#include "funcs.h"
#include "errors.h"
#include <string.h>
#include "assembler.h"
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

	new_node->address = address;

	/* Convert to a 6-character hex string manually */
	sprintf(new_node->data, "%06X", value & 0xFFFFFF);

	new_node->next = NULL;

	/* Insert in sorted order by address */
	if (!list->head || address < list->head->address)
	{
		new_node->next = list->head;
		list->head = new_node;
	}
	else
	{
		DataNode* current = list->head;
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
	while (current)
	{
		printf("%04d %s\n", current->address, current->data);
		current = current->next;
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

int isValidLabel(char *label) {
	char *c;

	if (label == NULL || *label == '\0' || isspace(*label)) {
		return 0; /* Empty label */
	}

	if (strlen(label) > MAX_LABEL_SIZE) {
		return 0; /* Too long */
	}

	if (!isalpha(label[0])) {
		return 0; /* Must start with a letter */
	}

	for (c = label; *c != '\0'; c++) {
		if (!isalnum(*c)) {
			return 0; /* Contains invalid character */
		}
	}

	if (is_reserved((char *)label)) {
		return 0; /* Reserved keyword */
	}

	return 1; /* Valid label */
}

int build_output_files(char* file_name, DataList* data_list, label_table* label_head, int ICF, int DCF)
{
	FILE* ob_fp, * ent_fp, * ext_fp;
	char ob_file_name[MAX_LINE_LENGTH], ent_file_name[MAX_LINE_LENGTH], ext_file_name[MAX_LINE_LENGTH];
	DataNode* current_data = data_list->head;
	label_table* current_label = label_head;
	ADDRESS_LIST *current_address = NULL;
	/* Build the output file names */
	sprintf(ob_file_name, "%s.ob", file_name);
	sprintf(ent_file_name, "%s.ent", file_name);
	sprintf(ext_file_name, "%s.ext", file_name);

	/* Open the output files */
	ob_fp = fopen(ob_file_name, "w");
	if (!ob_fp)
	{
		print_internal_error(ERROR_FILE_OPEN_OUTPUT);
		return 0;
	}

	ent_fp = fopen(ent_file_name, "w");
	if (!ent_fp)
	{
		print_internal_error(ERROR_FILE_OPEN_OUTPUT);
		fclose(ob_fp);
		return 0;
	}

	ext_fp = fopen(ext_file_name, "w");
	if (!ext_fp)
	{
		print_internal_error(ERROR_FILE_OPEN_OUTPUT);
		fclose(ob_fp);
		fclose(ent_fp);
		return 0;
	}

	/* Write the object file */
	fprintf(ob_fp, "%7d %d\n", ICF - 100, DCF);
	while (current_data)
	{
		fprintf(ob_fp, "%07d %s\n", current_data->address, current_data->data);
		current_data = current_data->next;
	}

	/* Write the entry and external files */
	while (current_label)
	{
		if (current_label->type == ENTRY)
		{
			fprintf(ent_fp, "%s %07d\n", current_label->name, current_label->addr);
		}
		else if (current_label->type == EXTERN)
		{
			current_address = current_label->addr_list;
			while (current_address)
			{
				fprintf(ext_fp, "%s %07d\n", current_label->name, current_address->addr);
				current_address = current_address->next;
			}
		}
		current_label = current_label->next;
	}

	/* Close the output files */
	fclose(ob_fp);
	fclose(ent_fp);
	fclose(ext_fp);

	return 1;
}