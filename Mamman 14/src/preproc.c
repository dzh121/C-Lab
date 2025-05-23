#include "../headers/preproc.h"

char* remove_white_spaces(char* line)
{
	int i = 0, j = 0, inside_quotes = 0; /* Indexes and flags */
	char* cleaned_line; /* Cleaned line buffer */

	if (!line || strlen(line) == 0 || line[0] == ';') return NULL; /* skip empty or comment lines */

	cleaned_line = (char*)handle_malloc(strlen(line) + 1); /* alloc space for cleaned string */
	if (!cleaned_line) return NULL;

	while (line[i] != '\0')
	{
		if (line[i] == '"') /* toggle flag if we enter/exit quotes */
		{
			inside_quotes = !inside_quotes;
			cleaned_line[j++] = line[i++];
			continue;
		}

		if (!isspace(line[i]) || inside_quotes) /* keep char if not a space or if inside quotes */
		{
			cleaned_line[j++] = line[i++];
		}
		else if (j > 0 && !isspace(cleaned_line[j - 1])) /* add a single space if last wasn’t a space */
		{
			cleaned_line[j++] = ' ';
			while (isspace(line[i])) i++; /* skip any extra spaces */
		}
		else
		{
			i++; /* skip leading or repeated spaces */
		}
	}

	while (j > 0 && (isspace(cleaned_line[j - 1]))) j--; /* remove trailing space */
	cleaned_line[j] = '\0'; /* null-terminate the string */
	return cleaned_line;
}

int isValidName(char* name, int line, char* file_name)
{
	if (!name || name[0] == '\0' || name[0] == '\n') /* empty macro name */
	{
		print_ext_error(ERROR_MACRO_NO_NAME, file_name, line);
		return FAILURE;
	}

	if (strlen(name) > MAX_LABEL_SIZE) /* too long */
	{
		print_ext_error(ERROR_MACRO_TOO_LONG, file_name, line);
		return FAILURE;
	}

	if (isdigit(name[0])) /* starts with number */
	{
		print_ext_error(ERROR_MACRO_NAME_INVALID, file_name, line);
		return FAILURE;
	}

	if (is_reserved(name)) /* reserved keyword */
	{
		print_ext_error(ERROR_MACRO_NAME_RESERVED, file_name, line);
		return FAILURE;
	}

	return SUCCESS; /* valid name */
}

int getLineLength(char* line)
{
	int i = 0; /* index counter */
	while (line[i] != '\0' && line[i] != '\n') i++; /* count till end or newline */
	return i; /* return the length */
}

/* Preprocessor function to handle macro expansion */
int preproc(char* file_as, char* file_am)
{
	FILE *fptr_as, *fptr_am; /* File pointers for source and output files */
	char macro_name[MAX_LINE_LENGTH], line[MAX_LINE_LENGTH * 2];
	/* Buffers for macro name and line times 2 to check if too long */
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
		return FAILURE;
	}

	/* Open Output File (.am) */
	fptr_am = fopen(file_am, "w");
	if (fptr_am == NULL)
	{
		print_internal_error(ERROR_FILE_OPEN_OUTPUT);
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
		else
		{
			line[0] = '\0'; /* Empty line or comment */
		}

		/* Skip empty lines */
		if (line[0] == '\0' || strcmp(line, "\n") == 0)
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
			if (!macro_content)
			{
				did_fail = TRUE;
				continue;
			}
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
				memset(macro_name, '\0', sizeof(macro_name));
				continue;
			}

			/* Free macro_name and macro_content after adding */
			free(macro_content);
			macro_content = NULL;
			memset(macro_name, '\0', sizeof(macro_name));

			content_size = 0;
			continue;
		}
		if (in_macro)
		{
			line_length = strlen(line); /* Get line length without the null terminator */

			/* Reallocate memory to accommodate the new line + newline character + null terminator */
			temp = realloc(macro_content, content_size + line_length + 2);
			if (!temp)
			{
				free(macro_content);
				macro_content = NULL;
				memset(macro_name, '\0', sizeof(macro_name));
				print_internal_error(ERROR_MEMORY_REALLOCATION);
				did_fail = TRUE;
				continue;
			}
			macro_content = temp;

			/* Copy the new line into the buffer */
			strncpy(macro_content + content_size, line, line_length);
			content_size += line_length;

			/* Ensure the line ends with a newline character */
			if (macro_content[content_size - 1] != '\n')
			{
				macro_content[content_size] = '\n';
				content_size++;
			}

			/* Null-terminate the macro content */
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
