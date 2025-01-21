#ifndef PREPROC_H
#define PREPROC_H

#include "globals.h"
#include "funcs.h"
#include "errors.h"
#include "data_structure.h"

int preproc(char *file_as, char* file_am);
int isValidName(char *name, int line, char *file_name);
char *remove_white_spaces(char *line);

#endif /* PREPROC_H */
