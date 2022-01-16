#ifndef __PARSER_H
#define __PARSER_H

#include "lex.h"
#include "parser.h"

typedef struct symbol
{
    int kind; // const, var, proc
    char name[12];
    int val;
    int level;
    int addr;
    int mark;
} symbol;

symbol* parse(lexeme* lexemeList, symbol* table);

#endif