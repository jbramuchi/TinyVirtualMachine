#include "lex.h"
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

int charsAreNum(char* str)
{
  int i=0;
  while(str[i] != '\0')
  {
    if(!(isdigit(str[i])))
      return 0;

    i++;
  }
  // returns 1 if all characters are numbers
  return 1;
}
/*
void main()
{
	lexeme *list = malloc(500*sizeof(lexeme));
	list = lex_analyze(list, "input.txt");
	free(list);
}
*/
lexeme* lex_analyze(lexeme *list, char *inputfile)
{
	int objects = 0, i = 0, length = 0, j = 0;
	char** tokenArray;
	char c;
	char* s = malloc(52 * sizeof(char));
	int index = 0;
	FILE *input = fopen(inputfile, "r");
	int slash_flag = 0;
	int block_flag = 0;
	int leq_flag = 0;
	int geq_flag = 0;
	int s_flag = 0;
	int colon_flag = 0;
	//lexeme *list = malloc(500 * sizeof(lexeme));
	if(input == NULL)
	{
		return NULL;
	}

  // special char combinations: ':=' '>=' '<=' '<>'
    printf("\nFILE INPUT:\n");
	while(fscanf(input, "%c", &c) != EOF)
	{
		// these if statements makes sure we ignore all of the text
		// after a // until we hit a new line character
		printf("%c", c);
		//printf("Objects: %d\n", objects);
		//printf("type: %d\n", list[objects].type);
		if (slash_flag == 2)
		{
			if (c == '\n')
			{
				slash_flag = 0;
				continue;
			}
			continue;
		}
		// these if statements make sure we ignore all of the text
		// after a /* until we hit a */
		if (block_flag == 2)
		{
			if (c == '*')
			{
				fscanf(input, "%c", &c);
				if (c == '/')
				{
					block_flag = 0;
					continue;
				}
			}
			continue;
		}
		if ((slash_flag == 1 && c != '/') && (block_flag == 1 && c != '*'))
		{
			list[objects].type = 7;
			list[objects].symbol = "slashsym";
			objects++;
			slash_flag = 0;
			block_flag = 0;
		}
		if((isspace(c) || !(isalnum(c))) && s_flag == 1)
		{
			s[index] = '\0';
			//printf("String: %s\n", s);
			if (charsAreNum(s) == 1)
			{
				if (index >= 5)
				{
					// ERROR TOO BIG
				}
				list[objects].type = 3;
				list[objects].symbol = "numsym";
				sscanf(s, "%d", &list[objects].value);
			}
			else
			{
				if (strcmp(s, "nul") == 0)
				{
					list[objects].type = 1;
					list[objects].symbol = "nulsym";
				}
				else if (strcmp(s, "odd") == 0)
				{
					list[objects].type = 8;
					list[objects].symbol = "oddsym";
				}
				else if (strcmp(s, "become") == 0)
				{
					list[objects].type = 20;
					list[objects].symbol = "becomesym";
				}
				else if (strcmp(s, "begin") == 0)
				{
					list[objects].type = 21;
					list[objects].symbol = "beginsym";
				}
				else if (strcmp(s, "end") == 0)
				{
					list[objects].type = 22;
					list[objects].symbol = "endsym";
				}
				else if (strcmp(s, "if") == 0)
				{
					list[objects].type = 23;
					list[objects].symbol = "ifsym";
				}
				else if (strcmp(s, "then") == 0)
				{
					list[objects].type = 24;
					list[objects].symbol = "thensym";
				}
				else if (strcmp(s, "while") == 0)
				{
					list[objects].type = 25;
					list[objects].symbol = "whilesym";
				}
				else if (strcmp(s, "do") == 0)
				{
					list[objects].type = 26;
					list[objects].symbol = "dosym";
				}
				else if (strcmp(s, "call") == 0)
				{
					list[objects].type = 27;
					list[objects].symbol = "callsym";
				}
				else if (strcmp(s, "const") == 0)
				{
					list[objects].type = 28;
					list[objects].symbol = "constsym";
				}
				else if (strcmp(s, "var") == 0)
				{
					list[objects].type = 29;
					list[objects].symbol = "varsym";
				}
				else if (strcmp(s, "procedure") == 0)
				{
					list[objects].type = 30;
					list[objects].symbol = "procsym";
				}
				else if (strcmp(s, "write") == 0)
				{
					list[objects].type = 31;
					list[objects].symbol = "writesym";
				}
				else if (strcmp(s, "read") == 0)
				{
					list[objects].type = 32;
					list[objects].symbol = "readsym";
				}
				else if (strcmp(s, "else") == 0)
				{
					list[objects].type = 33;
					list[objects].symbol = "elsesym";
				}
				else if (index >= 11)
				{
					// ERROR TOO LONG
				}
				else
				{
					list[objects].type = 2;
					list[objects].symbol = "identsym";
					list[objects].name = malloc(13*sizeof(char));
					strcpy(list[objects].name, s);
					//list[objects].name = s;
				}
			}
			objects++;
			free(s);
			s = malloc(500*sizeof(char));
			s_flag = 0;
			index = 0;
			if (isspace(c))
				continue;
		}
		if (leq_flag == 1)
		{
			if (c == '=')
			{
				list[objects].type = 10;
				list[objects].symbol = "neqsym";
				objects++;
				leq_flag = 0;
				continue;
			}
			else if (c == '>')
			{
				list[objects].type = 12;
				list[objects].symbol = "leqsym";
				objects++;
				leq_flag = 0;
				continue;
			}
			else
			{
				list[objects].type = 11;
				list[objects].symbol = "lessym";
				objects++;
				leq_flag = 0;
			}
		}
		if (geq_flag == 1)
		{
			if (c == '=')
			{
				list[objects].type = 14;
				list[objects].symbol = "geqsym";
				objects++;
				geq_flag = 0;
				continue;
			}
			else
			{
				list[objects].type = 13;
				list[objects].symbol = "gtrsym";
				objects++;
				geq_flag = 0;
			}
		}
		if (colon_flag == 1)
		{
			if (c == '=')
			{
				list[objects].type = 20;
				list[objects].symbol = "becomessym";
				objects++;
				colon_flag = 0;
				continue;
			}
			//else
				//continue;
				//ERROR
		}
		else if (c == '/')
		{
			block_flag++;
			slash_flag++;
			continue;
		}
		else if (c == '*')
		{
			// if * is part of a /* unit, don't add it
			if (block_flag == 1)
			{
				block_flag++;
				continue;
			}
			list[objects].type = 6;
			list[objects].symbol = "multsym";
			objects++;
			continue;
		}
		else if (c == '<')
		{
			leq_flag = 1;
			continue;
		}
		else if (c == '>')
		{
			geq_flag = 1;
			continue;
		}
		else if (c == '+')
		{
			list[objects].type = 4;
			list[objects].symbol = "plussym";
			objects++;
			continue;
		}
		else if (c == '-')
		{
			list[objects].type = 5;
			list[objects].symbol = "minussym";
			objects++;
			continue;
		}
		else if (c == '=')
		{
			list[objects].type = 9;
			list[objects].symbol = "eqsym";
			objects++;
			continue;
		}
		else if (c == '(')
		{
			list[objects].type = 15;
			list[objects].symbol = "lparentsym";
			objects++;
			continue;
		}
		else if (c == ')')
		{
			list[objects].type = 16;
			list[objects].symbol = "rparentsym";
			objects++;
			continue;
		}
		else if (c == ',')
		{
			list[objects].type = 17;
			list[objects].symbol = "commasym";
			objects++;
			continue;
		}
		else if (c == ';')
		{
			list[objects].type = 18;
			list[objects].symbol = "semicolonsym";
			objects++;
			continue;
		}
		else if (c == '.')
		{
			list[objects].type = 19;
			list[objects].symbol = "periodsym";
			objects++;
			continue;
		}
		else if (c == ':')
		{
			colon_flag = 1;
		}
		else if(!isalnum(c) && !isspace(c))
		{
			// ERROR WEIRDASS CHARACTER DETECTED
			continue;
		}
		// at this point c must be a number or letter
		else if (isalpha(c) || isdigit(c))
		{
			s_flag = 1;
			s[index] = c;
			index++;
		}		
	}

	fclose(input);
	free(s);

	printf("\n"); 
	//printf("\nlex is analyzed\n");
	return list;
}
void print_lex(lexeme *list)
{
	int i = 0;
    printf("TOKENS:\n");
	while(list[i].type > 0 && list[i].type < 35)
	{
        printf("%d ", list[i].type);
        if(list[i].type == 2)
        {
            printf("%s ", list[i].name);
        }
        else if(list[i].type == 3)
        {
            printf("%d ", list[i].value);
        }
		/*printf("Type: %d\n", list[i].type);
		printf("Symbol: %s\n", list[i].symbol);
		if (list[i].type == 2)
			printf("Name: %s\n", list[i].name);
		else if (list[i].type == 3)
			printf("Value: %d\n", list[i].value);*/
		i++;
	}

    printf("\n\nSYMBOLS:\n");
    i = 0;

    while(list[i].type > 0 && list[i].type < 35)
	{
        printf("%s ", list[i].symbol);
        if(list[i].type == 2)
        {
            printf("%s ", list[i].name);
        }
        else if(list[i].type == 3)
        {
            printf("%d ", list[i].value);
        }
		i++;
	}
    printf("\n\n");
}
/*
int main(void)
{
	lexeme *list = lex_analyze("input.txt");

	
}
*/