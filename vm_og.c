// Howard McDavid

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

// Assignment defined values
#define MAX_STACK_HEIGHT 50
#define MAX_CODE_LENGTH 100

// Instruction struct used for ease of access
typedef struct Instruction {
	int opcode;
	int l;
	int m;
} Instruction;

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

int main (int argc, char **argv)
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
	while (Halt != 0)
	{
		// Obtains necessary information from the current line
		sscanf(input_line[PC], "%d%d%d", &instruction.opcode,
		       &instruction.l, &instruction.m);
		
		// Handles SYS operations printed above SYS lines in output
		if (instruction.opcode == 9)
		{
			if (instruction.m == 1)
			{
				printf("Top of Stack Value: %d\n", stack[SP]);
				SP = SP - 1;
			}
			else if (instruction.m == 2)
			{
				SP = SP + 1;
				printf("Please enter an integer: ");
				scanf("%d", &stack[SP]);
			}
			else if (instruction.m == 3)
			{
				Halt = 0;
			}
		}

		// Prints current PC
		printf("%2d ", PC);

		// Handles each possible opcode within ISA
		if (instruction.opcode == 1) // LIT
		{
			printf("LIT  ");
			SP++;
			stack[SP] = instruction.m;
			PC++;
		}
		else if (instruction.opcode == 2) // OPR
		{
			if (instruction.m == 0)
			{
				printf("RTN  ");
				stack[BP - 1] = stack[SP];
				SP = BP - 1;
				BP = stack[SP + 2];
				PC = stack[SP + 3];
			}
			else if (instruction.m == 1)
			{
				printf("NEG  ");
				stack[SP] = -1 * stack[SP];
				PC++;
			}
			else if (instruction.m == 2)
			{
				printf("ADD  ");
				SP = SP - 1;
				stack[SP] = stack[SP] + stack[SP + 1];
				PC++;
			}
			else if (instruction.m == 3)
			{
				printf("SUB  ");
				SP = SP - 1;
				stack[SP] = stack[SP] - stack[SP + 1];
				PC++;
			}
			else if (instruction.m == 4)
			{
				printf("MUL  ");
				SP = SP - 1;
				stack[SP] = stack[SP] * stack[SP + 1];
				PC++;
			}
			else if (instruction.m == 5)
			{
				printf("DIV  ");
				SP = SP - 1;
				stack[SP] = stack[SP] / stack[SP + 1];
				PC++;
			}
			else if (instruction.m == 6)
			{
				printf("ODD  ");
				stack[SP] = stack[SP] % 2;
				PC++;
			}
			else if (instruction.m == 7)
			{
				printf("MOD  ");
				SP = SP - 1;
				stack[SP] = stack[SP] % stack[SP + 1];
				PC++;
			}
			else if (instruction.m == 8)
			{
				printf("EQL  ");
				SP = SP - 1;
				stack[SP] = stack[SP] == stack[SP + 1];
				PC++;
			}
			else if (instruction.m == 9)
			{
				printf("NEQ  ");
				SP = SP - 1;
				stack[SP] = stack[SP] != stack[SP + 1];
				PC++;
			}
			else if (instruction.m == 10)
			{
				printf("LSS  ");
				SP = SP - 1;
				stack[SP] = stack[SP] < stack[SP + 1];
				PC++;
			}
			else if (instruction.m == 11)
			{
				printf("LEQ  ");
				SP = SP - 1;
				stack[SP] = stack[SP] <= stack[SP + 1];
				PC++;
			}
			else if (instruction.m == 12)
			{
				printf("GTR  ");
				SP = SP - 1;
				stack[SP] = stack[SP] > stack[SP + 1];
				PC++;
			}
			else if (instruction.m == 13)
			{
				printf("GEQ  ");
				SP = SP - 1;
				stack[SP] = stack[SP] >= stack[SP + 1];
				PC++;
			}
		}
		else if (instruction.opcode == 3) // LOD
		{
			printf("LOD  ");
			SP = SP + 1;
			stack[SP] = stack[base(stack, instruction.l, BP) + instruction.m];
			PC++;
		}
		else if (instruction.opcode == 4) // STO
		{
			printf("STO  ");
			stack[base(stack, instruction.l, BP) + instruction.m] = stack[SP];
			SP = SP - 1;
			PC++;
		}
		else if (instruction.opcode == 5) // CAL
		{
			PC++;
			printf("CAL  ");
			stack[SP + 1] = base(stack, instruction.l, BP);
			stack[SP + 2] = BP;
			stack[SP + 3] = PC;
			stack[SP + 4] = stack[SP];
			BP = SP + 1;
			PC = instruction.m;
		}
		else if (instruction.opcode == 6) // INC
		{
			printf("INC  ");
			SP = SP + instruction.m;
			PC++;
		}
		else if (instruction.opcode == 7) // JMP
		{
			printf("JMP  ");
			PC = instruction.m;
		}
		else if (instruction.opcode == 8) // JPC
		{
			printf("JPC  ");
			if (stack[SP] == 0)
			{
				PC = instruction.m;
			}
			SP = SP - 1;
			PC++;
		}
		else if (instruction.opcode == 9) // SYS
		{
			printf("SYS  ");
			PC++;
		}
		// Handles printing of L, M, PC, BP, & SP
		printf("%d %2d    ", instruction.l, instruction.m);
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
}