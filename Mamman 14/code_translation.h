#ifndef CODE_TRANSLATION_H
#define CODE_TRANSLATION_H

#include "assembler.h"

#define MAX_COMMAND_NAME 5

typedef struct {
    char name[MAX_COMMAND_NAME]; /* Command name (e.g., MOV, CMP) */
    unsigned short opcode;       /* The opcode representing the command */
    unsigned short funct;        /* The funct value for additional operation specification */
    int num_operands;            /* Number of operands (0, 1, or 2) */
    int source_addressing;       /* Addressing method for source operand */
    int dest_addressing;         /* Addressing method for destination operand */
    int IC;                      /* Instruction Counter value */
    char src_operand[MAX_LABEL_SIZE]; /* Source operand if applicable */
    char dest_operand[MAX_LABEL_SIZE]; /* Destination operand if applicable */
} COMMAND;

#endif
