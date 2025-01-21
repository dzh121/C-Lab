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

/* Function Prototypes */
int main(int argc, char *argv[]);
void cleanup_resources(int *ICF, int *DCF, label_table *label_head, DataList *data_list, InstructionList *instruction_list, char *file_am, char *file_as);
#endif /* ASSEMBLER_H */
