cmake_minimum_required(VERSION 3.29)
project(C_Lab C)

set(CMAKE_C_STANDARD 90)


add_executable(mamman11_q1_main "Mamman 11/q1/letters.c"
)
add_executable(mamman11_q2_main "Mamman 11/q2/xy_bits.c")
add_executable(mamman12_main "Mamman 12/adjacency.c"
        "Mamman 12/header.h")
add_executable(mamman22_mycomplex "Mamman 22/complex.c"
        "Mamman 22/complex.h")
add_executable(mamman22_input "Mamman 22/input.c"
        "Mamman 22/input.h")
add_executable(mamman23_timediff "Mamman 23/timediff.c"
        "Mamman 23/timediff.h")
add_executable(mamman14_preproc
        "Mamman 14/src/preproc.c"
        "Mamman 14/headers/preproc.h"
        "Mamman 14/src/data_structure.c"
        "Mamman 14/headers/data_structure.h"
        "Mamman 14/headers/assembler.h"
        "Mamman 14/headers/funcs.h"
        "Mamman 14/src/funcs.c"
        "Mamman 14/src/errors.c"
        "Mamman 14/headers/errors.h"
)
add_executable(mamman14_assembler
        "Mamman 14/src/assembler.c"
        "Mamman 14/headers/assembler.h"
        "Mamman 14/src/tables.c"
        "Mamman 14/headers/tables.h"
        "Mamman 14/src/first_pass.c"
        "Mamman 14/headers/first_pass.h"
        "Mamman 14/src/funcs.c"
        "Mamman 14/headers/funcs.h"
        "Mamman 14/src/errors.c"
        "Mamman 14/headers/errors.h"
        "Mamman 14/src/preproc.c"
        "Mamman 14/headers/preproc.h"
        "Mamman 14/src/data_structure.c"
        "Mamman 14/headers/data_structure.h"
        "Mamman 14/src/code_translation.c"
        "Mamman 14/headers/code_translation.h"
        "Mamman 14/src/second_pass.c"
        "Mamman 14/headers/second_pass.h"
        "Mamman 14/headers/globals.h")
add_executable(mamman14_tables
        "Mamman 14/src/tables.c"
        "Mamman 14/headers/tables.h"
        "Mamman 14/src/assembler.c"
        "Mamman 14/headers/assembler.h"
        "Mamman 14/src/funcs.c"
        "Mamman 14/headers/funcs.h"
        "Mamman 14/src/errors.c"
        "Mamman 14/headers/errors.h"
        "Mamman 14/src/preproc.c"
        "Mamman 14/headers/preproc.h"
        "Mamman 14/src/data_structure.c"
        "Mamman 14/headers/data_structure.h"
        "Mamman 14/src/code_translation.c"
        "Mamman 14/headers/code_translation.h")
add_executable(mamman14_first_pass
        "Mamman 14/src/first_pass.c"
        "Mamman 14/headers/first_pass.h"
        "Mamman 14/src/tables.c"
        "Mamman 14/headers/tables.h"
        "Mamman 14/src/assembler.c"
        "Mamman 14/headers/assembler.h"
        "Mamman 14/src/funcs.c"
        "Mamman 14/headers/funcs.h"
        "Mamman 14/src/errors.c"
        "Mamman 14/headers/errors.h"
        "Mamman 14/src/preproc.c"
        "Mamman 14/headers/preproc.h"
        "Mamman 14/src/data_structure.c"
        "Mamman 14/headers/data_structure.h"
        "Mamman 14/src/code_translation.c"
        "Mamman 14/headers/code_translation.h")
add_executable(mamman14_code
        "Mamman 14/src/code_translation.c"
        "Mamman 14/headers/code_translation.h"
        "Mamman 14/src/first_pass.c"
        "Mamman 14/headers/first_pass.h"
        "Mamman 14/src/tables.c"
        "Mamman 14/headers/tables.h"
        "Mamman 14/src/assembler.c"
        "Mamman 14/headers/assembler.h"
        "Mamman 14/src/funcs.c"
        "Mamman 14/headers/funcs.h"
        "Mamman 14/src/errors.c"
        "Mamman 14/headers/errors.h"
        "Mamman 14/src/preproc.c"
        "Mamman 14/headers/preproc.h"
        "Mamman 14/src/data_structure.c"
        "Mamman 14/headers/data_structure.h")
add_executable(mamman14_data "Mamman 14/src/data_structure.c"
        "Mamman 14/headers/data_structure.h")
add_executable(testing_main "testing/main.c")
