#include "lexer.h"
#include "operations.h"
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>

Token list[2048];



Token* current_token;
uint32_t token_count;

bool is_number(const char* data) {
    int len = strlen(data);
    if (len == 0) return false; // Empty
    int i = 0;
    bool hex = false;
    if (data[0] == '0' && (data[1] == 'x' && len >= 2)) {
        i = 2;
        hex = true;
    }
    for (; i < len; i++) {
        if (!((data[i] <= '9' && data[i] >= '0') || ((data[i] <= 'a' && data[i] >= 'f') && hex))) {
            return false; // Not a number / hexadecimal number
        }
    }
    return true;
}

bool is_label_definition(const char* data) {
    int len = strlen(data);
    if (len == 0) return false;
    int i = 0;
    if (data[len - 1] == ':')
        return true;
    return false;
}

TokenType identify(const char* data) {
    for (int i = 0; i < OPNAME_SIZE; i++) {
        if (STREQ(data, opname[i].name)) {
            return INSTRUCTION;
        }
    }

    //printf("REND = %d\n", REND);

    /*for (int i = 0; i < REND; i++) {
        printf("regss[%d] = %p \"%s\"\n", i, (void *)regss[i], regss[i]);
    }*/
    for (int i = 0; i < REND; i++) {
        if (STREQ(data, regss[i])) {
            return REGISTER;
        }
    }

    for (int i = 0; i < DIR_SIZE; i++) {
        if (STREQ(data, dirnames[i].name)) {
            return DIRECTIVE;
        }
    }
    if (is_number(data)) return NUMBER;
    if (is_label_definition(data)) return LABEL;
    if (strlen(data) != 0) return IDENTIFIER;
    return NONE;
}

void lexer(const char* code) {
    current_token = &list[0];
    Token result;
    int i = 0;
    int j = 0;
    token_count = 0;
    uint32_t len = strlen(code);
    bool building_token = false;
    while (i < len) { // Get everything in the array
        if (code[i] == ' ' || code[i] == ',' || code[i] == 9) {
            if (building_token) {
                result.literal[j] = '\0';
                result.type = identify(result.literal);
                token_count++;
                *current_token = result;
                current_token++;
                j = 0;
                building_token = false;
            }
        } else if (code[i] == '\n') {
            
            Token made_up = {
                .literal = "\n",
                .type = NEWLINE
            };
            if (building_token) {
                result.literal[j] = '\0';
                result.type = identify(result.literal);
                *current_token = result;
                current_token++;
                token_count++;
            }
            
            *current_token = made_up;
            current_token++;
            token_count++;
            j = 0;
            building_token = false;
        } else {
            building_token = true;
            result.literal[j] = tolower(code[i]);
            j++;
        }
        i++;
    }
    if (building_token) {
        result.literal[j] = '\0';
        result.type = identify(result.literal);
        *current_token = result;
        current_token++;
        token_count++;
    }

    Token made_up = {
        .literal = "",
        .type = TEOF
    };

    *current_token = made_up;
    current_token++;
    token_count++;
}