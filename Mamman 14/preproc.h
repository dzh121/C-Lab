#ifndef PREPROC_H
#define PREPROC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "data_structure.h"
#include "assembler.h"
#include "funcs.h"

void removeSpaces(char *p);
int preproc(char *file_name);
#endif
