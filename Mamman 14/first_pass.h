#ifndef FIRST_PASS_H
#define FIRST_PASS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "tables.h"
#include "assembler.h"
#include "funcs.h"
#include "errors.h"

#define INITIAL_IC 100
#define INITIAL_DC 0

int first_pass(char *file_name, DataList *data_list);

#endif
