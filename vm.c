#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compiler.h"

// Assignment defined values
#define MAX_STACK_HEIGHT 50
#define MAX_CODE_LENGTH 100

// base() function, provided by Professor Euripides Montagne
int base(int stack[], int level, int BP)
{
	int base = BP;
	int counter = level;
	while (counter > 0)
	{
		base = stack[base];
		counter--;
	}
	return base;
}

void execute(instruction *code, int print)
{
	// Variables created specifically for this program
	FILE* input = fopen(argv[1], "r");
	char input_line[MAX_CODE_LENGTH][32];
	int i = 0;

	// Variables specified in assignment doc
	int stack[MAX_STACK_HEIGHT] = {0};
	int SP = -1;
	int BP = 0;
	int PC = 0;
	Instruction instruction;
	int Halt = 1;

	// Puts each instruction line from the input file
	// into an array of strings
	while (fgets(input_line[i], 32, input) != NULL && i < MAX_CODE_LENGTH)
	{
		input_line[i][strlen(input_line[i])] = '\0';
		i++;
	}
	fclose(input);

	printf("                PC   BP   SP   stack\nInitial values: %2d   %2d   %2d\n", PC, BP, SP);

	// Loops through until reaching the Halt condition (9 0 3)
	for (i = 0; Halt != 0; i++)
	{		
		// Handles SYS operations printed above SYS lines in output
		if (code[i]->opcode == 9)
		{
			if (code[i]->m == 1)
			{
				printf("Top of Stack Value: %d\n", stack[SP]);
				SP = SP - 1;
			}
			else if (code[i]->m == 2)
			{
				SP = SP + 1;
				printf("Please enter an integer: ");
				scanf("%d", &stack[SP]);
			}
			else if (code[i]->m == 3)
			{
				Halt = 0;
			}
		}

		// Prints current PC
		printf("%2d ", PC);

		// Handles each possible opcode within ISA
		if (code[i]->opcode == 1) // LIT
		{
			printf("LIT  ");
			SP++;
			stack[SP] = code[i]->m;
			PC++;
		}
		else if (code[i]->opcode == 2) // OPR
		{
			if (code[i]->m == 0)
			{
				printf("RTN  ");
				stack[BP - 1] = stack[SP];
				SP = BP - 1;
				BP = stack[SP + 2];
				PC = stack[SP + 3];
			}
			else if (code[i]->m == 1)
			{
				printf("NEG  ");
				stack[SP] = -1 * stack[SP];
				PC++;
			}
			else if (code[i]->m == 2)
			{
				printf("ADD  ");
				SP = SP - 1;
				stack[SP] = stack[SP] + stack[SP + 1];
				PC++;
			}
			else if (code[i]->m == 3)
			{
				printf("SUB  ");
				SP = SP - 1;
				stack[SP] = stack[SP] - stack[SP + 1];
				PC++;
			}
			else if (code[i]->m == 4)
			{
				printf("MUL  ");
				SP = SP - 1;
				stack[SP] = stack[SP] * stack[SP + 1];
				PC++;
			}
			else if (code[i]->m == 5)
			{
				printf("DIV  ");
				SP = SP - 1;
				stack[SP] = stack[SP] / stack[SP + 1];
				PC++;
			}
			else if (code[i]->m == 6)
			{
				printf("ODD  ");
				stack[SP] = stack[SP] % 2;
				PC++;
			}
			else if (code[i]->m == 7)
			{
				printf("MOD  ");
				SP = SP - 1;
				stack[SP] = stack[SP] % stack[SP + 1];
				PC++;
			}
			else if (code[i]->m == 8)
			{
				printf("EQL  ");
				SP = SP - 1;
				stack[SP] = stack[SP] == stack[SP + 1];
				PC++;
			}
			else if (code[i]->m == 9)
			{
				printf("NEQ  ");
				SP = SP - 1;
				stack[SP] = stack[SP] != stack[SP + 1];
				PC++;
			}
			else if (code[i]->m == 10)
			{
				printf("LSS  ");
				SP = SP - 1;
				stack[SP] = stack[SP] < stack[SP + 1];
				PC++;
			}
			else if (code[i]->m == 11)
			{
				printf("LEQ  ");
				SP = SP - 1;
				stack[SP] = stack[SP] <= stack[SP + 1];
				PC++;
			}
			else if (code[i]->m == 12)
			{
				printf("GTR  ");
				SP = SP - 1;
				stack[SP] = stack[SP] > stack[SP + 1];
				PC++;
			}
			else if (code[i]->m == 13)
			{
				printf("GEQ  ");
				SP = SP - 1;
				stack[SP] = stack[SP] >= stack[SP + 1];
				PC++;
			}
		}
		else if (code[i]->opcode == 3) // LOD
		{
			printf("LOD  ");
			SP = SP + 1;
			stack[SP] = stack[base(stack, code[i]->l, BP) + code[i]->m];
			PC++;
		}
		else if (code[i]->opcode == 4) // STO
		{
			printf("STO  ");
			stack[base(stack, code[i]->l, BP) + code[i]->m] = stack[SP];
			SP = SP - 1;
			PC++;
		}
		else if (code[i]->opcode == 5) // CAL
		{
			PC++;
			printf("CAL  ");
			stack[SP + 1] = base(stack, code[i]->l, BP);
			stack[SP + 2] = BP;
			stack[SP + 3] = PC;
			stack[SP + 4] = stack[SP];
			BP = SP + 1;
			PC = code[i]->m;
		}
		else if (code[i]->opcode == 6) // INC
		{
			printf("INC  ");
			SP = SP + code[i]->m;
			PC++;
		}
		else if (code[i]->opcode == 7) // JMP
		{
			printf("JMP  ");
			PC = code[i]->m;
		}
		else if (code[i]->opcode == 8) // JPC
		{
			printf("JPC  ");
			if (stack[SP] == 0)
			{
				PC = code[i]->m;
			}
			SP = SP - 1;
			PC++;
		}
		else if (code[i]->opcode == 9) // SYS
		{
			printf("SYS  ");
			PC++;
		}
		// Handles printing of L, M, PC, BP, & SP
		printf("%d %2d    ", code[i]->l, code[i]->m);
		printf("%2d   %2d   %2d   ", PC, BP, SP);

		// Handles Stack Printing
		if (SP >= 0)
		{
			for (int j = 0; j <= SP; j++)
			{
				if (BP > 0 && j == BP)
				{
					printf("| ");
				}
				printf("%d ", stack[j]);
			}
		}
		printf("\n");
	}
	printf("print the stack\n");
	return;
}