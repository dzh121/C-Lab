#ifndef FIRST_PASS_H
#define FIRST_PASS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "tables.h"
#include "assembler.h"
#include "code_translation.h"
#include "funcs.h"
#include "errors.h"

#define INITIAL_IC 100
#define INITIAL_DC 0

int first_pass(char *file_name, DataList *data_list, InstructionList *instruction_list, label_table **label_head, int* ICF, int* DCF);
int handle_extern(char *line, label_table **label_head, char *file_name, int line_count);
int encode_data(char *line, int *DC, int IC, DataList *data_list, char *file_name, int line_count);
int encode_string(char *line, int *DC, int IC, DataList *data_list, char *file_name, int line_count);
int valid_label(char *label);
void init_data_list(DataList *list);
void add_data_node(DataList *list, int address, int data);
void print_label_list(label_table *label_head);

#endif
