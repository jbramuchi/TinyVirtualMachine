// Annabel Bland and John Bramuchi

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lex.h"
#include "parser.h"

#define nulsym 1
#define identsym 2
#define numbersym 3
#define plussym 4
#define minussym 5
#define multsym 6
#define slashsym 7
#define oddsym 8
#define eqlsym 9
#define neqsym 10
#define lessym 11
#define leqsym 12
#define gtrsym 13
#define geqsym 14
#define lparentsym 15
#define rparentsym 16
#define commasym 17
#define semicolonsym 18
#define periodsym 19
#define becomessym 20
#define beginsym 21
#define endsym 22
#define ifsym 23
#define thensym 24
#define whilesym 25
#define dosym 26
#define callsym 27
#define constsym 28
#define varsym 29
#define procsym 30
#define writesym 31
#define readsym 32
#define elsesym 33

int tableIndex = 0;
int position = 0;

// only looks at one specific token
int isANumber(lexeme token)
{
    return token.value < 100000 && token.value > -100000;
}

int isARelation(lexeme token)
{
    if(token.type == 9 || token.type == 10 || token.type == 11 ||
       token.type == 12 || token.type == 13 || token.type == 14)
    {
        return 1;
    }
    else
    {
        return -1;
    }
}

// returns true if varName is in table and is unmarked AND is on the same lexLevel, starting backwards
int searchTableLEX(symbol* table, char* varName, int lexLevel)
{
    int itr = tableIndex;

    while(itr >= 0)
    {
        //printf("Table at index %d: name - %s, mark = %d, level = %d\n", itr, table[itr].name, table[itr].mark, table[itr].level);
        if(table[itr].mark == 0 && strcmp(table[itr].name, varName) == 0 && table[itr].level == lexLevel)
            return 1;

        itr -= 1;
    }

    return -1;
}

// returns true if varName is in table and is unmarked, starting backwards
int searchTable(symbol* table, char* varName)
{
    int itr = tableIndex;

    while(itr >= 0)
    {
        if(table[itr].mark == 0 && strcmp(table[itr].name, varName) == 0)
            return 1;

        itr -= 1;
    }

    return -1;
}

int check_block(lexeme* list, symbol* table, int lexLevel);

int check_expression(lexeme* list, symbol* table, int lexLevel);

int check_factor(lexeme* list, symbol* table, int lexLevel)
{
    //printf("Entering factor:\n");
    int temp;

    if(list[position].type == identsym)
    {
        if(searchTable(table, list[position].name) == -1)
        {
            printf("Error: identifier not in symbol table.\n");
            return -1;
        }

        position++;
    }
    else if(isANumber(list[position]))
    {
        position++;
    }
    else if(list[position].type == lparentsym) // lparentsym denotes "("
    {
        position++;

        temp = check_expression(list, table, lexLevel);

        if(temp == -1)
            return -1;

        if(list[position].type != rparentsym) // rparentsym = ")"
        {
            printf("Error: expected right parenthesis.\n");
            return -1;
        }

        position++;
    }
    else
    {
        printf("Error: incomplete term or expression.\n");
        return -1;
    }

    //printf("Leaving factor\n");
    return 1;
}

int check_term(lexeme* list, symbol* table, int lexLevel)
{
    //printf("Entering Term\n");
    int temp;
    
    temp = check_factor(list, table, lexLevel);
    
    if(temp == -1)
        return -1;

    while(list[position].type == multsym || list[position].type == slashsym)
    {
        position++;
        temp = check_factor(list, table, lexLevel);

        if(temp == -1)
            return -1;
    }

    //printf("Leaving term\n");
    return 1;
}

int check_expression(lexeme* list, symbol* table, int lexLevel)
{
    //printf("Entering expression:\n");
    int temp;
    
    if(list[position].type == plussym || list[position].type == minussym)
    {
        position++;
    }
    
    temp = check_term(list, table, lexLevel);

    if(temp == -1)
        return -1;

    while(list[position].type == plussym || list[position].type == minussym)
    {
        position++;
        
        temp = check_term(list, table, lexLevel);

        if(temp == -1)
            return -1;
    }

    //printf("Leaving expression\n");
    return 1;
}

int check_condition(lexeme* list, symbol* table, int lexLevel)
{
    //printf("Entering condition:\n");
    int temp;
    
    if(list[position].type == oddsym)
    {
        position++;

        temp = check_expression(list, table, lexLevel);

        if(temp == -1)
        return -1;
    }
    else
    {   
        temp = check_expression(list, table, lexLevel);

        if(temp == -1)
            return -1;

        if(!isARelation(list[position]))
        {
            printf("Error: expected relational operator.\n");
            return -1;
        }

        position++;

        temp = check_expression(list, table, lexLevel);

        if(temp == -1)
            return -1;
    }

    //printf("Leaving condition\n");
    return 1;
}

