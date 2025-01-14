#include "assembler.h"
#include <stdlib.h>
#include <stdio.h>
#include "preproc.h"
#include "errors.h"
#include "first_pass.h"
#include "second_pass.h"
#include "funcs.h"

int main(int argc, char *argv[]) {
    int file_index;
    char *input_file;
    char preproc_file[MAX_FILENAME_LENGTH];
    int *ICF;
    int *DCF;
    DataList data_list;
    InstructionList instruction_list;
    label_table *label_head;
    char *file_am;
    char *file_as;

    if (argc < 2) {
        fprintf(stderr, "ERROR: No input files provided\n");
        return 1;
    }

    printf("Starting assembler...\n");
    printf("Total files to process: %d\n", argc - 1);

    for (file_index = 1; file_index < argc; file_index++) {
        input_file = argv[file_index];
        strncpy(preproc_file, input_file, sizeof(preproc_file) - 1);
        preproc_file[sizeof(preproc_file) - 1] = '\0';

        printf("\n===================================================\n");
        printf("Processing file %d of %d: %s\n", file_index, argc - 1, input_file);
        printf("===================================================\n");

        ICF = (int *)handle_malloc(sizeof(int));
        DCF = (int *)handle_malloc(sizeof(int));

        init_data_list(&data_list);
        init_instruction_list(&instruction_list);
        label_head = NULL;

        file_am = add_suffix(preproc_file, ".am");
        file_as = add_suffix(preproc_file, ".as");

        printf("Step 1: Starting preprocessor for %s\n", file_as);
        if (!preproc(input_file)) {
            print_ext_error(ERROR_PREPROC_FAILED, file_as, -1);
            cleanup_resources(ICF, DCF, label_head, &data_list, &instruction_list);
            continue;
        }
        printf("Step 1 Complete: Preprocessor finished for %s\n", file_as);

        printf("Step 2: Starting first pass for %s\n", file_am);
        if (!first_pass(file_am, &data_list, &instruction_list, &label_head, ICF, DCF)) {
            print_ext_error(ERROR_FIRST_PASS_FAILED, file_am, -1);
            cleanup_resources(ICF, DCF, label_head, &data_list, &instruction_list);
            continue;
        }
        printf("Step 2 Complete: First pass done for %s\n", file_am);

        printf("Step 3: Starting second pass for %s\n", file_am);
        if (!second_pass(&instruction_list, &data_list, label_head, file_am)) {
            print_ext_error(ERROR_SECOND_PASS_FAILED, file_am, -1);
            cleanup_resources(ICF, DCF, label_head, &data_list, &instruction_list);
            continue;
        }
        printf("Step 3 Complete: Second pass done for %s\n", file_am);

        printf("Step 4: Building output files for %s\n", input_file);
        if (!build_output_files(input_file, &data_list, label_head, *ICF, *DCF)) {
            print_ext_error(ERROR_BUILD_OUTPUT_FAILED, file_am, -1);
            cleanup_resources(ICF, DCF, label_head, &data_list, &instruction_list);
            continue;
        }
        printf("Step 4 Complete: Output files created for %s\n", input_file);

        cleanup_resources(ICF, DCF, label_head, &data_list, &instruction_list);
        printf("File processing complete: %s\n", input_file);
    }

    printf("\n===================================================\n");
    printf("All files processed.\n");
    printf("===================================================\n");
    return 0;
}
void cleanup_resources(int *ICF, int *DCF, label_table *label_head, DataList *data_list, InstructionList *instruction_list) {
    if (ICF)
        free(ICF);
    if (DCF)
        free(DCF);
    if (label_head)
        free_label_list(label_head);
    if (data_list)
        free_data_list(data_list);
    if (instruction_list)
        free_instruction_list(instruction_list);
}
