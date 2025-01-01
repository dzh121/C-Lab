#include "funcs.h"
#include "errors.h"
#include <string.h>
#include "assembler.h"
const char *registers[] = {
	"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"
};

const char *commands[] = {
	"mov", "cmp", "add", "sub", "lea", "clr", "not", "inc",
	"dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop"
};

const char *directives[] = {
	".data", ".string", ".entry", ".extern"
};

void *handle_malloc(long size) {
	void *ptr = malloc(size);
	if (!ptr) {
		print_internal_error(ERROR_MEMORY_ALLOCATION);
		exit(1);
	}
	return ptr;
}

int is_reserved(char *line)
{
	int i;
	/* Check if the word matches any register */
	for (i = 0; i < sizeof(registers) / sizeof(registers[0]); i++) {
		if (strcmp(line, registers[i]) == 0) {
			return 1; /* Found in registers */
		}
	}

	/* Check if the word matches any command */
	for (i = 0; i < sizeof(commands) / sizeof(commands[0]); i++) {
		if (strcmp(line, commands[i]) == 0) {
			return 1; /* Found in commands */
		}
	}

	/* Check if the word matches any directives */
	for (i = 0; i < sizeof(directives) / sizeof(directives[0]); i++) {
		if (strcmp(line, directives[i]) == 0) {
			return 1; /* Found in directives */
		}
	}

	return 0; /* Not reserved */
}
void init_data_list(DataList *list) {
	list->head = NULL;
}

void add_data_node(DataList *list, int address, int value) {
	DataNode *new_node = (DataNode *)handle_malloc(sizeof(DataNode));

	new_node->address = address;
	snprintf(new_node->data, sizeof(new_node->data), "%06X", value & 0xFFFFFF); /* Convert to 6-bit hex */
	new_node->next = NULL;

	/* Insert in sorted order by address */
	if (!list->head || address < list->head->address) {
		new_node->next = list->head;
		list->head = new_node;
	} else {
		DataNode *current = list->head;
		while (current->next && current->next->address < address) {
			current = current->next;
		}
		new_node->next = current->next;
		current->next = new_node;
	}
}

void print_data_list(DataList *list) {
	DataNode *current = list->head;
	while (current) {
		printf("%04d %s\n", current->address, current->data);
		current = current->next;
	}
}

void free_data_list(DataList *list) {
	DataNode *current = list->head;
	while (current) {
		DataNode *temp = current;
		current = current->next;
		free(temp);
	}
	list->head = NULL;
}
