#ifndef GLOBALS_H
#define GLOBALS_H

/* General Constants */
#define TRUE 1 /* Boolean true */
#define FALSE 0 /* Boolean false */

#define MAX_LABEL_SIZE 31 /* Maximum label length */
#define MAX_LINE_LENGTH 80 /* Maximum line length */
#define MAX_FILENAME_LENGTH 100 /* Maximum file name length */
#define MAX_MEMORY_SIZE 2097151 /* Maximum memory size (2^21 - 1) */

#define WORD_LENGTH 24 /* Word length in bits */

#define INITIAL_IC 100 /* Initial value for the instruction counter */
#define INITIAL_DC 0 /* Initial value for the data counter */

/**
 * @brief Status codes for function return values.
 *
 * SUCCESS (0) indicates a successful operation.
 * FAILURE (1) indicates an error occurred.
 */
typedef enum {
    SUCCESS = 0,
    FAILURE = 1
} Status;

/**
 * @brief Addressing modes for instructions.
 *
 * UNKNOWN (0) - Uninitialized mode or for if the mode doesn't matter.
 * IMMEDIATE (1) - Immediate value as an operand.
 * DIRECT (2) - Direct memory address access.
 * RELATIVE (3) - Relative addressing mode.
 * REGISTER_DIRECT (4) - Register direct access.
 * EXTERNAL (5) - External label referencing.
 */
typedef enum {
    UNKNOWN = 0,
    /* Represents an uninitialized mode or cases where the mode is irrelevant (stop instruction, PRN destination mode, ...) */
    IMMEDIATE = 1,
    DIRECT = 2,
    RELATIVE = 3,
    REGISTER_DIRECT = 4,
    EXTERNAL = 5
} AddressingMode;

/**
 * @brief Label types used in the assembler.
 *
 * CODE - Label points to an instruction.
 * DATA - Label points to a data section.
 * ENTRY - Label is marked for external reference in output.
 * EXTERN - Label is declared as external.
 */
typedef enum {
    CODE,
    DATA,
    ENTRY,
    EXTERN
} LabelType;


/**
 * @brief Node structure for storing memory data.
 *
 * Each node stores a memory address and its corresponding data value.
 */
typedef struct DataNode {
    int address; /* Memory address */
    int data; /* Data value */
    struct DataNode *next; /* Pointer to the next node */
} DataNode;

/**
 * @brief Data list structure for managing stored memory values.
 *
 * A linked list of `DataNode` elements representing the stored data.
 */
typedef struct {
    DataNode *head; /* Pointer to the first node */
} DataList;

/**
 * @brief Structure representing an instruction in the assembler.
 *
 * Contains the instruction name, opcode, function code, addressing modes,
 * operands, and additional details such as the source and destination labels.
 */
typedef struct {
    char *name; /* Instruction name (add, mov, etc.) */
    int opcode; /* Bits 23-18: Operation code */
    int funct; /* Bits 7-3: Sub-function code */
    int src_mode; /* Bits 17-16: Source addressing mode */
    int src_operand; /* Bits 15-13: Source register or operand */
    int dest_mode; /* Bits 12-11: Destination addressing mode */
    int dest_operand; /* Bits 10-8: Destination register or operand */
    int are; /* Bits 2-0: A, R, E bits (Absolute, Relocatable, External) */
    char src_label[MAX_LINE_LENGTH]; /* Label name for source operand */
    char dest_label[MAX_LINE_LENGTH]; /* Label name for destination operand */
    int line_number; /* Line number in the source file */
} Instruction;


/**
 * @brief Node structure for storing an instruction in a linked list.
 *
 * Each node stores an `Instruction` structure and a pointer to the next node.
 */
typedef struct InstructionNode {
    Instruction instruction; /* Instruction data */
    struct InstructionNode *next; /* Pointer to the next instruction in the list */
} InstructionNode;

/**
 * @brief Linked list structure for managing assembler instructions.
 *
 * Contains pointers to the head and tail of the instruction list
 * to allow efficient insertion at the end.
 */
typedef struct {
    InstructionNode *head; /* Head of the instruction list */
    InstructionNode *tail; /* Tail of the instruction list for easy append */
} InstructionList;

/**
 * @brief Node structure for storing address references.
 *
 * Used for tracking multiple occurrences of a label in the source code.
 */
typedef struct ADDRESS_LIST {
    int addr; /* address */
    struct ADDRESS_LIST *next; /* pointer to the next node */
} ADDRESS_LIST;


/**
 * @brief Structure representing a label in the assembler.
 *
 * Contains the label name, address, type, and a list of addresses where
 * the label is referenced (for external labels).
 */
typedef struct label_table {
    char name[MAX_LABEL_SIZE]; /* Label name */
    int addr; /* Address (0 if unresolved, resolved during second pass) */
    int line; /* Line number */
    LabelType type; /* Type of label (CODE, DATA, ENTRY, EXTERN) */
    ADDRESS_LIST *addr_list; /* List of all usage addresses for EXTERN */
    struct label_table *next; /* Pointer to the next node */
} label_table;


#endif /* GLOBALS_H */
