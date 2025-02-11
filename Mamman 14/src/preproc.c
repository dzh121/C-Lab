#include "../headers/preproc.h"

/* Remove spaces from a string */
char* remove_white_spaces(char* line)
{
	int i = 0, j = 0, inside_quotes = 0; /* Indexes and flags */
	char* cleaned_line; /* Cleaned line buffer */

	if (!line || strlen(line) == 0) return NULL; /* Handle empty line */

	cleaned_line = (char*)handle_malloc(strlen(line) + 1); /* Allocate memory for the cleaned line */
	while (line[i] != '\0')
	{
		/* Stop processing at ';' if not inside quotes */
		if (line[i] == ';' && !inside_quotes) break;

		/* Toggle quote flag if encountering a quote */
		if (line[i] == '"')
		{
			inside_quotes = !inside_quotes;
			cleaned_line[j++] = line[i++];
			continue;
		}

		/* Handle non-space characters or characters inside quotes */
		if (!isspace(line[i]) || inside_quotes)
		{
			cleaned_line[j++] = line[i++];
		}
		/* Handle spaces outside quotes */
		else if (j > 0 && !isspace(cleaned_line[j - 1]))
		{
			cleaned_line[j++] = ' '; /* Add a single space */
			while (isspace(line[i])) i++; /* Skip consecutive spaces */
		}
		/* Skip leading spaces */
		else
		{
			i++;
		}
	}

	/* Remove trailing spaces */
	while (j > 0 && (isspace(cleaned_line[j - 1])))
	{
		j--;
	}
	/* Null-terminate the cleaned line */
	cleaned_line[j] = '\0';
	return cleaned_line;
}

int isValidName(char* name, int line, char* file_name)
{
	/* Check if the macro name is empty */
	if (!name || name[0] == '\0' || name[0] == '\n')
	{
		print_ext_error(ERROR_MACRO_NO_NAME, file_name, line);
		return FAILURE;
	}

	/* Check if the macro name is too long */
	if (strlen(name) > MAX_LABEL_SIZE)
	{
		print_ext_error(ERROR_MACRO_TOO_LONG, file_name, line);
		return FAILURE;
	}

	/* Check if the macro name is invalid */
	if (isdigit(name[0]))
	{
		print_ext_error(ERROR_MACRO_NAME_INVALID, file_name, line);
		return FAILURE;
	}

	/* Check if the macro name is reserved */
	if (is_reserved(name))
	{
		print_ext_error(ERROR_MACRO_NAME_RESERVED, file_name, line);
		return FAILURE;
	}

	return SUCCESS; /* Valid name */
}

int getLineLength(char* line)
{
	int i = 0; /* Index */
	/* Loop through the line */
	while (line[i] != '\0' && line[i] != '\n')
	{
		i++; /* Increment index */
	}
	return i; /* Return the line length */
}

