#ifndef REGISTERS_H
#define REGISTERS_H
#include <stdint.h>
typedef enum Register {
    AR,
    BR,
    CR,
    RR,
    L1,
    L2,
    L3,
    L4,
    L5,
    L6,
    L7,
    REND
} Register;

extern char* regss[];

typedef struct {
    uint8_t r[REND];
} Registers;
#endif