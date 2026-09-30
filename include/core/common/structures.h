#ifndef M8_STRUCTURES_H
#define M8_STRUCTURES_H
#define MAX_INS_SIZE 16
#define MAX_DATA_SIZE 8
#define MAX_TOKEN_SIZE 16
#include <stdint.h>
#include "operations.h"
#include "assembler.h"
typedef enum TokenType {
    INSTRUCTION,
    REGISTER, // Registers (AR, BR, CR, RR, L7...)
    LABEL, // label: code...
    DIRECTIVE, // db, dw, equ, .bss, .data, .rodata...
    NUMBER, // A Number
    NEWLINE, // Can be ; or enter.
    TEOF, // End of file
    NONE,
    END
} TokenType;

typedef struct {
    TokenType type;
    char literal[MAX_TOKEN_SIZE];
} Token;
typedef struct ParsedIns ParsedIns;
typedef void (*Dir)(Assembler*, ParsedIns*);

typedef struct Directive {
    Dir fn;
    uint8_t code;
} Directive;

typedef enum ParseType {
    NON,
    DIRECT,
    INSTRUCT
} ParseType;

typedef struct {
    TokenType type;
    uint8_t value;
} Operand;

typedef struct ParsedIns {
    Operation op;
    Operand ope1;
    Operand ope2;
    ParseType ptype;
    Directive directive;
} ParsedIns;

typedef enum ParsingError {
    OKAY,
    UNKNOWN_INSTRUCTION,
    UNKNOWN_DIRECTIVE,
    UNKNOWN_REGISTER,
    SYNTAX_ERROR,
    INVALID_OPCODE,
    INVALID_ARGUMENT,
    INVALID_NUMBER,
    MISSING_INSTRUCTION,
    MISSING_ARGUMENT,
    MISSING_LINE,
    OUT_OF_BOUNDS,
    TOO_MANY_OPERANDS
} ParsingError;

typedef struct {
    uint8_t* data;
    uint8_t size;
} Encoded;
#endif