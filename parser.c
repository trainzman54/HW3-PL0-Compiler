#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compiler.h"

instruction *code;
int codeidx = 0;
char *errormsg[] = {
	"",
	"Error : program must end with period\n",
	"Error : const, var, and read keywords must be followed by identifier\n",
	"Error : symbol name has already been declared\n",
	"Error : constants must be assigned with =\n",
	"Error : constants mustbe assigned an integer value\n",
	"Error : constant and variable declarations must be followed by a semicolon\n",
	"Error : undeclared symbol\n",
	"Error : only variable values may be altered\n",
	"Error : assignment statements must use :=\n",
	"Error : begin must be followed by end\n",
	"Error : if must be followed by then\n",
	"Error : while must be followed by do\n",
	"Error : condition must contain comparison operator\n",
	"Error : right parenthesis must follow left parenthesis\n",
	"Error : arithmetic equations must contain operands, parentheses, numbers, or symbols\n"
};

void emit(int opcode, char op[], int l, int m);

instruction *parse(token_struct *tokenList, int print)
{
	code = malloc(500 * sizeof(instruction));
	printf("code has been parsed\n");
	return code;
}

void emit(int opcode, char op[], int l, int m)
{
	code[codeidx].opcode = opcode;
	strcpy(code[codeidx].op, op);
	code[codeidx].l = l;
	code[codeidx].m = m;
	codeidx++;
}