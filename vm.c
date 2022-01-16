// Annabl Bland and John Bramuchi

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "codegen.h"

//   Constants, Structures, and Function signatures
//====================================================
/*
struct instruction{
	int op;
	int r;
	int l; // indicates lexigraphical levl - used l so we don't think it's a 1
	int m;	// can indicate a number, program address, a data address, or the identity of the operand
};
*/
#define MAX_STACK_HEIGHT 1000
#define MAX_CODE_LENGTH 500
#define REGISTER_NUM 8

int base(int stack[], int l, int base);
void print_results(int rf[], int stack[], int pc, int bp, int sp, int size, char* lines, int DLarray[]);
void print_initial(int rf[], int stack[], int pc, int bp, int sp, int size);

void virtual_machine(instruction *code, int print)
{
	//   Declarables
	//=================
		
	char* op_codes[23] = {"lit", "rtn", "lod", "sto", "cal", "inc", "jmp", "jpc", "sys", "neg", "add",
						  "sub", "mul", "div", "odd", "mod", "eql", "neq", "lss", "leq", "gtr", "geq"};					  
	int stack[MAX_STACK_HEIGHT] = {0}; // main stack
	int bp = MAX_STACK_HEIGHT - 1; // base pointer
	int sp = MAX_STACK_HEIGHT; // stack pointer
	int pc = 0; // program counter
	int ir; // instruction register (1 - 22)
	int rf[REGISTER_NUM] = {0}; // holds all 8 registers
	int halt = 1; // flag to halt program if it sees sys(m==3) in input
	int line = 0; // incrementor used in printing
	int size; // number of input lines
	char** allLines = malloc(MAX_CODE_LENGTH * sizeof(char*)); // holds each line of input as a String	  
	int pc_flag = 0; // flag to indicate that pc has been modified in an execute cycle
	int index; // used as a temporary variable for OPcode #4
	int DLarray[MAX_STACK_HEIGHT] = {0}; // records instances of dynamic links in the stack
	
	
	
	//   Print cleaned up input
	//============================
	/*
	printf("Line	OP    R  L  M\n");
	while (code[line].opcode > 0 && code[line].opcode < 35)
	{
		//printf("%2d      %s   %d  %d  %d\n", line, op_codes[code[line].opcode-1], code[line].r, code[line].l, code[line].m);
		allLines[line] = malloc(100*sizeof(char));
		sprintf(allLines[line], "%s   %d  %d  %d", op_codes[code[line].opcode-1], code[line].r, code[line].l, code[line].m);
		line++;
	}
	size = line - 1;
	*/
	
	  
	//   Main Cycle
	//================
	if (print == 1)
		print_initial(rf, stack, pc, bp, sp, size);
	line = 0;
	while (halt != 0)
	{
		pc_flag = 0;
		if (print == 1)
		{
			printf("\n                              PC   BP   SP");
			printf("\n%d ",pc);
		}
		// execute respective function given its' OPcode
		switch(code[pc].opcode)
		{		
			//   BASIC INSTRUCTIONS
			//------------------------
			// LIT
			case 1:
				rf[code[pc].r] = code[pc].m;
				break;

			// RTN
			case 2:
				sp = bp + 1;
				bp = stack[sp-2];
				pc = stack[sp-3];
				pc_flag = 1;
				break;
						
			// LOD
			case 3:
				rf[code[pc].r] = stack[base(stack, code[pc].l, bp)-code[pc].m];
				break;
						
			// STO
			case 4:
				index = base(stack, code[pc].l, bp);
				stack[index - code[pc].m] = rf[code[pc].r];
				break;
						
			// CAL
			case 5:
				stack[sp-1] = base(stack, code[pc].l, bp);
				stack[sp-2] = bp; // DL
				stack[sp-3] = pc + 1;
				bp = sp - 1;
				DLarray[bp] = 1; // Store Dynamic Link
				pc = code[pc].m;
				pc_flag = 1;
				break;
							
			// INC
			case 6:
				sp = sp - code[pc].m;
				break;
							
			// JMP
			case 7:
				pc = code[pc].m;
				pc_flag = 1;
				break;
							
			// JPC	
			case 8:
				if(rf[code[pc].r] == 0)
				{
					pc = code[pc].m;
					pc_flag = 1;
				}
				break;
							
			// SYS
			case 9:
				//3 different functions dependant on 'M'
				if (code[pc].m == 1 && print == 1) // COME BACK TO THIS!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
					printf("%d\n",rf[code[pc].r]); // print regester file specified by 'r'
				else if(code[pc].m == 2)
					scanf("%d", &rf[code[pc].r]); // place user input in register file specified by 'r'
				else
					halt = 0; // halt program
				break;
			
			
			//   ARITHMETIC FUNCTIONS
			//--------------------------
			// NEG
			case 10:
				rf[code[pc].r] *= -1;
				break;
							
			// ADD
			case 11:
				rf[code[pc].r] = rf[code[pc].l] + rf[code[pc].m];
				break;
							
			// SUB
			case 12:
				rf[code[pc].r] = rf[code[pc].l] - rf[code[pc].m];
				break;
						
			// MUL
			case 13:
				rf[code[pc].r] = rf[code[pc].l] * rf[code[pc].m];
				break;
						
			// DIV
			case 14:
				rf[code[pc].r] = rf[code[pc].l] / rf[code[pc].m];
				break;
							
			// ODD
			case 15:
				rf[code[pc].r] %= 2;
				break;
						
			// MOD
			case 16:
				rf[code[pc].r] = rf[code[pc].l] % rf[code[pc].m];
				break;
						
			// EQL
			case 17:
				if (rf[code[pc].l] == rf[code[pc].m])
					rf[code[pc].r] = 1;
				else
					rf[code[pc].r] = 0;
				break;
						
			// NEQ
			case 18:
				if (rf[code[pc].l] != rf[code[pc].m])
					rf[code[pc].r] = 1;
				else
					rf[code[pc].r] = 0;
				break;
						
			// LSS
			case 19:
				if (rf[code[pc].l] < rf[code[pc].m])
					rf[code[pc].r] = 1;
				else
					rf[code[pc].r] = 0;
				break;
						
			// LEQ
			case 20:
				if (rf[code[pc].l] <= rf[code[pc].m])
					rf[code[pc].r] = 1;
				else
					rf[code[pc].r] = 0;
				break;
						
			// GTR
			case 21:
				if(rf[code[pc].l] > rf[code[pc].m])
					rf[code[pc].r] = 1;
				else
					rf[code[pc].r] = 0;
				break;
							
			// GEQ
			case 22:
				if(rf[code[pc].l] >= rf[code[pc].m])
					rf[code[pc].r] = 1;
				else
					rf[code[pc].r] = 0;
				break; 
		}
		
		// increment pc if it was not modified by OPcode function
		if(pc_flag == 0)
		{
			pc++;
		}
		
		// print changes made to the terminal
		if (print == 1)
			print_results(rf, stack, pc, bp, sp, size, allLines[line], DLarray);

		line = pc;
	}
	
}

