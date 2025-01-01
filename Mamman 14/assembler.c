#include "assembler.h"
#include <stdlib.h>
#include "preproc.h"
#include "errors.h"
#include "first_pass.h"

int main(int argc, char *argv[])
{
    DataList data_list; /* Use an actual DataList structure */
    init_data_list(&data_list); /* Initialize the data list */

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
    int i = preproc("test");
    if(i != 1)
    {
        print_ext_error(ERROR_PREPROC_FAILED, "test", -1);
        exit(1);
    }
    first_pass("test.am", &data_list);


    return 0;
}