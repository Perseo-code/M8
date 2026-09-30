#ifndef ASSEMBLER_H
#define ASSEMBLER_H
#include <stdint.h>
#include <stddef.h>

typedef enum Section {
    DATA,
    BSS,
    TEXT
} Section;

typedef struct Assembler {
    uint8_t* output;
    size_t output_size;
    size_t position;
    Section sect;
} Assembler;
#endif