// function taken from homework rubric (HW1 PL0 Register.machine.fa20.doc)
int base(int stack[], int l, int base)
{  
	int b_one; //find base L levels up
	b_one = base; 
	while (l > 0)
	{
		b_one = stack[b_one];
		l--;
	}
	return b_one;
}

// Prints the EMPTY contents of the stack, register files, and pointers with correct formatting
void print_initial(int rf[], int stack[], int pc, int bp, int sp, int size)
{
	printf("\n                              PC   BP   SP\n");
	printf("Initial Values:               %d   %d   %d\n", pc, bp, sp);
	printf("Registers: ");
	for(int i = 0; i < REGISTER_NUM; i++)
	{
		printf("%d", rf[i]);
		printf(i == (REGISTER_NUM - 1) ? "\n" : " ");
	}
	  
	// Print contents of stack only up to stack pointer
	printf("Stack: ");
	for(int i = MAX_STACK_HEIGHT-1; i > MAX_STACK_HEIGHT - size;i--)
	{
		printf("%d ", stack[i]);
	}
	printf("\n");
}

// Prints the contents of the stack, register files, and pointers with correct formatting
void print_results(int rf[], int stack[], int pc, int bp, int sp, int size, char* lines, int DLarray[])
{
	printf(" %s:             %d   %d   %d\n", lines, pc, bp, sp);
	printf("Registers: ");
	for(int i = 0; i < REGISTER_NUM; i++)
	{
		printf("%d", rf[i]);
		printf(i == (REGISTER_NUM - 1) ? "\n" : " ");
	}
	  
	// Print contents of stack only up to stack pointer
	printf("Stack: ");
	for(int i = MAX_STACK_HEIGHT-1; i > MAX_STACK_HEIGHT - size;i--)
	{
		if(DLarray[i] == 1)
		{
			printf("| ");
		}
		printf("%d ", stack[i]);
	}
	printf("\n");
}