#ifndef __LEX_H
#define __LEX_H
typedef struct lexeme
{
	char *name;
	int value;
	int type;
	char *symbol;
} lexeme;

lexeme* lex_analyze(lexeme *list, char *inputfile);
void print_lex(lexeme *list);

#endif