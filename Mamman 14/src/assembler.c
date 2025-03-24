#include "../headers/assembler.h"

int main(int argc, char *argv[]) {
    int file_index; /* which file we're on */
    char *input_file; /* current file name */
    char preproc_file[MAX_FILENAME_LENGTH]; /* name of the file after preprocessing */
    int *ICF, *DCF; /* counters we'll use later */
    DataList data_list; /* where data stuff goes */
    InstructionList instruction_list; /* same, but for instructions */
    label_table *label_head; /* start of the label list */
    char *file_am = NULL, *file_as = NULL; /* .am is preprocessed, .as is original */
    int total_success = 0; /* count how many files actually worked */
    int did_fail = FALSE; /* flag to know if something broke */

    /* need at least one file */
    if (argc < 2) {
        fprintf(stderr, "ERROR: No input files provided\n");
        return FAILURE;
    }

    printf("======================= STARTING ASSEMBLER =======================\n");
    printf("                    Total files to process: %d\n", argc - 1);
    printf("==================================================================\n");

    /* loop through all given files */
    for (file_index = 1; file_index < argc; file_index++) {
        input_file = argv[file_index]; /* grab filename */
        strncpy(preproc_file, input_file, sizeof(preproc_file) - 1); /* copy it */
        preproc_file[sizeof(preproc_file) - 1] = '\0'; /* always null terminate */

        printf("\n==================================================================\n");
        printf("                 Processing File %d of %d: %s                 \n",
               file_index, argc - 1, input_file);
        printf("===================================================================\n");

        /* get space for counters */
        ICF = (int *) handle_malloc(sizeof(int));
        if (!ICF) {
            continue;
        }
        DCF = (int *) handle_malloc(sizeof(int));
        if (!DCF) {
            free(ICF);
            continue;
        }

        /* setup all lists/tables to be empty */
        init_data_list(&data_list);
        init_instruction_list(&instruction_list);
        label_head = NULL;

        /* add suffixes like .as and .am */
        file_am = add_suffix(preproc_file, ".am");
        file_as = add_suffix(preproc_file, ".as");
        /* check if we got the memory and file length's */
        if (!file_am || !file_as || strlen(file_am) >= MAX_FILENAME_LENGTH || strlen(file_as) >= MAX_FILENAME_LENGTH) {
            print_ext_error(ERROR_MEMORY_ALLOCATION, input_file, -1);
            cleanup_resources(&ICF, &DCF, label_head, &data_list, &instruction_list, &file_am, &file_as);
            continue;
        }

        /* Step 1: run the preprocessor */
        printf("\n------------------ Step 1: Preprocessing %s ------------------\n", file_as);
        if (preproc(file_as, file_am)) {
            printf("Step 1 Complete: Preprocessor failed for %s\n", file_as);
            did_fail = TRUE;
        } else {
            printf("Step 1 Complete: Preprocessor finished successfully for %s\n", file_as);

            /* Step 2: First Pass */
            printf("\n------------------ Step 2: First Pass %s ------------------\n", file_am);
            if (first_pass(file_am, &data_list, &instruction_list, &label_head, ICF, DCF)) {
                printf("Step 2 Complete: First pass finished with errors for %s\n", file_am);
                did_fail = TRUE;
            } else {
                printf("Step 2 Complete: First pass done successfully for %s\n", file_am);
            }

            /* Step 3: Second Pass */
            printf("\n------------------ Step 3: Second Pass %s ------------------\n", file_am);
            if (second_pass(&instruction_list, &data_list, label_head, file_am)) {
                printf("Step 3 Complete: Second pass finished with errors for %s\n", file_am);
                did_fail = TRUE;
            } else {
                printf("Step 3 Complete: Second pass done successfully for %s\n", file_am);
            }
        }

        /* if all good, move on to building the output files */
        if (!did_fail)
        {
            printf("\n------------------ Step 4: Building Output Files %s ------------------\n", input_file);
            if (!did_fail && build_output_files(input_file, &data_list, label_head, *ICF, *DCF)) {
                printf("Step 4 Complete: Failed to create output files for %s\n", input_file);
            } else {
                total_success++;
                printf("Step 4 Complete: Output files created successfully for %s\n", input_file);
            }
        }

        /* wrap it up */
        if (did_fail)
            printf("\n=============== Failed to process file: %s ===============\n", file_as);
        else
            printf("\n=============== File: %s Processed Successfully ===============\n", file_as);

        cleanup_resources(&ICF, &DCF, label_head, &data_list, &instruction_list, &file_am, &file_as);
        did_fail = FALSE; /* reset flag for next file */
    }

    /* all done */
    printf("\n==================================================================\n");
    printf("               %d of %d files processed successfully.        \n", total_success, argc - 1);
    printf("==================================================================\n");

    return SUCCESS;
}

void cleanup_resources(int **ICF, int **DCF, label_table *label_head, DataList *data_list,
                       InstructionList *instruction_list, char **file_am, char **file_as)
{
    /* free everything we allocated in assembler.c */

    if (*ICF) {
        free(*ICF);
        *ICF = NULL;
    }
    if (*DCF) {
        free(*DCF);
        *DCF = NULL;
    }
    if (label_head) free_label_list(label_head);
    if (data_list) free_data_list(data_list);
    if (instruction_list) free_instruction_list(instruction_list);

    if (*file_am) {
        free(*file_am);
        *file_am = NULL;
    }
    if (*file_as) {
        free(*file_as);
        *file_as = NULL;
    }
}
