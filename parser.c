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

void block(int lev, int tx, symbol* table, token_struct *tokenList, instruction* code);
void constdeclaration(int lev, int *ptx, int *pdx, token_struct *tokenList, symbol* table);
void vardeclaration(int lev, int *ptx, int *pdx, token_struct *tokenList, symbol* table);
void statement(int lev, int *ptx, token_struct *tokenList, instruction* code, symbol* table);
void condition(int lev, int *ptx, token_struct *tokenList, instruction* code, symbol* table);
void expression(int lev, int *ptx, token_struct *tokenList, instruction* code, symbol* table);
void term(int lev, int *ptx, token_struct *tokenList, instruction* code, symbol* table);
void factor(int lev, int *ptx, symbol* table, token_struct *tokenList, instruction* code);
void program(token_struct *tokenList, symbol *table, instruction *code);
int position(char *id, int *ptx, symbol* table, int lev);

int codeidx, token, num, kind, lexemeListIndex=0, errorCount=0, difference, previousDifference=0, j = 0;
char id[12];

void emit(int opcode, int l, int m, instruction *code);
void enter(int k, int *ptx, int *pdx, int lev, symbol* table);
void error(int errorCase);


instruction *parse(token_struct *tokenList, int print)
{
	code = malloc(500 * sizeof(instruction));
	int i;
	int lev = 0;
	int dx = 0;
	int tx = 0;

	symbol table[MAX_SYMBOL_TABLE_SIZE] = {0};

	instruction code[CODE_SIZE];

	program(tokenList, table, code);

	return code;
}

void program(token_struct *tokenList, symbol *table, instruction *code)
{
	token = tokenList[j++].number_value;
	block(0, 0, table, tokenList, code);

	if(token != periodsym)
	{
		error(9);
	}
}

void block(int lev, int tx, symbol* table, token_struct *tokenList, instruction *code)
{
	int dx, tx0, codeidx0;
	dx = 4;
	tx0 = tx;
	table[tx].addr = codeidx;
	emit(7, 0, 0, code);

	do
	{
		if(token == constsym)
		{
			token = tokenList[j++].number_value;
			do
			{
				constdeclaration(lev, &tx, &dx, tokenList, table);
				while (token == commasym)
				{
					token = tokenList[j++].number_value;
					constdeclaration(lev, &tx, &dx, tokenList, table);
				}
				if(token == semicolonsym)
				{
					token = tokenList[j++].number_value;
				}
				else
				{
					error(5);
				}
			} while (token == identsym);
		}
		if(token == varsym)
		{
			token = tokenList[j++].number_value;
			do
			{
				vardeclaration(lev, &tx, &dx, tokenList, table);
				while (token == commasym)
				{
					token = tokenList[j++].number_value;
					vardeclaration(lev, &tx, &dx, tokenList, table);
				}
				if (semicolonsym)
				{
					token = tokenList[j++].number_value;
				}
				else
				{
					error(5);
				}
			} while (token == identsym);
		}
		while (token == procsym)
		{
			token = tokenList[j++].number_value;
			if(token == identsym)
			{
				enter(3, &tx, &dx, lev, table);
				token = tokenList[j++].number_value;
			}
			else
			{
				error(4);
			}
			if(token == semicolonsym)
			{
				token = tokenList[j++].number_value;
			}
			else
			{
				error(5);
			}
			block(lev+1, tx, table, tokenList, code);
			if(token == semicolonsym)
			{
				token = tokenList[j++].number_value;
			}
			else
			{
				error(5);
			}
		}
	} while ((token == constsym) || (token == varsym) || (token == procsym));

	code[table[tx0].addr].m = codeidx;
	table[tx0].addr = codeidx;
	codeidx0 = codeidx;
	emit(6, 0, dx, code);
	statement(lev, &tx, tokenList, code, table);
	emit(2, 0, 0, code);
}

void constdeclaration(int lev, int *ptx, int *pdx, token_struct *tokenList, symbol *table)
{
	if(token == identsym)
	{
		token = tokenList[j++].number_value;
		if((token == eqsym) || (token == becomessym))
		{
			if(token == becomessym)
			{
				error(1);
			}
			token = tokenList[j++].number_value;
			if(token == numbersym)
			{
				enter(1, ptx, pdx, lev, table);
				token = tokenList[j++].number_value;
			}
		}
	}
}

void vardeclaration(int lev, int *ptx, int *pdx, token_struct *tokenList, symbol *table)
{
	if(token == identsym)
	{
		enter(2, ptx, pdx, lev, table);
		token = tokenList[j++].number_value;
	}
	else
	{
		error(4);
	}
}

