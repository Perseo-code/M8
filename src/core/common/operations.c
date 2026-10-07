#include "operations.h"
#include <stdio.h>
#define CREATEOP(fn, byte, size) [byte] = {false, true, byte, size, fn} 

void nop(CPU* cpu, GivenCommands cmd) {}
void mov(CPU* cpu, GivenCommands cmd) {
    uint8_t value = cmd.operand2;
    if (cmd.mem_types == LBL) return;
    cpu->regs.r[cmd.operand1.uint_operand] = value;
}

void add(CPU* cpu, GivenCommands cmd) {
    if (cmd.mem_types == LBL) return;
    uint8_t destination = cmd.operand1.uint_operand;
    uint8_t source = cmd.operand2;
    cpu->regs.r[destination] += cpu->regs.r[source];
}

void sub(CPU* cpu, GivenCommands cmd) {
    if (cmd.mem_types == LBL) return;
    uint8_t reg1 = cmd.operand1.uint_operand;
    uint8_t reg2 = cmd.operand2;
    cpu->regs.r[AR] = cpu->regs.r[reg1] - cpu->regs.r[reg2];
}

void jmp(CPU* cpu, GivenCommands cmd) {
    if (cmd.mem_types == LBL) {
        cpu->pc = cmd.operand1.label_operand->address;
    } else {
        cpu->pc = cmd.operand1.uint_operand;
    }
}

void showreg(CPU* cpu, GivenCommands cmd) {
    if (cmd.mem_types == LBL) return;
    uint8_t reg = cpu->regs.r[cmd.operand1.uint_operand];
    printf("%d \n", reg);
}

Operation OPS[INS_SET_SIZE] = { 
    CREATEOP(nop, _NOP, 1),
    CREATEOP(mov, _MOV, 3),
    CREATEOP(add, _ADD, 3),
    CREATEOP(sub, _SUB, 3),
    CREATEOP(jmp, _JMP, 2),
    CREATEOP(showreg, _SHOWREG, 2)
};

OpName opname[OPNAME_SIZE] = {
    {"nop", _NOP},
    {"mov", _MOV},
    {"add", _ADD},
    {"sub", _SUB},
    {"jmp", _JMP},
    {"sreg", _SHOWREG}
};

Expects ins_expect[INS_SET_SIZE] = {
    [_NOP] = {NOTHING, NOTHING},
    [_MOV] = {REG, NUM},
    [_ADD] = {REG, REG},
    [_SUB] = {REG, REG},
    [_JMP] = {NUM, NOTHING},
    [_SHOWREG] = {REG, NOTHING}
};