/* Preprocessor function to handle macro expansion */
int preproc(char* file_as, char* file_am)
{
	FILE *fptr_as, *fptr_am; /* File pointers for source and output files */
	char macro_name[MAX_LINE_LENGTH], line[MAX_LINE_LENGTH * 2]; /* Buffers for macro name and line times 2 to check if too long */
	char *macro_content = NULL, *cleaned_line = NULL, *temp = NULL;
	/* Pointers for dynamic content and temporary values */
	char *after_mcroend = NULL, *after_mcroname = NULL; /* Pointers for text after 'mcroend' and macro name */
	int line_count = 0, macro_line = 0, content_size = 0, line_length = 0; /* Counters and sizes */
	int in_macro = 0, did_fail = FALSE; /* Flags for macro detection and error handling */
	macro_node *head = NULL, *macro_found = NULL; /* Linked list head and macro search result */

	/* Open Source File (.as) */
	fptr_as = fopen(file_as, "r");
	if (fptr_as == NULL)
	{
		print_internal_error(ERROR_FILE_OPEN_SOURCE);
		free(file_as);
		free(file_am);
		return FAILURE;
	}

	/* Open Output File (.am) */
	fptr_am = fopen(file_am, "w");
	if (fptr_am == NULL)
	{
		print_internal_error(ERROR_FILE_OPEN_OUTPUT);
		free(file_as);
		free(file_am);
		fclose(fptr_as);
		return FAILURE;
	}

	/* Initialize buffers */
	memset(macro_name, '\0', sizeof(macro_name));
	memset(line, '\0', sizeof(line));

	/* Read and Parse Lines */
	while (fgets(line, sizeof(line), fptr_as) != NULL)
	{
		line_count++; /* Increment line counter */
		/* Check if not to long */
		if (getLineLength(line) > MAX_LINE_LENGTH)
		{
			print_ext_error(ERROR_LINE_TOO_LONG, file_as, line_count);
			did_fail = TRUE;
			continue;
		}

		cleaned_line = remove_white_spaces(line); /* Clean the line */

		if (cleaned_line)
		{
			strncpy(line, cleaned_line, sizeof(line) - 1); /* Copy cleaned content back to line */
			line[sizeof(line) - 1] = '\0'; /* Ensure null-termination */
			free(cleaned_line); /* Free dynamically allocated cleaned line */
			cleaned_line = NULL; /* Reset pointer to avoid dangling references */
		}

		/* Skip empty lines */
		if (!line || line[0] == '\0' || strcmp(line, "\n") == 0)
		{
			continue;
		}
		/* Detect macro declaration */
		if (strncmp(line, "mcro", 4) == 0 && (line[4] == '\0' || isspace(line[4])))
		{
			in_macro = 1; /* Set flag to indicate we are inside a macro */
			macro_line = line_count; /* Save the line number for error messages */
			/* Extract macro name */
			sscanf(line + 4, "%s", macro_name);
			after_mcroname = line + 5 + strlen(macro_name); /* Skip 'mcro' and the name and the space*/

			if (after_mcroname[0] != '\0' && after_mcroname[0] != '\n')
			{
				print_ext_error(ERROR_EXTRANEOUS_TEXT_MCRONAME, file_as, line_count);
				did_fail = TRUE;
				continue;
			}

			macro_name[MAX_LINE_LENGTH - 1] = '\0'; /* Ensure null-termination */
			if (isValidName(macro_name, macro_line, file_as))
			{
				did_fail = TRUE;
				continue;
			}

			/* Allocate initial memory for macro content */
			macro_content = handle_malloc(1);
			macro_content[0] = '\0';
			content_size = 0;
			continue;
		}
		if (strncmp(line, "mcroend", 7) == 0 && (line[7] == '\0' || isspace(line[7])))
		{
			if (!in_macro)
			{
				print_ext_error(ERROR_MACRO_NOT_OPENED, file_as, line_count);
				did_fail = TRUE;
				continue;
			}
			in_macro = 0; /* Exit macro definition mode */
			after_mcroend = line + 7;

			/* Check for extraneous text after 'mcroend' */
			if (*after_mcroend != '\0' && *after_mcroend != '\n')
			{
				print_ext_error(ERROR_EXTRANEOUS_TEXT_MCROEND, file_as, line_count);
				did_fail = TRUE;
				continue;
			}

			/* Add macro to the linked list */
			if (add_node(&head, macro_name, macro_content, macro_line, file_as))
			{
				did_fail = TRUE;
				free(macro_content);
				macro_content = NULL;
				continue;
			}

			/* Free macro_name and macro_content after adding */
			free(macro_content);
			memset(macro_name, '\0', sizeof(macro_name));
			macro_content = NULL;
			content_size = 0;
			continue;
		}
		if (in_macro)
		{
			line_length = (int)strlen(line) + 1; /* +1 for null terminator */
			/* Reallocate memory if buffer is too small */
			temp = realloc(macro_content, content_size + line_length + 1);
			if (!temp)
			{
				free(macro_content);
				macro_content = NULL;
				print_internal_error(ERROR_MEMORY_REALLOCATION);
				did_fail = TRUE;
				continue;
			}
			macro_content = temp;

			/* Add new line to macro content */
			strncpy(macro_content + content_size, line, line_length);
			content_size += line_length;

			/* If the line does not end with '\n', add it */
			if (macro_content[content_size - 1] != '\n')
			{
				strncat(macro_content, "\n", 1);
				content_size += 1;
			}

			/* Ensure null-termination */
			macro_content[content_size] = '\0';
			continue;
		}


		macro_found = search_list(head, line); /* Search for macro in the list */

		if (macro_found != NULL)
		{
			fputs(macro_found->content, fptr_am); /* Write macro content to the output file */
			continue;
		}

		/* Write regular lines to the output file */
		fputs(line, fptr_am); /* Write the line to the output file */
		fputs("\n", fptr_am); /* Add a newline */
	}
	if (line_count == 0)
	{
		print_ext_error(ERROR_EMPTY_FILE, file_as, 0); /* Handle empty file */
		did_fail = TRUE; /* Set failure flag */
	}
	/* Check if macro was left open without mcroend */
	if (in_macro)
	{
		print_ext_error(ERROR_MACRO_NOT_CLOSED, file_as, macro_line);
		did_fail = TRUE;
	}

	/* Free memory and close files */
	fclose(fptr_as);
	fclose(fptr_am);
	free_macro_list(head);

	/* Remove output file if an error occurred */
	if (did_fail)
	{
		if (remove(file_am) != 0)
		{
			print_internal_error(ERROR_FAILED_REMOVAL);
		}
		return FAILURE;
	}

	return SUCCESS; /* Success */
}
