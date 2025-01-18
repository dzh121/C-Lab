#include "preproc.h"
#include "errors.h"

/* Remove spaces from a string */
char *remove_white_spaces(char *line) {
    int i = 0, j = 0;
    int inside_quotes = 0; /* Flag to track if we're inside quotation marks */
    char *cleaned_line = (char *)handle_malloc(strlen(line) + 2);  /* +2 for potential '\n' and '\0' */
    int start_of_line = TRUE; /* Flag to track if we're at the start of the line */
    if (!line) {
        return NULL;
    }

    while (line[i] != '\0') {
        /* Handle comments (terminate at ';') */
        if (line[i] == ';' && !inside_quotes) {
            cleaned_line[j++] = '\0'; /* Add newline */
            break; /* Ignore everything after ';' */
        }

        /* Handle quotation marks */
        if (line[i] == '"') {
            inside_quotes = !inside_quotes; /* Toggle the flag */
            cleaned_line[j++] = line[i++];
            start_of_line = FALSE;
            continue;
        }

        /* Skip leading spaces */
        if (start_of_line && isspace(line[i])) {
            i++; /* Skip leading spaces */
            continue;
        }

        /* Handle spaces and tabs */
        if (isspace(line[i]) && !inside_quotes) {
            /* Skip consecutive spaces/tabs */
            while (isspace(line[i])) {
                i++;
            }
            if (line[i] != '\0' && line[i] != ';') {
                cleaned_line[j++] = ' '; /* Add a single space */
            }
            start_of_line = FALSE;
            continue;
        }

        /* Copy regular characters */
        cleaned_line[j++] = line[i++];
        start_of_line = FALSE;
    }

    /* Null-terminate the cleaned string */
    cleaned_line[j] = '\0';

    return cleaned_line;
}