int check_statement(lexeme* list, symbol* table, int lexLevel)
{
    //printf("Entering statement:\n");
    int temp;
    
    if(list[position].type == identsym)
    {
        if(searchTable(table, list[position].name) == -1)
        {
            printf("Error: identifier not in symbol table.\n");
            return -1;
        }

        position++;

        if(list[position].type != becomessym) // ':=' 
        {
            printf("Error: expected 'becomes' token.\n");
            return -1;
        }

        position++;
        
        temp = check_expression(list, table, lexLevel);

        if(temp == -1)
            return -1;

    }
    else if(list[position].type == callsym)
    {
        position++;

        if(searchTable(table, list[position].name) == -1)
        {
            printf("Error: identifier not in symbol table.\n");
            return -1;
        }

        position++;
    }
    else if(list[position].type == beginsym)
    {
        position++;

        temp = check_statement(list, table, lexLevel);

        if(temp == -1)
            return -1;

        while(list[position].type == semicolonsym)
        {
            position++;
            temp =  check_statement(list, table, lexLevel);

            if(temp == -1)
                return -1;
        }
        
        if(list[position].type != endsym)
        {
            printf("Error: expected 'end' token.\n");
            return -1;
        }

        position++;
    }
    else if(list[position].type == ifsym)
    {
        position++;

        temp = check_condition(list, table, lexLevel);

        if(temp == -1)
            return -1;

        if(list[position].type != thensym)
        {
            printf("Error: expected 'then' token.\n");
            return -1;
        }

        position++;

        temp = check_statement(list, table, lexLevel);

        if(temp == -1)
            return -1;

        if(list[position].type == elsesym)
        {
            position++;

            temp = check_statement(list, table, lexLevel);

            if(temp == -1)
                return -1;
        }
    }
    else if(list[position].type == whilesym)
    {
        position++;

        temp = check_condition(list, table, lexLevel);

        if(temp == -1)
            return -1;

        if(list[position].type != dosym)
        {
            printf("Error: expected 'do' token.\n");
            return -1;
        }

        position++;

        temp = check_statement(list, table, lexLevel);

        if(temp == -1)
            return -1;
    }
    else if(list[position].type == readsym)
    {
        position++;
        
        if(searchTable(table, list[position].name) == -1)
        {
            printf("Error: identifier not in symbol table.\n");
            return -1;
        }

        position++;
    }
    else if(list[position].type == writesym)
    {
        position++;
        
        temp = check_expression(list, table, lexLevel);
    }

    //printf("Leaving statement\n");

    return 1;
}

int const_declaration(lexeme* list, symbol* table, int lexLevel)
{
    //printf("Entering Const declaration:\n");
    int numSymbols = 0;
    char* tempName;

    if(list[position].type == constsym)
    {
        do
        {
            position++;

            if(list[position].type != identsym)
            {
                printf("Error: expected identifier.\n");
                return -1;
            }

            tempName = list[position].name;
            
            if(searchTableLEX(table, list[position].name, lexLevel) == 1)
            {
                printf("Error: identifier already in symbol table.\n");
                return -1;
            }

            position++;

            if(list[position].type != eqlsym)
            {
                printf("Error: expected equals token.\n");
                return -1;
            }

            position++;
            
            if(!isANumber(list[position]))
            {
                printf("Error: expected number.\n");
                return -1;
            }
            //printf("\nAdding to table const\n\n");
            table[tableIndex].kind = 1;
            strcpy(table[tableIndex].name, tempName);
            table[tableIndex].val = list[position].value;
            table[tableIndex].level = lexLevel;
            table[tableIndex].addr = 0;
            table[tableIndex].mark = 0;
            tableIndex += 1;

            position++;
            numSymbols += 1;
        }
        while(list[position].type == commasym);

        if(list[position].type != semicolonsym)
        {
            printf("Error: expected semicolon.\n");
            return -1;
        }
        position++;
    }

    //printf("Leaving const declaration\n");
    return numSymbols;
}

