#include "../headers/errors.h"
#include <stdio.h>

void print_internal_error(char *error_message) {
	if (error_message) {
		fprintf(stderr, "INTERNAL ERROR: %s\n", error_message);
	} else {
		fprintf(stderr, "INTERNAL ERROR: Unknown internal error\n");
	}
}

void print_ext_error(char *error_message, char *file_name, int line_number) {
	if (line_number < 0) {
		fprintf(stderr, "ERROR: %s | File: %s\n", error_message, file_name);
	}
	else if (error_message && file_name) {
		fprintf(stderr, "ERROR: %s | File: %s | Line: %d\n", error_message, file_name, line_number);
	} else {
		fprintf(stderr, "ERROR: Unknown external error\n");
	}
}
void print_ext_warning(char *warning_message, char *file_name, int line_number) {
	if (line_number < 0) {
		fprintf(stderr, "WARNING: %s | File: %s\n", warning_message, file_name);
	} else if (warning_message && file_name) {
		fprintf(stderr, "WARNING: %s | File: %s | Line: %d\n", warning_message, file_name, line_number);
	} else {
		fprintf(stderr, "WARNING: Unknown external warning.\n");
	}
}

