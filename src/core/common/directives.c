#include "directives.h"
#include "lexer.h"
#define DEFDIR(b, fn) [b] = {fn, b}
#define DEFNAME(b, n) {n, b}
#define DEFEXP(b, e, e2) [b] = {e, e2}
void allocateNew(Assembler* assm) {
    uint8_t* temp = realloc(assm->output, sizeof(uint8_t) * assm->output_size);
    if (temp == NULL) {
        free(temp);
        return;
    }

    assm->output = temp;
}
void dataHeader(Assembler* assm, ParsedIns* p) {assm->sect = DATA;}
void bssHeader(Assembler* assm, ParsedIns* p) {assm->sect = BSS;}
void textHeader(Assembler* assm, ParsedIns* p) {assm->sect = TEXT;}
void defbyte(Assembler* assm, ParsedIns* p) {
    assm->output[assm->position++] = p->ope1.val.value;
    assm->output_size++;
}
void equ(Assembler* assm, ParsedIns* p) {
    assm->output[assm->position++] = p->ope1.val.value;
    assm->output_size++;
}
void org(Assembler* assm, ParsedIns* p) {
    if (assm->output + p->ope1.val.value == NULL) {
        uint8_t* temp = realloc(assm->output, sizeof(uint8_t) * p->ope1.val.value);
        if (temp == NULL) {
            free(temp);
            return;
        }

        assm->output = temp;
        assm->output_size = p->ope1.val.value;
        assm->position = p->ope1.val.value;
        allocateNew(assm);
    }
    assm->position = p->ope1.val.value;
}

void resb(Assembler* assm, ParsedIns* p) {
    int i = 0;
    for (; i < p->ope1.val.value; i++) {
        assm->output[assm->position + i] = 0;
    }
    assm->position += i;
    assm->output_size += i;
    allocateNew(assm);
}
Directive directives[] = {
    DEFDIR(_DATA, dataHeader),
    DEFDIR(_BSS, bssHeader),
    DEFDIR(_TEXT, textHeader),
    DEFDIR(_BYTE, defbyte),
    DEFDIR(_EQU, equ),
    DEFDIR(_ORG, org)
}; 

DirName dirnames[DIR_SIZE] = {
    DEFNAME(_DATA, ".data"),
    DEFNAME(_BSS, ".bss"),
    DEFNAME(_TEXT, ".text"),
    DEFNAME(_BYTE, "byte"),
    DEFNAME(_EQU, "equ"),
    DEFNAME(_ORG, "org")
};

Expects direxpects[] = {
    DEFEXP(_DATA, NOTHING, NOTHING),
    DEFEXP(_BSS, NOTHING, NOTHING),
    DEFEXP(_TEXT, NOTHING, NOTHING),
    DEFEXP(_BYTE, NUMBER, NOTHING),
    DEFEXP(_EQU, NUMBER, NOTHING), // later expect labels.
    DEFEXP(_ORG, NUMBER, NOTHING)
};

void handleDirective(ParsedIns ins, Assembler* assm, Encoded* encoded) {
    ins.data.directive->fn(assm, &ins);
    for (int i = 0; i < assm->output_size; i++) {
        bool err = false;
        appendToEncodedBuffer(encoded, assm->output[i], &err);
        if (err)
            return;
    }
}