#include "directives.h"
#include "lexer.h"
#define DEFDIR(b, fn) [b] = {fn, b}
#define DEFNAME(b, n) {n, b}
#define DEFEXP(b, e, e2) [b] = {e, e2}
void dataHeader(Assembler* assm, ParsedIns* p) {assm->sect = DATA;}
void bssHeader(Assembler* assm, ParsedIns* p) {assm->sect = BSS;}
void textHeader(Assembler* assm, ParsedIns* p) {assm->sect = TEXT;}
void defbyte(Assembler* assm, ParsedIns* p) {
    assm->output[assm->position++] = 0;
}
void equ(Assembler* assm, ParsedIns* p) {
    assm->output[assm->position++] = p->ope1.value;
}
void org(Assembler* assm, ParsedIns* p) {
    assm->position = p->ope1.value;
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
    ins.directive.fn(assm, &ins);
    // TODO: finish this function so directives work
}