void statement(int lev, int *ptx, token_struct *tokenList, instruction *code, symbol *table)
{
	int i, codeidx1, codeidx2;

	if(token == identsym)
	{
		i = position(id, ptx, table, lev);
		if(i == 0)
		{
			error(11);
		}
		else if(table[i].kind != 2)
		{
			error(12);
			i = 0;
		}
		token = tokenList[j++].number_value;
		if(token == becomessym)
		{
			token = tokenList[j++].number_value;
		}
		else
		{
			error(13);
		}
		expression(lev, ptx, tokenList, code, table);
		if(i != 0)
		{
			emit(4, lev - table[i].level, table[i].addr, code);
		}
	}
	else if(token == callsym)
	{
		token = tokenList[j++].number_value;
		if(token != identsym)
		{
			error(14);
		}
		else
		{
			i = position(id, ptx, table, lev);
			if(token != identsym)
			{
				error(14);
			}
			else
			{
				i = position(id, ptx, table, lev);
				if(i == 0)
				{
					error(11);
				}
				else if(table[i].kind == 3)
				{
					emit(5, lev - table[i].level, table[i].addr, code);
				}
				else
				{
					error(15);
				}
				token = tokenList[j++].number_value;
			}
		}
	}

	else if (token == ifsym)
	{
		token = tokenList[j++].number_value;
		condition(lev, ptx, tokenList, code, table);
		if(token == thensym)
		{
			token = tokenList[j++].number_value;
		}
		else
		{
			error(16);
		}

		codeidx1 = codeidx;
		emit(8, 0, 0, code);
		statement(lev, ptx, tokenList, code, table);

		if(token == elsesym)
		{
			token = tokenList[j++].number_value;
			code[codeidx1].m = codeidx+1;
			emit(7, 0, 0, code);
			statement(lev, ptx, tokenList, code, table);
		}
		code[codeidx1].m = codeidx;
	}
	else if(token == beginsym)
	{
		token = tokenList[j++].number_value;
		statement(lev, ptx, tokenList, code, table);
		while (token == semicolonsym)
		{
			token = tokenList[j++].number_value;
			statement(lev, ptx, tokenList, code, table);
		}

		if(token == endsym)
		{
			token = tokenList[j++].number_value;
		}
		else
		{
			error(17);
		}
	}
	else if(token == whilesym)
	{
		codeidx1 = codeidx;
		token = tokenList[j++].number_value;
		condition(lev, ptx, tokenList, code, table);
		codeidx2 = codeidx;
		emit(8, 0, 0, code);
		if(token == dosym)
		{
			token = tokenList[j++].number_value;
		}
		else
		{
			error(18);
		}
		statement(lev, ptx, tokenList, code, table);
		emit(7, 0, codeidx1, code);
		code[codeidx2].m = codeidx;
	}
	else if(token == writesym)
	{
		token = tokenList[j++].number_value;
		expression(lev, ptx, tokenList, code, table);
		emit(9, 0, 1, code);
	}
	else if(token == readsym)
	{
		token = tokenList[j++].number_value;
		emit(10, 0, 2, code);
		i = position(id, ptx, table, lev);
		if(i == 0)
		{
			error(11);
		}
		else if(table[i].kind!=2)
		{
			error(12);
			i = 0;
		}
		if(i != 0)
		{
			emit(4, lev - table[i].level, table[i].addr, code);
		}
		token = tokenList[j++].number_value;
	}
}

void condition(int lev, int *ptx, token_struct *tokenList, instruction *code, symbol *table)
{
	int relationSwitch;
	
	if(token == oddsym)
	{
		token = tokenList[j++].number_value;
		expression(lev, ptx, tokenList, code, table);
		emit(2, 0, 6, code);
	}
	else
	{
		expression(lev, ptx, tokenList, code, table);
		if((token != eqsym) && (token != neqsym) && (token != lessym) && (token != leqsym) && (token != gtrsym) && (token != geqsym))
		{
			error(20);
		}
		else
		{
			relationSwitch = token;
			token = tokenList[j++].number_value;
			expression(lev, ptx, tokenList, code, table);
			switch(relationSwitch)
			{
				case 9:
					emit(2, 0, 8, code);
					break;
				case 10:
					emit(2, 0, 9, code);
					break;
				case 11:
					emit(2, 0, 10, code);
					break;
				case 12:
					emit(2, 0, 11, code);
					break;
				case 13:
					emit(2, 0, 12, code);
					break;
				case 14:
					emit(2, 0, 13, code);
					break;
			}
		}
	}
}

void expression(int lev, int *ptx, token_struct *tokenList, instruction *code, symbol *table)
{
	int addop;
	if(token == plussym || token == minussym)
	{
		addop = token;
		token = tokenList[j++].number_value;
		term(lev, ptx, tokenList, code, table);
		if(addop == minussym)
		{
			emit(2, 0, 1, code);
		}
	}
	else
	{
		term(lev, ptx, tokenList, code, table);
	}
	while (token == plussym || token == minussym)
	{
		addop = token;
		token = tokenList[j++].number_value;
		term(lev, ptx, tokenList, code, table);
		if(addop == plussym)
		{
			emit(2, 0, 2, code);
		}
		else
		{
			emit(2, 0, 3, code);
		}
	}
}

