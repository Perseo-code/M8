#ifndef ASSEMBLER_H
#define ASSEMBLER_H
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
typedef enum Section {
    DATA,
    BSS,
    TEXT,
    NONE
} Section;

typedef struct Assembler {
    uint8_t* output;
    size_t output_size;
    size_t position;
    Section sect;
} Assembler;
inline Assembler* init_assembler() {
    Assembler* result = malloc(sizeof(Assembler));
    result->output = malloc(sizeof(uint8_t));
    result->output_size = 1;
    result->position = 0;
    result->sect = NONE;
    return result;
}
#endif