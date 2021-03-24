typedef enum
{
	modsym = 1, identsym, numbersym, plussym, minussym, 
	multsym, slashsym, oddsym, eqsym, neqsym, 
	lessym, leqsym, gtrsym, geqsym, lparentsym, 
	rparentsym, commasym, semicolonsym, periodsym, becomessym, 
	beginsym, endsym, ifsym, thensym, whilesym, 
	dosym, callsym,	constsym, varsym, procsym, 
	writesym, readsym, elsesym, returnsym
} token_type;

typedef struct {
    token_type tokenID;
    int number_value;
    char name[12];
} token_struct;

typedef struct instruction
{
	int opcode;
	char op[4];
	int l;
	int m;
	
} instruction;

typedef struct symbol
{
	int kind;
	char name[12];
	int val;
	int level;
	int addr;
} symbol;

int input_size;

token_struct* lex_analyze(char *inputfile, int print);
void execute(instruction *code, int print);
instruction *parse(token_struct *tokenList, int print);