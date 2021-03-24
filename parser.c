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

void block(int lev, int tx, symbol* table, FILE* ifp, instruction* code);
void constdeclaration(int lev, int *ptx, int *pdx, FILE* ifp, symbol* table);
void vardeclaration(int lev, int *ptx, int *pdx, FILE* ifp, symbol* table);
void statement(int lev, int *ptx, FILE* ifp, instruction* code, symbol* table);
void condition(int lev, int *ptx, FILE* ifp, instruction* code, symbol* table);
void expression(int lev, int *ptx, FILE* ifp, instruction* code, symbol* table);
void term(int lev, int *ptx, FILE* ifp, instruction* code, symbol* table);
void factor(int lev, int *ptx, symbol* table, FILE* ifp, instruction* code);

int codeidx, token, num, kind, lexemeListIndex=0, errorCount=0, difference, previousDifference=0;
char id[12];

void emit(int opcode, char op[], int l, int m);

instruction *parse(token_struct *tokenList, int print)
{
	code = malloc(500 * sizeof(instruction));

	symbol table[MAX_SYMBOL_TABLE_SIZE] = {0};

	

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