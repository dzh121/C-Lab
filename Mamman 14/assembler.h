#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include "globals.h"
#include <stdlib.h>
#include <stdio.h>
#include "preproc.h"
#include "errors.h"
#include "first_pass.h"
#include "second_pass.h"
#include "funcs.h"

/**
 * @brief Assembles the input assembly file/s.
 *
 * This function is the main entry point for the assembler program. It processes
 * the command line arguments files, preprocesses the input assembly file, performs the
 * first and second passes, and generates the output files(.ob, .ent, .ext).
 *
 * @param argc The number of command line arguments.
 * @param argv An array of command line argument strings.
 * @return int Returns 1 on success, or 0 on failure.
 */
int main(int argc, char *argv[]);

/**
 * @brief Frees allocated resources used during assembly.
 *
 * This function cleans up dynamically allocated memory and other resources
 * used during the assembly process, such as the instruction list, data list,
 * label table, and temporary files.
 *
 * @param ICF Pointer to the final instruction counter.
 * @param DCF Pointer to the final data counter.
 * @param label_head Pointer to the head of the label table.
 * @param data_list Pointer to the list of data directives.
 * @param instruction_list Pointer to the list of instructions.
 * @param file_am Pointer to the name of the preprocessed file (.am).
 * @param file_as Pointer to the name of the input assembly file (.as).
 */
void cleanup_resources(int *ICF, int *DCF, label_table *label_head, DataList *data_list, InstructionList *instruction_list, char *file_am, char *file_as);

#endif /* ASSEMBLER_H */
