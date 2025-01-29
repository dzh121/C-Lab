#include "../headers/errors.h"

void print_internal_error(char *error_message) {
    /* Print the internal error message */
    if (error_message) {
        fprintf(stderr, "INTERNAL ERROR: %s\n", error_message);
    } else {
        fprintf(stderr, "INTERNAL ERROR: Unknown internal error\n");
    }
}

void print_ext_error(char *error_message, char *file_name, int line_number) {
    /* Print the error message with the file name and line number */
    if (!error_message) {
        fprintf(stderr, "ERROR: Unknown external error\n"); /* Unknown error */
        return;
    }

    if (!file_name) {
        fprintf(stderr, "ERROR: %s | File: Unknown | Line: %d\n", error_message, line_number); /* Unknown file */
        return;
    }

    if (line_number < 0) {
        fprintf(stderr, "ERROR: %s | File: %s\n", error_message, file_name); /* No line number */
    } else {
        fprintf(stderr, "ERROR: %s | File: %s | Line: %d\n", error_message, file_name, line_number); /* Full details */
    }
}

void print_ext_warning(char *warning_message, char *file_name, int line_number) {
    /* Print the warning message with the file name and line number */
    if (!warning_message) {
        fprintf(stderr, "WARNING: Unknown external warning.\n");
        return;
    }

    if (!file_name) {
        fprintf(stderr, "WARNING: %s | File: Unknown | Line: %d\n", warning_message, line_number);
        return;
    }

    if (line_number < 0) {
        fprintf(stderr, "WARNING: %s | File: %s\n", warning_message, file_name);
    } else {
        fprintf(stderr, "WARNING: %s | File: %s | Line: %d\n", warning_message, file_name, line_number);
    }
}
