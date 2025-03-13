#include "../headers/errors.h"

#include "../headers/globals.h"

void handle_memory_overflow(char* file_name, int overflow_address)
{
	fprintf(stdout, "ERROR: MEMORY OVERFLOW Address: %d out of %d | File: %s\n",
	        overflow_address, MAX_MEMORY_SIZE, file_name);
}


void print_internal_error(char* error_message)
{
	/* Print the internal error message */
	if (error_message)
	{
		fprintf(stdout, "INTERNAL ERROR: %s\n", error_message);
	}
	else
	{
		fprintf(stdout, "INTERNAL ERROR: Unknown internal error\n");
	}
}

void print_ext_error(char* error_message, char* file_name, int line_number)
{
	/* Print the error message with the file name and line number */
	if (!error_message)
	{
		fprintf(stdout, "ERROR: Unknown external error\n"); /* Unknown error */
		return;
	}

	if (!file_name)
	{
		fprintf(stdout, "ERROR: %s | File: Unknown | Line: %d\n", error_message, line_number); /* Unknown file */
		return;
	}

	if (line_number < 0)
	{
		fprintf(stdout, "ERROR: %s | File: %s\n", error_message, file_name); /* No line number */
	}
	else
	{
		fprintf(stdout, "ERROR: %s | File: %s | Line: %d\n", error_message, file_name, line_number); /* Full details */
	}
}

void print_ext_warning(char* warning_message, char* file_name, int line_number)
{
	/* Print the warning message with the file name and line number */
	if (!warning_message)
	{
		fprintf(stdout, "WARNING: Unknown external warning.\n");
		return;
	}

	if (!file_name)
	{
		fprintf(stdout, "WARNING: %s | File: Unknown | Line: %d\n", warning_message, line_number);
		return;
	}

	if (line_number < 0)
	{
		fprintf(stdout, "WARNING: %s | File: %s\n", warning_message, file_name);
	}
	else
	{
		fprintf(stdout, "WARNING: %s | File: %s | Line: %d\n", warning_message, file_name, line_number);
	}
}
