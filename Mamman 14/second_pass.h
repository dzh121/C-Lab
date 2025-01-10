#ifndef SECOND_PASS_H
#define SECOND_PASS_H

#include <stdio.h>
#include "assembler.h"
#include "tables.h"
#include "errors.h"
#include "funcs.h"
#include <string.h>
int second_pass(InstructionList *instruction_list, DataList *data_list, label_table *label_head, char *file_name);

#endif
