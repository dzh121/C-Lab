#ifndef GLOBALS_H
#define GLOBALS_H

/* General Constants */
#define TRUE 1
#define FALSE 0

#define MAX_LABEL_SIZE 31 /* Maximum label length */
#define MAX_LINE_LENGTH 80 /* Maximum line length */
#define MAX_FILENAME_LENGTH 100 /* Maximum file name length */
#define MAX_MEMORY_SIZE 2097151 /* 2^21 - 1 */

/* Instruction Word Structure */
#define WORD_LENGTH 24 /* Word length in bits */

/* Status Codes */
typedef enum
{
	SUCCESS = 0,
	FAILURE = 1
} Status;

/* Addressing Modes */
typedef enum
{
	UNKNOWN = 0,
	IMMEDIATE = 1,
	DIRECT = 2,
	RELATIVE = 3,
	REGISTER_DIRECT = 4,
	EXTERNAL = 5
} AddressingMode;

/* Label Types */
typedef enum
{
	CODE,
	DATA,
	ENTRY,
	EXTERN
} LabelType;

/* Data List for memory data storage */
typedef struct DataNode
{
	int address; /* Memory address */
	int data; /* Data value */
	struct DataNode* next; /* Pointer to the next node */
} DataNode;

typedef struct
{
	DataNode* head; /* Pointer to the first node */
} DataList;

/* Instruction Node and List */
typedef struct
{
	char* name; /* Instruction name (e.g., ADD, MOV) */
	int opcode; /* Bits 23-18: Operation code */
	int funct; /* Bits 7-3: Sub-function code */
	int src_mode; /* Bits 17-16: Source addressing mode */
	int src_operand; /* Bits 15-13: Source register or operand */
	int dest_mode; /* Bits 12-11: Destination addressing mode */
	int dest_operand; /* Bits 10-8: Destination register or operand */
	int are; /* Bits 2-0: A, R, E bits */
	char src_label[MAX_LINE_LENGTH]; /* Label name for source operand */
	char dest_label[MAX_LINE_LENGTH];
	int line_number; /* Line number in the source file */
} Instruction;

typedef struct InstructionNode
{
	Instruction instruction; /* The instruction */
	struct InstructionNode* next; /* Pointer to the next instruction in the list */
} InstructionNode;

typedef struct
{
	InstructionNode* head; /* Head of the instruction list */
	InstructionNode* tail; /* Tail of the instruction list for easy append */
} InstructionList;

typedef struct ADDRESS_LIST{
	int addr; /* address */
	struct ADDRESS_LIST *next; /* pointer to the next node */
} ADDRESS_LIST;

/* Label Table for storing defined labels */
typedef struct label_table {
	char name[MAX_LABEL_SIZE]; /* Label name */
	int addr; /* Address (0 if unresolved, resolved during second pass) */
	int line; /* Line number */
	LabelType type; /* Type of label (CODE, DATA, ENTRY, EXTERN) */
	ADDRESS_LIST *addr_list; /* List of all usage addresses for EXTERN */
	struct label_table *next; /* Pointer to the next node */
} label_table;


#endif /* GLOBALS_H */
