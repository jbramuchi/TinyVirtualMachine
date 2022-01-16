#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lex.h"
#include "parser.h"
#include "codegen.h"
#include "vm.h"

int main(int argc, char **argv)
{
	int i;
	char** command;
	if (argc < 2)
	{
		printf("error : please include the file name");
		return 0;
	}
	else if (argc == 3)
	{
		command = malloc(1*sizeof(char));
	}
	else if (argc == 4)
	{
		command = malloc(2*sizeof(char*));
	}
	else if (argc == 5)
	{
		command = malloc(3*sizeof(char*));
	}
	
	/*
	might use this later, probably not
	FILE *ifp = fopen(argv[1], "r");
	char *inputfile = malloc(500 * sizeof(char));
	char c = fgetc(ifp);
	int i = 0;
	while (1)
	{
		inputfile[i++] = c;
		c = fgetc(ifp);
		if (c == EOF)
			break;
	}
	inputfile[i] = '\0';
	
	printf("%s\n", inputfile);
	*/
	char *inputfile = argv[1];
	FILE *test = fopen(inputfile, "r");
	if(test == NULL)
	{
		printf("Invalid file name.\n");
		return 0;
	}
	fclose(test);
	lexeme *temp_list = calloc(500, sizeof(lexeme));
	symbol *temp_table = calloc(500, sizeof(symbol));
	instruction *temp_code = calloc(500, sizeof(instruction));
	
	if (argc == 2)
	{
		lexeme *list = lex_analyze(temp_list, inputfile);
		symbol *table = parse(list, temp_table);
		instruction *code = generate_code(table, list, temp_code);
		virtual_machine(code, 0);
	}
	else
	{
		lexeme *list = lex_analyze(temp_list, inputfile);
		for (i = 0; i < argc-2; i++)
		{
			if (strcmp(argv[i+2], "-l") == 0)
			{
				print_lex(list);
			}
		}
		symbol *table = parse(list, temp_table);
		instruction *code = generate_code(table, list, temp_code);
		for (i = 0; i < argc-2; i++)
		{
			if (strcmp(argv[i+2], "-a") == 0)
			{
				print_assembly(code);
			}
		}	
		for (i = 0; i < argc-2; i++)
		{
			if(strcmp(argv[i+2], "-v") == 0)
			{
				virtual_machine(code, 1);
				free(list);
				return 0;
			}
		}
		virtual_machine(code, 0);
	}

	free(temp_list);
	free(temp_table);
	free(temp_code);
	
	return 0;
}