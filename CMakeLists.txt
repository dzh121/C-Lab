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

add_executable(testing_main "testing/main.c")
