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
	int i = 0;

	// Variables specified in assignment doc
	int stack[MAX_STACK_HEIGHT] = {0};
	int SP = -1;
	int BP = 0;
	int PC = 0;
	int Halt = 1;

	if (print == 1)
		printf("                PC   BP   SP   stack\nInitial values: %2d   %2d   %2d\n", PC, BP, SP);

	// Loops through until reaching the Halt condition (9 0 3)
	for (i = 0; Halt != 0; i++)
	{		
		// Handles SYS operations printed above SYS lines in output
		if (code[i].opcode == 9)
		{
			if (code[i].m == 1)
			{
				if (print == 1)
					printf("Top of Stack Value: %d\n", stack[SP]);
				SP = SP - 1;
			}
			else if (code[i].m == 2)
			{
				SP = SP + 1;
				printf("Please enter an integer: ");
				scanf("%d", &stack[SP]);
			}
			else if (code[i].m == 3)
			{
				Halt = 0;
			}
		}

		// Prints current PC
		if (print == 1)
			printf("%2d ", PC);

		// Handles each possible opcode within ISA
		if (code[i].opcode == 1) // LIT
		{
			if (print == 1)
				printf("LIT  ");
			SP++;
			stack[SP] = code[i].m;
			PC++;
		}
		else if (code[i].opcode == 2) // OPR
		{
			if (code[i].m == 0)
			{
				if (print == 1)
					printf("RTN  ");
				stack[BP - 1] = stack[SP];
				SP = BP - 1;
				BP = stack[SP + 2];
				PC = stack[SP + 3];
			}
			else if (code[i].m == 1)
			{
				if (print == 1)
					printf("NEG  ");
				stack[SP] = -1 * stack[SP];
				PC++;
			}
			else if (code[i].m == 2)
			{
				if (print == 1)
					printf("ADD  ");
				SP = SP - 1;
				stack[SP] = stack[SP] + stack[SP + 1];
				PC++;
			}
			else if (code[i].m == 3)
			{
				if (print == 1)
					printf("SUB  ");
				SP = SP - 1;
				stack[SP] = stack[SP] - stack[SP + 1];
				PC++;
			}
			else if (code[i].m == 4)
			{
				if (print == 1)
					printf("MUL  ");
				SP = SP - 1;
				stack[SP] = stack[SP] * stack[SP + 1];
				PC++;
			}
			else if (code[i].m == 5)
			{
				if (print == 1)
					printf("DIV  ");
				SP = SP - 1;
				stack[SP] = stack[SP] / stack[SP + 1];
				PC++;
			}
			else if (code[i].m == 6)
			{
				if (print == 1)
					printf("ODD  ");
				stack[SP] = stack[SP] % 2;
				PC++;
			}
			else if (code[i].m == 7)
			{
				if (print == 1)
					printf("MOD  ");
				SP = SP - 1;
				stack[SP] = stack[SP] % stack[SP + 1];
				PC++;
			}
			else if (code[i].m == 8)
			{
				if (print == 1)
					printf("EQL  ");
				SP = SP - 1;
				stack[SP] = stack[SP] == stack[SP + 1];
				PC++;
			}
			else if (code[i].m == 9)
			{
				if (print == 1)
					printf("NEQ  ");
				SP = SP - 1;
				stack[SP] = stack[SP] != stack[SP + 1];
				PC++;
			}
			else if (code[i].m == 10)
			{
				if (print == 1)
					printf("LSS  ");
				SP = SP - 1;
				stack[SP] = stack[SP] < stack[SP + 1];
				PC++;
			}
			else if (code[i].m == 11)
			{
				if (print == 1)
					printf("LEQ  ");
				SP = SP - 1;
				stack[SP] = stack[SP] <= stack[SP + 1];
				PC++;
			}
			else if (code[i].m == 12)
			{
				if (print == 1)
					printf("GTR  ");
				SP = SP - 1;
				stack[SP] = stack[SP] > stack[SP + 1];
				PC++;
			}
			else if (code[i].m == 13)
			{
				if (print == 1)
					printf("GEQ  ");
				SP = SP - 1;
				stack[SP] = stack[SP] >= stack[SP + 1];
				PC++;
			}
		}
		else if (code[i].opcode == 3) // LOD
		{
			if (print == 1)
				printf("LOD  ");
			SP = SP + 1;
			stack[SP] = stack[base(stack, code[i].l, BP) + code[i].m];
			PC++;
		}
		else if (code[i].opcode == 4) // STO
		{
			if (print == 1)
				printf("STO  ");
			stack[base(stack, code[i].l, BP) + code[i].m] = stack[SP];
			SP = SP - 1;
			PC++;
		}
		else if (code[i].opcode == 5) // CAL
		{
			PC++;
			if (print == 1)
				printf("CAL  ");
			stack[SP + 1] = base(stack, code[i].l, BP);
			stack[SP + 2] = BP;
			stack[SP + 3] = PC;
			stack[SP + 4] = stack[SP];
			BP = SP + 1;
			PC = code[i].m;
		}
		else if (code[i].opcode == 6) // INC
		{
			if (print == 1)
				printf("INC  ");
			SP = SP + code[i].m;
			PC++;
		}
		else if (code[i].opcode == 7) // JMP
		{
			if (print == 1)
				printf("JMP  ");
			PC = code[i].m;
		}
		else if (code[i].opcode == 8) // JPC
		{
			if (print == 1)
				printf("JPC  ");
			if (stack[SP] == 0)
			{
				PC = code[i].m;
			}
			SP = SP - 1;
			PC++;
		}
		else if (code[i].opcode == 9) // SYS
		{
			if (print == 1)
				printf("SYS  ");
			PC++;
		}
		// Handles printing of L, M, PC, BP, & SP
		if (print == 1)
		{
			printf("%d %2d    ", code[i].l, code[i].m);
			printf("%2d   %2d   %2d   ", PC, BP, SP);
		}

		// Handles Stack Printing
		if (SP >= 0 && print == 1)
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