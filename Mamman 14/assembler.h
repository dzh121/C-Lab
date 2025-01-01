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

#endif //ASSEMBLER_H
