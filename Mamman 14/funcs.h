#ifndef FUNCS_H
#define FUNCS_H

#include <stdio.h>
#include <stdlib.h>
#include "assembler.h"
void *handle_malloc(long size);
int is_reserved (char *line);
void add_data_node(DataList *list, int address, int value);
void print_data_list(DataList *list);
void free_data_list(DataList *list);
void init_data_list(DataList *list);
#endif
