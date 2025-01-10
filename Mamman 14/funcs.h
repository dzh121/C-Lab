#ifndef FUNCS_H
#define FUNCS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "assembler.h"
#include "errors.h"
#include "tables.h"

void *handle_malloc(long size);
int is_reserved (char *line);
void add_data_node(DataList *list, int address, int value);
void print_data_list(DataList *list);
void free_data_list(DataList *list);
void init_data_list(DataList *list);
int isValidLabel(char *label);
int build_output_files(char* file_name, DataList* data_list, label_table* label_head, int ICF, int DCF);
#endif