int isValidName(char *name, int line, char *file_name) {
    /* Check if the macro name is empty */
    if (!name || name[0] == '\0' || name[0] == '\n') {
        print_ext_error("Macro name is missing.", file_name, line);
        return FALSE;
    }

    /* Check if the macro name is too long */
    if (strlen(name) > MAX_LABEL_SIZE) {
        print_ext_error(ERROR_MACRO_TOO_LONG, file_name, line);
        return FALSE;
    }
    /* Check if the macro name is invalid */
    if(isdigit(name[0])){
        print_ext_error(ERROR_MACRO_NAME_INVALID, file_name, line);
        return FALSE;
    }
    /* Check if the macro name is reserved */
    if(is_reserved(name)){
        print_ext_error(ERROR_MACRO_NAME_RESERVED, file_name, line);
        return FALSE;
    }

    return TRUE;
}
void print_hex_string(char *str) {
    size_t i;
    if (!str) {
        printf("String is NULL\n");
        return;
    }
    printf("Hexadecimal representation: ");
    for (i = 0; i < strlen(str); i++) {
        printf("%02X ", (unsigned char)str[i]);
    }
    printf("\n");
}
/* Preprocessor function to handle macro expansion */
int preproc(char *file_name) {
    FILE *fptr_as, *fptr_am;
    char *file_am, *file_as;
    char macro_name[MAX_LINE_LENGTH];
    char *macro_content = NULL;
    int line_count = 0, macro_line = 0, content_size = 0, line_length;
    char line[MAX_LINE_LENGTH];
    macro_node *head = NULL;
    int in_macro = 0; /* Flag for macro block detection */
    macro_node *macro_found = NULL;
    char *cleaned_line = NULL;
    char *temp = NULL;
    char *after_mcroend = NULL;
    char *after_mcroname = NULL;
    int did_fail = FALSE;


    /* Add suffixes for source (.as) and macro (.am) files */
    file_as = add_suffix(file_name, ".as");
    file_am = add_suffix(file_name, ".am");

    /* Open Source File (.as) */
    fptr_as = fopen(file_as, "r");
    if (fptr_as == NULL) {
        print_internal_error(ERROR_FILE_OPEN_SOURCE);
        free(file_as);
        free(file_am);
        return 0;
    }

    /* Open Output File (.am) */
    fptr_am = fopen(file_am, "w");
    if (fptr_am == NULL) {
        print_internal_error(ERROR_FILE_OPEN_OUTPUT);
        free(file_as);
        free(file_am);
        fclose(fptr_as);
        return 0;
    }

    memset(macro_name, '\0', sizeof(macro_name));
    memset(line , '\0', sizeof(line));

    /* Read and Parse Lines */
    while (fgets(line, sizeof(line), fptr_as) != NULL) {
        line_count++;

        cleaned_line = remove_white_spaces(line); /* Clean the line */
        if (cleaned_line) {
            strncpy(line, cleaned_line, sizeof(line) - 1); /* Copy cleaned content back to line */
            line[sizeof(line) - 1] = '\0'; /* Ensure null-termination */
            free(cleaned_line); /* Free dynamically allocated cleaned line */
        }

        /* Skip empty lines */
        if (line[0] == '\0' || strcmp(line, "\n") == 0) {
            continue;
        }

        /* Detect macro declaration */
        if (strncmp(line, "mcro", 4) == 0 && strncmp(line, "mcroend", 7) != 0) {
            in_macro = 1;
            macro_line = line_count;

            /* Extract macro name */
            sscanf(line + 4, "%s", macro_name);
            after_mcroname = line + 5 + strlen(macro_name); /* Skip 'mcro' and the name and the space*/

            if (after_mcroname[0] != '\0' && after_mcroname[0] != '\n') {
                print_ext_error(ERROR_EXTRANEOUS_TEXT_MCRONAME, file_as, line_count);
                did_fail = TRUE;
            }

            macro_name[MAX_LINE_LENGTH - 1] = '\0'; /* Ensure null-termination */
            if (!isValidName(macro_name, macro_line, file_as))
            {
                did_fail = TRUE;
            }

            /* Allocate initial memory for macro content */
            macro_content = handle_malloc(1);
            macro_content[0] = '\0';
            content_size = 1;
            continue;
        }
        if (strncmp(line, "mcroend", 7) == 0) {
            in_macro = 0;
            after_mcroend = line + 7;
            if (after_mcroend[0] != '\0' && after_mcroend[0] != '\n') {
                print_ext_error(ERROR_EXTRANEOUS_TEXT_MCROEND, file_as, line_count);
                did_fail = TRUE;
            }
            /* Add macro to the linked list */
            if (add_node(&head, macro_name, macro_content, macro_line, file_as) != 1) {
                did_fail = TRUE;
            }

            /* Free macro_name and macro_content after adding */
            free(macro_content);
            memset(macro_name, '\0', sizeof(macro_name));
            macro_content = NULL;
            content_size = 0;
            continue;
        }

        /* Handle macro content dynamically */
        if (in_macro) {
            line_length = (int) strlen(line) + 2; /* +2 for '\n' and null terminator */

            /* Reallocate memory if buffer is too small */
            temp = realloc(macro_content, content_size + line_length);
            if (!temp) {
                print_internal_error(ERROR_MEMORY_REALLOCATION);
                exit(1);
            }
            macro_content = temp;
            /* Add new line to macro content */
            strcat(macro_content, line);

            /* If the line does not end with '\n', add it */
            if (line[strlen(line) - 1] != '\n') {
                strcat(macro_content, "\n");
                content_size += 1; /* Account for added newline */
            }

            macro_content[content_size + line_length] = '\0'; /* Ensure null-termination */

            content_size += line_length;
            continue;
        }
        macro_found = search_list(head, line);

        if (macro_found != NULL) {
            fputs(macro_found->content, fptr_am);
            continue;
        }

        /* Write regular lines to the output file */
        fputs(line, fptr_am);
        fputs("\n", fptr_am);
    }
    if (line_count == 0) {
        print_ext_error(ERROR_EMPTY_FILE, file_name, 0);
        did_fail = TRUE;
    }
    /* Check if macro was left open without mcroend */
    if (in_macro) {
        print_ext_error(ERROR_MACRO_NOT_CLOSED, file_as, macro_line);
        did_fail = TRUE;
    }

    fclose(fptr_as);
    fclose(fptr_am);
    free(file_as);
    free(file_am);
    free_macro_list(head);

    if (did_fail)
    {
        if (remove(file_am) != 0)
        {
            print_internal_error(ERROR_FAILED_REMOVAL);
        }
        return 0;
    }

    return 1;
}
