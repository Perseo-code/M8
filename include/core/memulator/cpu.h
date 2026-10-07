#ifndef CPU_H
#define CPU_H
#include "operations.h"
#include "structures.h"
#define MEMORY_SIZE 4096
#include "registers.h"
typedef struct CPU {
    Registers regs;
    uint16_t pc; // Program Counter
    uint8_t memory[MEMORY_SIZE];
} CPU;


typedef struct GivenCommands {
    uint8_t operand1;
    uint8_t operand2;
} GivenCommands;

void execute(CPU* cpu);
#endif