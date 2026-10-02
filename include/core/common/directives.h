#ifndef DIRECTIVES_H
#define DIRECTIVES_H
#include "structures.h"
#include "assembler.h"
#define DIR_SIZE 6
#define _BSS 0xAB
#define _DATA 0xAC
#define _TEXT 0xAD
#define _BYTE 0xAE
#define _EQU 0xAF
#define _ORG 0xB0

typedef struct DirName {
    const char* name;
    uint8_t code;
} DirName;

extern Directive directives[];
extern DirName dirnames[];

void handleDirective(ParsedIns, Assembler*, Encoded*);
#endif