#ifndef LEXER
#define LEXER
#include "registers.h"
#include "directives.h"
#include "includes.h"
#include "structures.h"

#define STREQ(a, b) (strcmp((a), (b)) == 0)



extern Token list[2048];
void lexer(const char*);
extern Token* current_token; // If needed in the parser or another file
extern uint32_t token_count;
#endif