#ifndef SECOND_PASS_H
#define SECOND_PASS_H

#include "globals.h"
#include "tables.h"
#include "errors.h"
#include "funcs.h"
#include "code_translation.h"

int second_pass(InstructionList *instruction_list, DataList *data_list, label_table *label_head, char *file_name);

#endif /* SECOND_PASS_H */