int var_declaration(lexeme* list, symbol* table, int lexLevel)
{
    //printf("Entering var declartion:\n");
    int numVar = 0;

    if(list[position].type == varsym)
    {
        do
        {
            numVar++;

            position++;

            if(list[position].type != identsym)
            {
                printf("Error: expected identifier.\n");
                return -1;
            }

            if(searchTableLEX(table, list[position].name, lexLevel) == 1)
            {
                printf("Error: identifier already in symbol table.\n");
                return -1;
            }
            //printf("\nAdding to table var\n\n");
            table[tableIndex].kind = 2;
            strcpy(table[tableIndex].name, list[position].name);
            table[tableIndex].val = 0;
            table[tableIndex].level = lexLevel;
            table[tableIndex].addr = numVar + 2;
            table[tableIndex].mark = 0;
            tableIndex += 1;

            position++;

        }
        while(list[position].type == commasym);

        if(list[position].type != semicolonsym)
        {
                printf("Error: expected semicolon.\n");
                return -1;
        }

        position++;
    }

    //printf("Entering var declaration:\n");
    return numVar;
}

int procedure_declaration(lexeme* list, symbol* table, int lexLevel)
{
    //printf("Entering procedure declaration:\n");
    int numProcedures = 0;
    int temp;

    if(list[position].type == procsym)
    {
        do
        {
            position++;

            if(list[position].type != identsym)
            {
                printf("Error: expected identifier.\n");
                return -1;
            }

            if(searchTableLEX(table, list[position].name, lexLevel) == 1)
            {
                printf("Error: identifier already in symbol table.\n");
                return -1;
            }
            //printf("\nAdding to table proc\n\n");
            table[tableIndex].kind = 3;
            strcpy(table[tableIndex].name, list[position].name);
            table[tableIndex].val = 0;
            table[tableIndex].level = lexLevel;
            table[tableIndex].addr = 0;
            table[tableIndex].mark = 0;
            tableIndex += 1;

            position++;

            if(list[position].type != semicolonsym)
            {
                printf("Error: expected semicolon.\n");
                return -1;
            }

            position++;

            temp = check_block(list, table, lexLevel);

            if(temp == -1)
                return -1;

            if(list[position].type != semicolonsym)
            {
                printf("Error: expected semicolon.\n");
                return -1;
            }

            numProcedures += 1;
            position++;
        }
        while(list[position].type == procsym);
    }

    //printf("Leaving Proc declaration\n");
    return numProcedures;
}

int check_block(lexeme* list, symbol* table, int lexLevel)
{
    //printf("Entering block:\n");
    int temp;
    int itr;
    int numSymbols = 0;
    
    temp = const_declaration(list, table, lexLevel);

    if(temp == -1)
        return -1;

    numSymbols += temp;

    temp = var_declaration(list, table, lexLevel);

    if(temp == -1)
        return -1;

    numSymbols += temp;

    temp = procedure_declaration(list, table, lexLevel + 1);

    if(temp == -1)
        return -1;

    numSymbols += temp;

    temp = check_statement(list, table, lexLevel);

    if(temp == -1)
        return -1;

    // mark the last numsymbols number of unmarked symbols

    itr = tableIndex - 1;
    temp = numSymbols;
    //printf("NumSymbols: %d\n", numSymbols);
    while(temp > 0)
    {
        if(table[itr].mark == 0)
        {
            table[itr].mark = 1;
            temp--;
        }

        itr--;
    }

    //printf("Leaving Block\n");
    return 1;
}

int program(lexeme* list, symbol* table)
{
    //printf("Entering program:\n");

    int temp;
    int lexLevel = 0;

    //printf("\nAdding to table main\n\n");
    table[tableIndex].kind = 3;
    strcpy(table[tableIndex].name, "main");
    table[tableIndex].val = 0;
    table[tableIndex].level = 0;
    table[tableIndex].addr = 0;
    table[tableIndex].mark = 0;
    tableIndex += 1; 

    temp = check_block(list, table, lexLevel);
    
    if(temp == -1)
    {
        return -1;
    }

    if(list[position].type != periodsym)
    {
        printf("Error: period expected.");
        return -1;
    }

    //printf("Leaving Program\n");
    return 1;
}

symbol* parse(lexeme* lexemeList, symbol *table)
{
    // Every function has int return type
    // to allow errors to percolate up through 
    // function calls, except symbol declaration
    // functions.
    
    // Upon error free code, these functions 
    // should not have any calculated return value.
    int result = program(lexemeList, table);
    int i = 0;

    if(result == -1)
        return NULL;

    /*while(i < tableIndex)
    {
        printf("Table[%d]:\n # kind - %d\n # name - %s\n # value - %d\n # level - %d\n # address - %d\n # mark - %d\n", i, table[i].kind, table[i].name, table[i].val, table[i].level, table[i].addr, table[i].mark);
        i++;
    }*/
    return table;
}