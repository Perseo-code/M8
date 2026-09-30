#ifndef MAIN_IASM
#define MAIN_IASM
#include <stdlib.h>
#include <stdio.h>
#include "structures.h"
#include "operations.h"
#include "registers.h"
#include "lexer.h"
#include "parser.h"
#include "encoder.h"
#include "cpu.h"

int compiler(char* buffer, FILE* output);
void emulator(const uint8_t* buffer, size_t size);
#endif