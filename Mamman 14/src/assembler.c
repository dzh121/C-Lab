#include "../headers/assembler.h"

int main(int argc, char* argv[])
{
	int file_index; /* Index of the current file being processed */
	char* input_file; /* Name of the input file */
	char preproc_file[MAX_FILENAME_LENGTH]; /* Preprocessed file name */
	int *ICF, *DCF; /* Final instruction and data counters */
	DataList data_list; /* List of data directives */
	InstructionList instruction_list; /* List of instructions */
	label_table* label_head; /* Head of the label table */
	char *file_am = NULL, *file_as = NULL; /* File paths for .am and .as files */
	int total_success = 0; /* Total number of files processed successfully */

	/* Check for input files */
	if (argc < 2)
	{
		fprintf(stderr, "ERROR: No input files provided\n");
		return FAILURE;
	}

	printf("\n======================= STARTING ASSEMBLER =======================\n");
	printf("Total files to process: %d\n", argc - 1);
	printf("=================================================================\n");

	/* Process each input file */
	for (file_index = 1; file_index < argc; file_index++)
	{
		/* Get the input file name */
		input_file = argv[file_index];
		/* Copy the input file name to the preprocessor file name */
		strncpy(preproc_file, input_file, sizeof(preproc_file) - 1);
		/* Add a null terminator to the preprocessor file name */
		preproc_file[sizeof(preproc_file) - 1] = '\0';

		printf("\n=================================================================\n");
		printf("                 Processing File %d of %d: %-20s                 \n",
		       file_index, argc - 1, input_file);
		printf("=================================================================\n");

		/* Allocate memory for the instruction and data counters */
		ICF = (int*)handle_malloc(sizeof(int));
		DCF = (int*)handle_malloc(sizeof(int));

		/* Initialize data structures */
		init_data_list(&data_list);
		init_instruction_list(&instruction_list);
		label_head = NULL;

		/* Generate file paths for .am and .as files */
		file_am = add_suffix(preproc_file, ".am");
		file_as = add_suffix(preproc_file, ".as");
		if (!file_am || !file_as)
		{
			print_ext_error(ERROR_MEMORY_ALLOCATION, input_file, -1);
			cleanup_resources(ICF, DCF, label_head, &data_list, &instruction_list, file_am, file_as);
			continue;
		}

		/* Step 1: Preprocessing */
		printf("\n------------------ Step 1: Preprocessing %s ------------------\n", file_as);
		if (preproc(file_as, file_am))
		{
			print_ext_error(ERROR_PREPROC_FAILED, file_as, -1);
			cleanup_resources(ICF, DCF, label_head, &data_list, &instruction_list, file_am, file_as);
			continue;
		}
		printf("Step 1 Complete: Preprocessor finished successfully for %s\n", file_as);

		/* Step 2: First Pass */
		printf("\n------------------ Step 2: First Pass %s ------------------\n", file_am);
		if (first_pass(file_am, &data_list, &instruction_list, &label_head, ICF, DCF))
		{
			print_ext_error(ERROR_FIRST_PASS_FAILED, file_am, -1);
			cleanup_resources(ICF, DCF, label_head, &data_list, &instruction_list, file_am, file_as);
			continue;
		}
		printf("Step 2 Complete: First pass done successfully for %s\n", file_am);

		/* Step 3: Second Pass */
		printf("\n------------------ Step 3: Second Pass %s ------------------\n", file_am);
		if (second_pass(&instruction_list, &data_list, label_head, file_am))
		{
			print_ext_error(ERROR_SECOND_PASS_FAILED, file_am, -1);
			cleanup_resources(ICF, DCF, label_head, &data_list, &instruction_list, file_am, file_as);
			continue;
		}
		printf("Step 3 Complete: Second pass done successfully for %s\n", file_am);

		/* Step 4: Build Output Files */
		printf("\n------------------ Step 4: Building Output Files %s ------------------\n", input_file);
		if (build_output_files(input_file, &data_list, label_head, *ICF, *DCF))
		{
			print_ext_error(ERROR_BUILD_OUTPUT_FAILED, file_am, -1);
			cleanup_resources(ICF, DCF, label_head, &data_list, &instruction_list, file_am, file_as);
			continue;
		}
		printf("Step 4 Complete: Output files created successfully for %s\n", input_file);

		/* Increment the total success count */
		total_success += 1;

		/* Cleanup resources */
		cleanup_resources(ICF, DCF, label_head, &data_list, &instruction_list, file_am, file_as);
		printf("=============== File: %s Processing Complete ===============\n", input_file);
	}

	/* Final summary */
	printf("\n=================================================================\n");
	printf("        %d of %d files processed successfully.        \n", total_success, argc - 1);
	printf("=================================================================\n");

	return SUCCESS;
}

void cleanup_resources(int* ICF, int* DCF, label_table* label_head, DataList* data_list,
                       InstructionList* instruction_list, char* file_am, char* file_as)
{
	/* Free memory and reset pointers */
	if (ICF)
	{
		free(ICF);
		ICF = NULL;
	}
	if (DCF)
	{
		free(DCF);
		DCF = NULL;
	}
	if (label_head)
		free_label_list(label_head);
	if (data_list)
		free_data_list(data_list);
	if (instruction_list)
		free_instruction_list(instruction_list);
	if (file_am)
	{
		free(file_am);
		file_am = NULL;
	}
	if (file_as)
	{
		free(file_as);
		file_as = NULL;
	}
}
