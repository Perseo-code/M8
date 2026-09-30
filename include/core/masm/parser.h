#ifndef PARSER
#define PARSER
#include "operations.h"
#include "registers.h"
#include "structures.h"
#include "directives.h"
#include "lexer.h"
ParsedIns parse(ParsingError*); 

extern uint64_t parser_position;
#endif