#include "errors.h"
#include <stdio.h>

void print_internal_error(char *error_message) {
	if (error_message) {
		fprintf(stderr, "INTERNAL ERROR: %s\n", error_message);
	} else {
		fprintf(stderr, "INTERNAL ERROR: Unknown internal error.\n");
	}
}

void print_ext_error(char *error_message, char *file_name, int line_number) {
	if (error_message && file_name) {
		fprintf(stderr, "ERROR: %s | File: %s | Line: %d\n", error_message, file_name, line_number);
	} else {
		fprintf(stderr, "ERROR: Unknown external error.\n");
	}
}
