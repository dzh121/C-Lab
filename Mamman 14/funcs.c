#include "funcs.h"
#include "errors.h"
#include <string.h>

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