#include "assembler.h"
#include "tables.h"
#include <stdlib.h>
#include "preproc.h"
#include "errors.h"
#include "first_pass.h"
#include "second_pass.h"

int main(int argc, char *argv[])
{
    DataList data_list; /* Use an actual DataList structure */
    init_data_list(&data_list); /* Initialize the data list */
    int *ICF = (int *)handle_malloc(sizeof(int));
    int *DCF = (int *)handle_malloc(sizeof(int));

    /*while (argc > 1)
    {
        if (preproc(argv[argc - 1]) == 1)
        {

            printf("Success\n");
        }
        else
        {

            exit(1);
        }
        argc--;
    }*/
    printf("Starting preproc\n");
    int i = preproc("test");
    if(i != 1)
    {
        print_ext_error(ERROR_PREPROC_FAILED, "test", -1);
        exit(1);
    }
    printf("Preproc done\n");
    printf("Starting first pass\n");
    InstructionList instruction_list;
    init_instruction_list(&instruction_list);
    label_table *label_head = NULL;

    first_pass("test.am", &data_list, &instruction_list, &label_head, ICF, DCF);
    printf("First pass done\n");
    print_data_list(&data_list);

    printf("Starting second pass\n");
    second_pass(&instruction_list, &data_list, label_head, "test.am");
    printf("Second pass done\n");
    printf("Building output files\n");
    build_output_files("test", &data_list, label_head, *ICF, *DCF);
    print_label_list(label_head);
    print_instruction_list(&instruction_list);
    print_data_list(&data_list);
    return 0;
}
