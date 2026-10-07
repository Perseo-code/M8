#include "encoder.h"

Encoded encoder(ParsedIns ins, Assembler* assm) {
    Encoded result;
    result.data = malloc(sizeof(uint8_t));
    if (ins.ptype == DIRECT) {
        handleDirective(ins, assm, &result);
        return result;
    } else {
        result.data[0] = ins.data.op->code;
    }
    result.size = 1;
    
    if (ins.ope1.type != NONE) {
        bool temp_err;
        appendToEncodedBuffer(&result, ins.ope1.value, &temp_err);
        if (temp_err) {
            free(result.data);
            return (Encoded){};
        }
    }

    if (ins.ope2.type != NONE) {
        bool temp_err;
        appendToEncodedBuffer(&result, ins.ope2.value, &temp_err);
        if (temp_err) {
            free(result.data);
            return (Encoded){};
        }
    }

    return result;
}

