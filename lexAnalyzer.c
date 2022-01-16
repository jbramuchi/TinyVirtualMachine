#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "lexAnalyzer.h"
#include "parser.h"


// returns 1 if string is comprised of numbers ONLY, else return 0
int charsAreNum(char* str)
{
  int i=0;
  while(str[i] != '\0')
  {
    if(!(str[i]>=48 && str[i]<=57))
      return 0;

    i+=1;
  }
  return 1;
}

// returns 1 if string is comprised of numbers and alphabetical characters ONLY, else return 0
int charsAreAlNum(char* str)
{
  int i=0;
  while(str[i] != '\0')
  {
    if(!isalnum(str[i]))
      return 0;

    i+=1;
  }
  return 1;
}

char** lexAnalyzer(char *filename)
  int objects = 0, i = 0, length = 0, j = 0;
  char** tokenArray;  // holds generated tokens
  char c; // variable to scan document with
  char* s = malloc(12 * sizeof(char));  // variable to scan document with
  char* prev = NULL;  // placeholder variable to scan document with
  FILE *input = fopen(filename, "r"); // holds file input
  FILE *groomedCode = fopen("output.txt", "w"); // holds groomed fileinput in "write" mode
  FILE *new1; // holds groomed file input in "read" mode
  int flag = 0; // used in tokenArray iteration
  int slash_flag = 0;
  int block_flag = 0;


  // Print source file to terminal and groom code:
  // grooming puts spaces between all tokenizable objects.
  printf("Source Program:\n");
  while(fscanf(input, "%c", &c) != EOF)
  {
    // Print input
    printf("%c", c);
  	if (slash_flag == 2)
  	{
  		if (c == '\n')
  		{
  			slash_flag = 0;
  			continue;
  		}
  		continue;
  	}
  	if (block_flag == 2)
  	{
  		if (c == '*')
  		{
  			fscanf(input, "%c", &c);
        printf("%c", c);
  			if (c == '/')
  			{
  				block_flag = 0;
  				continue;
  			}
  		}
  		continue;
  	}
    if(isspace(c))
  	{
        objects += 1;
  	}
  	else if (c == '/')
  	{
  		block_flag++;
  		slash_flag++;
  		continue;
  	}
    else if (c == '*')
  	{
  		block_flag++;
  		continue;
  	}
  	else if (c == '>' || c == '<' || c == '!' || c == ':')
  	{
  		fputc(c, groomedCode);
  		flag = 1;
  		objects++;
  		continue;
  	}
  	else if (c == '=')
  	{
  		if (flag == 1)
  		{
  			fputc(c, groomedCode);
  			flag = 0;
  			continue;
  		}
  		else
  		{
  			fputc(' ', groomedCode);
  			fputc(c, groomedCode);
  			objects++;
  			continue;
  		}
  	}
    else if(!isalnum(c) && !isspace(c))
    {
      fputc(' ', groomedCode);
      fputc(c, groomedCode);
      objects += 1;
      continue;
    }
    fputc(c, groomedCode);
  	flag = 0;
  	slash_flag = 0;
  	block_flag = 0;
}

  // Close output.txt so that we may read it
  fclose(groomedCode);
  fclose(input);

  // Allocate space for our tokens.
  tokenArray = malloc(objects * sizeof(char*));

  // Print Lexeme Table
  printf("\n\nLexeme Table:\nlexeme       token type\n");
  new1 = fopen("output.txt", "r");  // Open groomed groomedCode

  while(fscanf(new1, "%s", s) != EOF)
  {
    printf("%-12s", s);
    length = strlen(s);

    if(strstr(s, "nul") != NULL)
    {
      // nulsym = 1
      printf("1\n");
      tokenArray[i] = malloc(2 * sizeof(char));
      strcpy(tokenArray[i], "1");
    }
    else if(strchr(s, '+') != NULL)
    {
      // plussym = 4
      printf("4\n");
      tokenArray[i] = malloc(2 * sizeof(char));
      strcpy(tokenArray[i], "4");
    }
    else if(strchr(s, '-') != NULL)
    {
      // minussym = 5
      printf("5\n");
      tokenArray[i] = malloc(2 * sizeof(char));
      strcpy(tokenArray[i], "5");
    }
    else if(strchr(s, '*') != NULL)
    {
      // multsym = 6
      printf("6\n");
      tokenArray[i] = malloc(2 * sizeof(char));
      strcpy(tokenArray[i], "6");
    }
    else if(strchr(s, '/') != NULL)
    {
      // slashsym = 7
      printf("7\n");
      tokenArray[i] = malloc(2 * sizeof(char));
      strcpy(tokenArray[i], "7");
    }
    else if(strchr(s, '(') != NULL)
    {
     // oddsym = 8
      printf("8\n");
      tokenArray[i] = malloc(2 * sizeof(char));
      strcpy(tokenArray[i], "8");
    }
	  else if(strstr(s, ":=") != NULL)
    {
      // becomessym = 20
      printf("20\n");
      tokenArray[i] = malloc(3 * sizeof(char));
      strcpy(tokenArray[i], "20");
    }
    else if(strchr(s, '=') != NULL)
    {
      // eqlsym = 9
      printf("9\n");
      tokenArray[i] = malloc(2 * sizeof(char));
      strcpy(tokenArray[i], "9");
    }
    else if(strstr(s, "!=") != NULL)
    {
      // neqsym = 10,
      printf("10\n");
      tokenArray[i] = malloc(3 * sizeof(char));
      strcpy(tokenArray[i], "10");
    }
    else if(strchr(s, '<') != NULL)
    {
      // lessym = 11,
      printf("11\n");
      tokenArray[i] = malloc(3 * sizeof(char));
      strcpy(tokenArray[i], "11");
    }
    else if(strstr(s, "<=") != NULL)
    {
      // leqsym = 12,
      printf("12\n");
      tokenArray[i] = malloc(3 * sizeof(char));
      strcpy(tokenArray[i], "12");
    }
    else if(strchr(s, '>') != NULL)
    {
      // gtrsym = 13,
      printf("13\n");
      tokenArray[i] = malloc(3 * sizeof(char));
      strcpy(tokenArray[i], "13");
    }
    else if(strstr(s, ">=") != NULL)
    {
      // geqsym = 14
      printf("14\n");
      tokenArray[i] = malloc(3 * sizeof(char));
      strcpy(tokenArray[i], "14");
    }
   else if(strchr(s, '(') != NULL)
    {
      // lparentsym = 15
      printf("15\n");
      tokenArray[i] = malloc(3 * sizeof(char));
      strcpy(tokenArray[i], "15");
    }
    else if(strchr(s, ')') != NULL)
    {
      // rparentsym = 16
      printf("16\n");
      tokenArray[i] = malloc(3 * sizeof(char));
      strcpy(tokenArray[i], "16");
    }
    else if(strchr(s, ',') != NULL)
    {
      // commasym = 17
      printf("17\n");
      tokenArray[i] = malloc(3 * sizeof(char));
      strcpy(tokenArray[i], "17");
    }
      else if(strchr(s, ';') != NULL)
    {
      // semicolonsym = 18
      printf("18\n");
      tokenArray[i] = malloc(3 * sizeof(char));
      strcpy(tokenArray[i], "18");
    }
    else if(strchr(s, '.') != NULL)
    {
      // periodsym = 19
      printf("19\n");
      tokenArray[i] = malloc(3 * sizeof(char));
      strcpy(tokenArray[i], "19");
    }

   else if(strstr(s, "begin") != NULL)
    {
      // beginsym = 21
      printf("21\n");
      tokenArray[i] = malloc(3 * sizeof(char));
      strcpy(tokenArray[i], "21");
    }
    else if(strstr(s, "end") != NULL)
    {
      // endsym = 22
      printf("22\n");
      tokenArray[i] = malloc(3 * sizeof(char));
      strcpy(tokenArray[i], "22");
    }
    else if(strstr(s, "if") != NULL)
    {
      // ifsym = 23
      printf("23\n");
      tokenArray[i] = malloc(3 * sizeof(char));
      strcpy(tokenArray[i], "23");
    }
    else if(strstr(s, "then") != NULL)
    {
      // thensym = 24
      printf("24\n");
      tokenArray[i] = malloc(3 * sizeof(char));
      strcpy(tokenArray[i], "24");
    }
    else if(strstr(s, "while") != NULL)
    {
      // whilesym = 25
      printf("25\n");
      tokenArray[i] = malloc(3 * sizeof(char));
      strcpy(tokenArray[i], "25");
    }
    else if(strstr(s, "do") != NULL)
    {
      // dosym = 26
      printf("26\n");
      tokenArray[i] = malloc(3 * sizeof(char));
      strcpy(tokenArray[i], "26");
    }
    else if(strstr(s, "call") != NULL)
    {
      // callsym = 27
      printf("27\n");
      tokenArray[i] = malloc(3 * sizeof(char));
      strcpy(tokenArray[i], "27");
    }
    else if(strstr(s, "const") != NULL)
    {
      // constsym = 28
      printf("28\n");
      tokenArray[i] = malloc(3 * sizeof(char));
      strcpy(tokenArray[i], "28");
    }
    else if(strstr(s, "var") != NULL)
    {
      // varsym = 29
      printf("29\n");
      tokenArray[i] = malloc(3 * sizeof(char));
      strcpy(tokenArray[i], "29");
    }
    else if(strstr(s, "procedure") != NULL)
    {
      // procsym = 30
      printf("30\n");
      tokenArray[i] = malloc(3 * sizeof(char));
      strcpy(tokenArray[i], "30");
    }
    else if(strstr(s, "write") != NULL)
    {
      // writesym = 31
      printf("31\n");
      tokenArray[i] = malloc(3 * sizeof(char));
      strcpy(tokenArray[i], "31");
    }
    else if(strstr(s, "read") != NULL)
    {
      // readsym = 32
      printf("32\n");
      tokenArray[i] = malloc(3 * sizeof(char));
      strcpy(tokenArray[i], "32");
    }
    else if(strstr(s, "else") != NULL)
    {
      // elsesym = 33
      printf("33\n");
      tokenArray[i] = malloc(3 * sizeof(char));
      strcpy(tokenArray[i], "33");
    }
    else
    {
      // numsym = 3
      if(charsAreNum(s))
      {
        // Err: invalid identifier
        if(prev != NULL && strstr(prev, "var") != NULL)
        {
          printf("Error: Identifier should not start with a number\n");
          tokenArray[i] = malloc(4 * sizeof(char));
          strcpy(tokenArray[i], "Err");
          i++;
          prev = s;
          continue;
        }
        // Err: invalid number size
        else if(length > 5)
        {
          printf("Error: Number too long (MAX: 5 digits.)\n");
          tokenArray[i] = malloc(4 * sizeof(char));
          strcpy(tokenArray[i], "Err");
          i++;
          prev = s;
          continue;
        }

        // Tokenized numbers tokenize to 3 as well as the actual number value
        printf("3\n");
        tokenArray[i] = malloc(2 * sizeof(char));
        strcpy(tokenArray[i], "3");
        i+=1;
        tokenArray[i] = malloc((strlen(s)+1) * sizeof(char));
        strcpy(tokenArray[i], s);
      }

      // identsym = 2
      else if(charsAreAlNum(s))
      {
        if(s[0] >= 48 && s[0] <= 57)
        {
          printf("Error: Identifier should not start with a number\n");
          tokenArray[i] = malloc(4 * sizeof(char));
          strcpy(tokenArray[i], "Err");
          i++;
          prev = s;
          continue;
        }
        else if(length > 11)
        {
          printf(" Error: Identifier too long (MAX: 11 digits.)\n");
          tokenArray[i] = malloc(4 * sizeof(char));
          strcpy(tokenArray[i], "Err");
          i++;
          prev = s;
          continue;
        }

        // Tokenized identifiers tokenize to 2 as well as the actual identifier value
        printf("2\n");
        tokenArray[i] = malloc(2 * sizeof(char));
        strcpy(tokenArray[i], "2");
        i+=1;
        tokenArray[i] = malloc((strlen(s)+1) * sizeof(char));
        strcpy(tokenArray[i], s);
      }

      // If we reach here, fscanf has processed an invalid character
      // so, Err: Invalid symbol
      else
      {
        printf("Error: Invalid symbol\n");
        tokenArray[i] = malloc(4 * sizeof(char));
        strcpy(tokenArray[i], "Err");
        i++;
        prev = s;
        continue;

      }
    }

    i++;

    // Keep track of previous string value
    prev = s;
  }
  fclose(new1);
  free(s);

  printf("\nLexeme List:\n");
  for (j=0;j<i;j++)
  {
    printf("%s ", tokenArray[j]);
  }
  printf("\n");

  return tokenArray;

  // Free 2D char array, row by row
  for(int k = 0; k < i; k++)
  {
    free(tokenArray[k]);
  }
  free(tokenArray);
}
