#ifndef M8_STRUCTURES_H
#define M8_STRUCTURES_H
#define MAX_INS_SIZE 16
#define MAX_DATA_SIZE 8
#define MAX_TOKEN_SIZE 16
#include <stdint.h>
#include <stdbool.h>
#include "operations.h"
#include "assembler.h"
typedef enum TokenType {
    INSTRUCTION,
    REGISTER, // Registers (AR, BR, CR, RR, L7...)
    IDENTIFIER, // For already created labels
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
    INSTRUCT,
    ID
} ParseType;

typedef struct Label {
    char* name;
    size_t name_size;
    uint16_t offset;
    uintptr_t index;
} Label;

typedef struct {
    TokenType type;
    union {
        uint8_t value;
        Label* label;
    } val;
} Operand;

typedef struct ParsedIns {
    Operand ope1;
    Operand ope2;
    ParseType ptype;
    union {
        Operation* op;
        Directive* directive;
        Label* label;
    } data;
} ParsedIns;

extern Label* all_labels;
extern size_t amount_labels;
typedef enum ParsingError {
    OKAY,
    UNKNOWN_INSTRUCTION,
    UNKNOWN_DIRECTIVE,
    UNKNOWN_REGISTER,
    UNKNOWN_IDENTIFIER,
    SYNTAX_ERROR,
    INVALID_OPCODE,
    INVALID_ARGUMENT,
    INVALID_NUMBER,
    MISSING_INSTRUCTION,
    MISSING_ARGUMENT,
    MISSING_LINE,
    OUT_OF_BOUNDS,
    TOO_MANY_OPERANDS,
    LABEL_HAS_NO_NAME,
    OUT_OF_MEMORY
} ParsingError;

typedef struct {
    uint8_t* data;
    uint8_t size;
} Encoded;

inline void appendToEncodedBuffer(Encoded* enc, uint8_t newData, bool *err) {
    uint8_t* temp = realloc(enc->data, enc->size++);
    *err = false;
    if (temp == NULL) {
        *err = true;
        free(temp);
        return;
    }
    
    enc->data = temp;
    enc->data[enc->size] = newData;
}
#endif 