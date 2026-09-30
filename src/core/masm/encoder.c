#include "encoder.h"

Encoded encoder(ParsedIns ins) {
    Encoded result;
    result.data = malloc(sizeof(uint8_t));
    if (ins.ptype == DIRECT) {
        result.data[0] = ins.directive.code;
    } else {
        result.data[0] = ins.op.code;
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

void appendToEncodedBuffer(Encoded* enc, uint8_t newData, bool *err) {
    uint8_t* temp = realloc(enc->data, enc->size++);
    *err = false;
    if (temp == NULL) {
        *err = true;
        free(temp);
        return;
    }
    
    enc->data = temp;
    enc->data[enc->size] = newData;
}