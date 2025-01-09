#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#define TRUE 1
#define FALSE 0

#define MAX_LABEL_SIZE 31
#define MAX_LINE_LENGTH 80

#define word_length 24

#define MAX_MEMORY_SIZE 2097151 /* 2^21 - 1 */

typedef struct DataNode {
    int address;       /* Memory address */
    char data[7];      /* 6-bit hex representation as a string (+1 for '\0') */
    struct DataNode *next; /* Pointer to the next node */
} DataNode;

typedef struct {
    DataNode *head; /* Pointer to the first node */
} DataList;
typedef struct {
    char *name;       /* Instruction name (e.g., ADD, MOV) */
    int opcode;       /* Bits 23-18: Operation code */
    int funct;        /* Bits 7-3: Sub-function code */
    int src_mode;     /* Bits 17-16: Source addressing mode */
    int src_reg;      /* Bits 15-13: Source register */
    int dest_mode;    /* Bits 12-11: Destination addressing mode */
    int dest_reg;     /* Bits 10-8: Destination register */
    int are;          /* Bits 2-0: A, R, E bits */
    char src_label[MAX_LINE_LENGTH]; /* Label name for source operand */
    char dest_label[MAX_LINE_LENGTH];
} Instruction;

typedef struct InstructionNode {
    Instruction instruction;       /* The instruction */
    struct InstructionNode *next; /* Pointer to the next instruction in the list */
} InstructionNode;

typedef struct {
    InstructionNode *head; /* Head of the instruction list */
    InstructionNode *tail; /* Tail of the instruction list for easy append */
} InstructionList;

#endif //ASSEMBLER_H