void term(int lev, int *ptx, token_struct *tokenList, instruction *code, symbol * table)
{
	int mulop;
	factor(lev, ptx, table, tokenList, code);
	while(token == multsym || token == slashsym)
	{
		mulop = token;
		token = tokenList[j++].number_value;
		factor(lev, ptx, table, tokenList, code);
		if(mulop == multsym)
		{
			emit(2, 0, 4, code);
		}
		else
		{
			emit(2, 0, 5, code);
		}
	}
}

void factor(int lev, int *ptx, symbol *table, token_struct *tokenList, instruction * code)
{
	int i, level, adr, val;
	
	while((token == identsym) || (token == numbersym) || (token == lparentsym))
	{
		if(token == identsym)
		{
			i = position(id, ptx, table, lev);
			if(i == 0)
			{
				error(11);
			}
			else
			{
				kind = table[i].kind;
				level = table[i].level;
				adr = table[i].addr;
				val = table[i].val;
				if(kind == 1)
				{
					emit(1, 0, val, code);
				}
				else if(kind == 2)
				{
					emit(3, lev - level, adr, code);
				}
				else
				{
					error(21);
				}
			}
			token = tokenList[j++].number_value;
		}
		else if(token == numbersym)
		{
			if(num > 2047)
			{
				error(25);
				num = 0;
			}
			emit(1, 0, num, code);
			token = tokenList[j++].number_value;
		}
		else if(token == lparentsym)
		{
			token = tokenList[j++].number_value;
			expression(lev, ptx, tokenList, code, table);
			if(token == rparentsym)
			{
				token = tokenList[j++].number_value;
			}
			else
			{
				error(22);
			}
		}
	}
}

void emit(int op, int l, int m, instruction* code)
{
    if(codeidx > CODE_SIZE)
        printf("Program too long! codeidx > CODE_SIZE\n");
    else {
        code[codeidx].opcode = op;
        code[codeidx].l = l;
        code[codeidx].m = m;
        codeidx++;
    }
}

void enter(int k, int *ptx, int *pdx, int lev, symbol* table)
{
    char *id1;
    int ii, len;

    (*ptx)++;
    id1=id;
    len=strlen(id);
    for (ii=0;ii<=len;ii++)
	{
        table[*ptx].name[ii]=*id1;
        id1++;
    }
    table[*ptx].kind=k;
    if (k==1)
	{
        table[*ptx].val=num;
    }
    else if (k==2)
	{
        table[*ptx].level=lev;
        table[*ptx].addr=*pdx;
        (*pdx)++;
    }
    else
	{
        table[*ptx].level=lev;
    }
}

void error(int errorCase)
{
    errorCount++;
    
    switch (errorCase)
	{
        case 1:
            printf("constants must be assigned with =\n");
            break;
        case 2:
            printf("= must be followed by a number.\n");
            break;
        case 3:
            printf("assignment statements must use :=\n");
            break;
        case 4:
            printf("const, var, and read keywords must be followed by identifier\n");
            break;
        case 5:
            printf("constant and variable declarations must be followed by a semicolon\n");
            break;
        case 9:
            printf("program must end with period\n");
            break;
        case 10:
            printf("constant and variable declarations must be followed by a semicolon\n");
            break;
        case 11:
            printf("undeclared symbol\n");
            break;
        case 12:
            printf("only variable values may be altered\n");
            break;
        case 13:
            printf("assignment statements must use :=\n");
            break;
        case 14:
            printf("call must be followed by an identifier\n");
            break;
        case 16:
            printf("if must be followed by then\n");
            break;
        case 17:
            printf("begin must be followed by end\n");
            break;
        case 18:
            printf("while must be followed by do\n");
            break;
        case 20:
            printf("condition must contain comparison operator\n");
            break;
        case 21:
            printf("arithmetic equations must contain operands, parentheses, numbers, or symbols\n");
            break;
        case 22:
            printf("right parenthesis must follow left parenthesis\n");
            break;
        default:
            break;
    }
}

int position(char *id, int *ptx, symbol* table, int lev)
{    
    int s;
    s=*ptx;
    int currentS;
    int differenceCount = 0;

    while(s!=0) {
        if (strcmp(table[s].name,id) == 0) {
            if(table[s].level<=lev)
			{   
                if (differenceCount!=0)
				{
                    previousDifference = difference;
                }
                
                difference = lev-table[s].level;
                
                if(differenceCount==0)
				{
                    currentS=s;
                }
                
                if (difference<previousDifference)
				{
                    currentS=s;
                }
                differenceCount++;
            }
        }
        s--;
    }

    return currentS;
}