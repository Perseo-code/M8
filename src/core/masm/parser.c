#include "parser.h"
#include <stdlib.h>
#include <stdio.h>

uint64_t parser_position = 0;
Label* all_labels;
size_t amount_labels = 1;
ParsedIns parse(ParsingError* error) {
    ParsedIns result = {
        .ope1.type = NONE,
        .ope2.type = NONE,
        .ptype = NON
    };
    //printf("NEW PARSE: ptype=%d\n", result.ptype);
    // We look at the list the lexer has built by itself, and go ahead by compiling with the encoder (my fav part honestly)
    for (; parser_position < token_count; parser_position++) {
        switch (list[parser_position].type) {
            case INSTRUCTION:
                if (result.ptype != NON) {
                    //printf("TOO MANY: instruction ptype was %d\n", result.ptype);
                    *error = TOO_MANY_OPERANDS;
                    return (ParsedIns){};
                }
                result.data.op = malloc(sizeof(Operation));
                result.ptype = INSTRUCT;
                OpName op;
                //printf("INSTRUCTION: %s, ptype=%d\n",
                //    list[parser_position].literal,
                //    result.ptype);
                bool broken = true;
                for (int j = 0; j < INS_SET_SIZE; j++) {
                    if (STREQ(list[parser_position].literal, opname[j].name)) {
                        broken = false;
                        op = opname[j];
                        break;
                    }
                }
                if (broken) {
                    *error = UNKNOWN_INSTRUCTION;
                    return (ParsedIns){};
                } // No such instruction...
                *result.data.op = OPS[op.inst];
                break;
            case REGISTER:
                //printf("REGISTER: %s, ope1=%d, ope2=%d\n",
                //    list[parser_position].literal,
                //    result.ope1.type,
                //    result.ope2.type);
                if (result.ptype != INSTRUCT) {
                    *error = INVALID_ARGUMENT;
                    return (ParsedIns){};
                }
                uint16_t reg;
                bool br = true;
                for (int j = 0; j < REND; j++) {
                    if (STREQ(list[parser_position].literal, regss[j])) {
                        reg = j;  
                        br = false;
                        break;
                    }
                }
                if (br) {
                    *error = UNKNOWN_REGISTER;
                    return (ParsedIns){};
                }
                if (result.ope1.type == NONE) result.ope1 = (Operand){REGISTER, reg};
                else if (result.ope2.type == NONE) result.ope2 = (Operand){REGISTER, reg};
                else {*error = TOO_MANY_OPERANDS; return (ParsedIns){};}
                break;
            case DIRECTIVE:
                if (result.ptype != NON) {
                    *error = TOO_MANY_OPERANDS;
                    return (ParsedIns){};
                }
                result.data.directive = malloc(sizeof(Directive));
                result.ptype = DIRECT;
                bool b = true;
                Directive d;
                for (int j = 0; j < DIR_SIZE; j++) {
                    if (STREQ(list[parser_position].literal, dirnames[j].name)) {
                        d = directives[dirnames[j].code];
                        b = false;
                        break;
                    }
                }
                if (b) {
                    *error = UNKNOWN_DIRECTIVE;
                    return (ParsedIns){};
                }
                
                result.data.directive = &d;
                break;
            case LABEL:
                if (result.ptype != NON) {
                    *error = TOO_MANY_OPERANDS;
                    return (ParsedIns){};
                }

                result.data.label = malloc(sizeof(Label));
                result.ptype = LAB;
                strcpy(result.data.label->name, list[parser_position].literal);
                result.data.label->name_size = strlen(result.data.label->name);
                if (result.data.label->name_size == 0) {
                    *error = LABEL_HAS_NO_NAME;
                    return (ParsedIns){};
                }
                Label* temp = realloc(all_labels, amount_labels);
                if (temp == NULL) {
                    *error = OUT_OF_MEMORY;
                    free(temp);
                    return (ParsedIns){};
                }
                all_labels = temp;
                amount_labels++;
                all_labels->index = amount_labels;
                break;
            case IDENTIFIER:
                if (result.ptype != NON) {
                    *error = TOO_MANY_OPERANDS;
                    return (ParsedIns){};
                }

                for (size_t i = 0; i < amount_labels; i++) {
                    if (STREQ(list[parser_position].literal, all_labels[i].name)) {
                        result.ptype = LAB;
                        *result.data.label = all_labels[i];
                        return result;
                    }
                }
                
                *error = UNKNOWN_IDENTIFIER;
                return (ParsedIns){};

            case NUMBER:
                if (result.ptype != INSTRUCT) {
                    *error = INVALID_ARGUMENT; 
                    return (ParsedIns){};
                }
                char* endptr;
                uint64_t num = strtoul(list[parser_position].literal, &endptr, 0);
                if (num >= 256) {
                    *error = OUT_OF_BOUNDS;
                    return (ParsedIns){};
                }

                if (*endptr != '\0') {
                    *error = INVALID_NUMBER;
                    return (ParsedIns){};
                }
                if (result.ope1.type == NONE) result.ope1 = (Operand){NUMBER, num};
                else if (result.ope2.type == NONE) result.ope2 = (Operand){NUMBER, num};
                else {*error = TOO_MANY_OPERANDS; return (ParsedIns){};}
                break;
            case NEWLINE: // Ressembles an OR (||) operator
            case TEOF:
                if (result.ptype == NON) {
                    parser_position++;
                    *error = OKAY;
                    return (ParsedIns){};
                }

                // validate and return the instruction
                switch (result.ptype) {
                    case DIRECT:
                        break;
                    case INSTRUCT:
                        if (result.data.op->empty) {*error = MISSING_INSTRUCTION; return (ParsedIns){};}
                        Args args1 = NOTHING;
                        Args args2 = NOTHING;
                        switch (result.ope1.type) {
                            case REGISTER:
                                args1 = REG;
                                break;
                            case NUMBER:
                                args1 = NUM;
                                break;
                        }
                        if (!(ins_expect[result.data.op->code].arg1 == args1)) {
                            *error = INVALID_ARGUMENT;
                            return (ParsedIns){};
                        }

                        switch (result.ope2.type) {
                            case REGISTER:
                                args2 = REG;
                                break;
                            case NUMBER:
                                args2 = NUM;
                                break;
                        }

                        if (!(ins_expect[result.data.op->code].arg2 == args2)) {
                            *error = INVALID_ARGUMENT;
                            return (ParsedIns){};
                        }
                        break;
                }
                *error = OKAY;
                parser_position++;
                return result;
        }
    }

    *error = OKAY;
    return result; // It's almost imposible for the code to end up here
}