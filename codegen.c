#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lex.h"
#include "parser.h"
#include "codegen.h"

int list_index = 0;
int code_index = 0;
int table_index = 0;
int num_procedures = 0;

void emit(int op, int r, int l, int m, instruction *code);
void block(symbol *table, lexeme *list, instruction *code, int lex_level);
void statement(symbol *table, lexeme *list, instruction *code, int lex_level);
void condition(symbol *table, lexeme *list, instruction *code, int lex_level);
void expression(int endRegister, symbol *table, lexeme *list, instruction *code, int lex_level);
void term(int endRegister, symbol *table, lexeme *list, instruction *code, int lex_level);
void factor(int endRegister, symbol *table, lexeme *list, instruction *code, int lex_level);
void print_assembly(instruction *code);

instruction* generate_code(symbol *table, lexeme *list, instruction *code)
{
	printf("ENTERING CODEGEN\n");
	int i = 0;
	int j = 0;
	while (table[i].kind > 0 && table[i].kind < 5)
	{
		//printf("Table[%d].kind = %d\n", i, table[i].kind);
		if (table[i].kind == 3)
		{
			table[i].val = i;
			//i++;
			emit(7, 0, 0, 0, code);
		}
		i++;
	}
	printf("calling block from program\n");
	block(table, list, code, 0);
	i = 0;
	j = code_index;
	//table[0].addr = num_procedures;
	
	while (code[i].opcode > 0 && code[i].opcode < 30)
	{
		if (code[i].opcode == 7)
		{
			while (j >= 0)
			{
				if (code[j].opcode == 6)
				{
					printf("fixing procedure address to %d\n", j); // come back to later
					//printf("i is %d and j is %d\n", i, j);
					code[i].m = j;
					j--;
					break;
				}
				j--;
			}
		}
		i++;
	}
	
	i = 0;
	j = 0;
	// not sure about any of this
	while (code[i].opcode > 0 && code[i].opcode < 30)
	{
		if (code[i].opcode == 5)
		{
			j = 0;
			while (table[j].kind > 0 && table[j].kind < 4)
			{
				if (table[j].kind == 3)
				{
					if (table[j].val == code[i].m)
					{
						code[i].m = table[j].addr;
						break;
					}
				}
				j++;
			}
		}
		i++;
	}
	emit(9, 0, 0, 3, code);
	return code;
}
void emit(int op, int r, int l, int m, instruction *code)
{
	printf("emitting %d %d %d %d\n", op, r, l, m);
	code[code_index].opcode = op;
	code[code_index].r = r;
	code[code_index].l = l;
	code[code_index].m = m;
	code_index++;
}
void block(symbol *table, lexeme *list, instruction *code, int lex_level)
{
	int numVars = 0;
	int numSymbols = 0;
	int saved_table_index = -1;
	printf("entering block\n");
	int i = 0;
	int j = 0;
	int temp;

	if (list[list_index].type == 28) // const
	{
		// CHECK ALL THIS SHIT
		do
		{
			list_index++;
			numSymbols++;
			table[table_index].mark = 0;
			table_index++;
			/*
			table[table_index].mark = 0;
			table_index++;
			*/
			list_index += 3;
		}
		while (list[list_index].type == 17);
		list_index++;
	}
	//printf("table index: %d\n", table_index);
	if (list[list_index].type == 29) // var
	{
		do
		{
			table[table_index].mark = 0;
			table_index++;
			//list_index++;
			printf("incrementing numVars because of %s\n", table[table_index].name);
			numVars++;
			numSymbols++;
			temp = 0;
			list_index+=2;
			/*
			table[table_index].mark = 0;
			table_index++;
			*/
		}
		while (list[list_index].type == 17);
		list_index++;
		//numVars++;
	}
	if (list[list_index].type == 30) // procedure
	{
		do
		{
			table[table_index].mark = 0;
			table_index++;
			saved_table_index = temp;
			numSymbols++;
			list_index += 3;
			printf("calling block from block\n");
			block(table, list, code, lex_level+1);
			emit(2, 0, 0, 0, code);
			list_index++;
			num_procedures++;
		}
		while (list[list_index].type == 30);
		printf("saved table index is: %d\n", saved_table_index);		
	}
	printf("numVars: %d\n", numVars);
	table[saved_table_index].addr = code_index;
	emit(6, 0, 0, 3 + numVars, code);
	printf("calling statement from block\n");
	statement(table, list, code, lex_level);
	// marking the last numSymbols number of unmarked symbols
	// whatever that means
	// probably not doing it right
	while (i < numSymbols)
	{
		if (table[j].mark == 1)
		{
			table[j].mark = 0;
			i++;
			j++;
		}
		else if (table[j].mark == 0)
		{
			j++;
		}
		else
			break;
	}	
}
void statement(symbol *table, lexeme *list, instruction *code, int lex_level)
{
	printf("entering statement\n");
	int index, index_two;
	int temp;
	//printf("list type at list_index: %d\n", list[list_index].type);
	printf("list index: %d\n", list_index);
	if (list[list_index].type == 2) // ident
	{
		printf("calling expression from statement because of %s\n", list[list_index].name);
		//index = list_index;
		list_index += 2;
		// save the symbol table index of the var entry unmarked and closest in lexlevel
		// whatever that means
		expression(0, table, list, code, lex_level);
		printf("table index: %d, list index: %d, lex level: %d\n", table_index, list_index, lex_level);
		// idk about the lex_level minus part
		emit(4, 0, table[table_index].level, table[table_index].addr, code); // sto
	}
	if (list[list_index].type == 27) // call
	{
		list_index++;
		// save symbol table index???
		emit(5, 0, lex_level - table[table_index].level, table[table_index].addr, code);
		list_index++;
		/*
		token + 1
		save the symbol table index of the procedure entry unmarked and closest in lexlevel
		emit CAL (5, 0, L = lexlevel - L from the symbol table, M = value from the symbol table entry)
		token + 1
		*/
	}
	if (list[list_index].type == 21)
	{
		list_index++;
		statement(table, list, code, lex_level);
		while (list[list_index].type == 18)
		{
			list_index++;
			statement(table, list, code, lex_level);
		}
		list_index++;
	}
	if (list[list_index].type == 23) // ifsym
	{
		list_index++;
		condition(table, list, code, lex_level);
		index = list_index;
		emit(8, 0, 0, 0, code);
		list_index++;
		statement(table, list, code, lex_level);
		if (list[list_index].type == 33) // elsesym
		{
			list_index++;
			index_two = list_index; // saving current code index?
			emit (7, 0, 0, 0, code);
			code[index].m = list_index;
			statement(table, list, code, lex_level);
			code[index_two].m = list_index;
		}
		else
		{
			// fix the jpc from earlier, savedCodeIndexForJPC.M = currentcodeindex
			code[index].m = list_index;
		}
	}
	if (list[list_index].type == 25) // whilesym
	{
		list_index++;
		index = list_index;
		condition(table, list, code, lex_level);
		list_index++;
		index_two = list_index;
		emit(8, 0, 0, 0, code);
		statement(table, list, code, lex_level);
		emit (7, 0, 0, index, code);
		code[index_two].m = list_index;
	}
	if (list[list_index].type == 31) // write
	{
		list_index++;
		expression(0, table, list, code, lex_level); // not sure about the 0
		emit(9, 0, 0, 1, code); 
	}
	if (list[list_index].type == 32) // read
	{
		list_index++;
		index = list_index;
		list_index++;
		emit (9, 0, 0, 2, code);
		emit (4, 0, lex_level - table[table_index].level, table[list_index].addr, code);
	}
	
}
void condition(symbol *table, lexeme *list, instruction *code, int lex_level)
{
	printf("entering condition\n");
	if (list[list_index].type == 8)
	{
		list_index++;
		expression(0, table, list, code, lex_level);
		emit(15, 0, 0, 0, code);
	}
	else
	{
		if (list[list_index].type == 9)
		{
			list_index++;
			expression(1, table, list, code, lex_level);
			emit(17, 0, 0, 1, code);
		}
		if (list[list_index].type == 10)
		{
			list_index++;
			expression(1, table, list, code, lex_level);
			emit(18, 0, 0, 1, code);
		}
		if (list[list_index].type == 11)
		{
			list_index++;
			expression(1, table, list, code, lex_level);
			emit(19, 0, 0, 1, code);
		}
		if (list[list_index].type == 12)
		{
			list_index++;
			expression(1, table, list, code, lex_level);
			emit(20, 0, 0, 1, code);
		}
		if (list[list_index].type == 13)
		{
			list_index++;
			expression(1, table, list, code, lex_level);
			emit(21, 0, 0, 1, code);
		}
		if (list[list_index].type == 14)
		{
			list_index++;
			expression(1, table, list, code, lex_level);
			emit(22, 0, 0, 1, code);
		}
	}
}
void expression(int endRegister, symbol *table, lexeme *list, instruction *code, int lex_level)
{
	printf("entering expression, list.type = %d\n", list[list_index].type);
	if (list[list_index].type == 4)
	{
		list_index++;
	}
	if (list[list_index].type == 5)
	{
		list_index++;
		term(endRegister, table, list, code, lex_level);
		emit(10, endRegister, 0, 0, code);
		while (list[list_index].type == (4 || 5))
		{
			if (list[list_index].type == 4)
			{
				list_index++;
				term(endRegister, table, list, code, lex_level);
				emit(11, endRegister, endRegister, endRegister+1, code); // ADD
			}
			if (list[list_index].type == 5)
			{
				list_index++;
				term(endRegister, table, list, code, lex_level);
				emit(12, endRegister, endRegister, endRegister+1, code);
			}
		}
		return;
	}
	printf("calling term from expression\n");
	term(endRegister, table, list, code, lex_level);
	//emit(11, endRegister, endRegister, endRegister+1, code); // ADD
	while (list[list_index].type == (4 || 5))
	{
		if (list[list_index].type == 4)
		{
			list_index++;
			term(endRegister, table, list, code, lex_level);
			emit(11, endRegister, endRegister, endRegister+1, code); // ADD
		}
		if (list[list_index].type == 5)
		{
			list_index++;
			term(endRegister, table, list, code, lex_level);
			emit(11, endRegister, endRegister, endRegister+1, code);
		}
	}
}
void term(int endRegister, symbol *table, lexeme *list, instruction *code, int lex_level)
{
	printf("entering term\n");
	factor(endRegister, table, list, code, lex_level);
	while (list[list_index].type == (6 || 7))
	{
		if (list[list_index].type == 6)
		{
			list_index++;
			factor(endRegister, table, list, code, lex_level);
			emit(13, endRegister, endRegister, endRegister+1, code);
		}
		if (list[list_index].type == 7)
		{
			list_index++;
			term(endRegister, table, list, code, lex_level);
			emit(14, endRegister, endRegister, endRegister+1, code);
		}
	}
}
void factor(int endRegister, symbol *table, lexeme *list, instruction *code, int lex_level)
{
	printf("entering factor\n");
	int index;
	if (list[list_index].type == 2)
	{
		// not sure about this
		index = list_index;
		if (table[table_index].kind == 1)
		{
			emit(1, endRegister, 0, table[list_index].val, code);
		}
		if (table[table_index].kind == 2)
		{
			printf("m value for %s is %d\n", table[table_index].name, table[table_index].addr);
			emit(3, endRegister, lex_level - table[table_index].level, table[table_index].addr, code); //LOD
		}
		list_index++;
	}
	else if (list[list_index].type == 3)
	{
		emit(1, endRegister, 0, list[list_index].value, code);
		list_index++;
	}
	else
	{
		list_index++;
		expression(endRegister, table, list, code, lex_level);
		list_index++;
	}
}
void print_assembly(instruction *code)
{
	int i = 0;
	char* op_codes[23] = {"lit", "rtn", "lod", "sto", "cal", "inc", "jmp", "jpc", "sys", "neg", "add",
						  "sub", "mul", "div", "odd", "mod", "eql", "neq", "lss", "leq", "gtr", "geq"};
	
	//printf("CODE: %d\n", code[200].opcode);
	printf("Line	OP    R  L  M\n");
	while (code[i].opcode != 0)
	{
		printf("%2d      %s   %d  %d  %d\n", i, op_codes[code[i].opcode-1], code[i].r, code[i].l, code[i].m);
		i++;
	}
}