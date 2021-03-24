
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "compiler.h"

const char *reserved_words[] = {"const", "var", "procedure", "call", "if", "then", "else", "while", "do", "read", "write", "odd", "return", "begin", "end"};

const char special_symbols[] = {'+', '-', '*', '/', '(', ')', '=', ',', '.', '<', '>', ';', ':', '%'};

token_struct* lex_analyze(char *inputfile, int print)
{
	token_struct *lex_list = malloc(500 * sizeof(token_struct));
	int lex_list_index = 0;
    int c;
    int i, j = 0, k = 0, z = 0;
    int err = 0;
    int comment = 0;
    int read_next = 0;

	if(print == 1)
	{
		printf("Lexime Table:\n");
		printf("%12s  %s\n", "lexeme", "token type");
	}
    
    while (z < input_size)
    {
		c = inputfile[z++];
		if (i >= input_size)
			break;

        if(isspace(c))
        {
            c = inputfile[z++];
			if (i >= input_size)
				break;
            read_next = 0;
            continue;
        }

        if(isalpha(c))
        {
            char character_string[12];
            memset(character_string, 0, sizeof(character_string));

            int index = 0;
            character_string[index] = c;

            index++;
            read_next = 1;

            while(isalpha(c = inputfile[z++]) || isdigit(c))
            {
                if(index > 10)
                {
                    printf("Error: Identifier names cannot exceed 11 characters\n");

                    while (isalpha(c = inputfile[z++]) || isdigit(c)){                    }
                    err = 1;

                    break;
                }
                character_string[index] = c;
                index++;
            }

            if (err == 1)
            {
                err = 0;
                continue;
            }

            int reserved_switch = -1;

            for(i = 0; i < 15; i++)
            {
                if(strcmp(character_string, reserved_words[i]) == 0)
                {
                    reserved_switch = i;
                }
            }
            switch(reserved_switch)
            {
                // const
                case 0:
                    lex_list[lex_list_index].tokenID = constsym;
                    if(print == 1)
                    	printf("%12s  %d\n", character_string, lex_list[lex_list_index].tokenID);
                    break;
                // var
                case 1:
                    lex_list[lex_list_index].tokenID = varsym;
                    if(print == 1)
                    	printf("%12s  %d\n", character_string, lex_list[lex_list_index].tokenID);
                    break;
                // procedure
                case 2:
                    lex_list[lex_list_index].tokenID = procsym;
                    if(print == 1)
                    	printf("%12s  %d\n", character_string, lex_list[lex_list_index].tokenID);
                    break;
                // call
                case 3:
                    lex_list[lex_list_index].tokenID = callsym;
                    if(print == 1)
                    	printf("%12s  %d\n", character_string, lex_list[lex_list_index].tokenID);
                    break;
                // if
                case 4:
                    lex_list[lex_list_index].tokenID = ifsym;
                    if(print == 1)
                    	printf("%12s  %d\n", character_string, lex_list[lex_list_index].tokenID);
                    break;
                // then
                case 5:
                    lex_list[lex_list_index].tokenID = thensym;
                    if(print == 1)
                    	printf("%12s  %d\n", character_string, lex_list[lex_list_index].tokenID);
                    break;
                // else
                case 6:
                    lex_list[lex_list_index].tokenID = elsesym;
                    if(print == 1)
                    	printf("%12s  %d\n", character_string, lex_list[lex_list_index].tokenID);
                    break;
                // while
                case 7:
                    lex_list[lex_list_index].tokenID = whilesym;
                    if(print == 1)
                    	printf("%12s  %d\n", character_string, lex_list[lex_list_index].tokenID);
                    break;
                // do
                case 8:
                    lex_list[lex_list_index].tokenID = dosym;
                    if(print == 1)
                    	printf("%12s  %d\n", character_string, lex_list[lex_list_index].tokenID);
                    break;
                // read
                case 9:
                    lex_list[lex_list_index].tokenID = readsym;
                    if(print == 1)
                    	printf("%12s  %d\n", character_string, lex_list[lex_list_index].tokenID);
                    break;
                // write
                case 10:
                    lex_list[lex_list_index].tokenID = writesym;
                    if(print == 1)
                    	printf("%12s  %d\n", character_string, lex_list[lex_list_index].tokenID);
                    break;
                // odd
                case 11:
                    lex_list[lex_list_index].tokenID = oddsym;
                    if(print == 1)
                    	printf("%12s  %d\n", character_string, lex_list[lex_list_index].tokenID);
                    break;
                // return
                case 12:
                    lex_list[lex_list_index].tokenID = returnsym;
                    if(print == 1)
                    	printf("%12s  %d\n", character_string, lex_list[lex_list_index].tokenID);
                    break;
                // begin
                case 13:
                    lex_list[lex_list_index].tokenID = beginsym;
                    if(print == 1)
                    	printf("%12s  %d\n", character_string, lex_list[lex_list_index].tokenID);
                    break;
                // end
                case 14:
                    lex_list[lex_list_index].tokenID = endsym;
                    if(print == 1)
                    	printf("%12s  %d\n", character_string, lex_list[lex_list_index].tokenID);
                    break;

                default:
                    lex_list[lex_list_index].tokenID = identsym;
                    if(print == 1)
                    	printf("%12s  %d\n", character_string, lex_list[lex_list_index].tokenID);
                    strcpy(lex_list[lex_list_index].name, character_string);
                    break;
            }
            lex_list_index++;
        }

        else if(isdigit(c))
        {
            int number = c - '0';
            int d;
            int num_digits = 1;

            read_next = 1;

            while(isdigit(c=inputfile[z++]))
            {
                if(num_digits > 4)
                {
                    printf("Error: Numbers cannot exceed 5 digits\n");

                    while(isdigit(c=inputfile[z++])){}

                    err = 1;
                    break;
                }
                d = c - '0';
                number = 10 * number + d;
                num_digits++;
            }

            if(isalpha(c))
            {
                if(err != 1)
                {
                    printf("Error: Identifiers cannot begin with a digit\n");
                }
                while(isalpha(c = inputfile[z++]) || isdigit(c)){}
                continue;
            }

            if(err == 1)
            {
                err = 0;
                continue;
            }

            lex_list[lex_list_index].tokenID = numbersym;
            lex_list[lex_list_index].number_value = number;
            lex_list_index++;
        }

        else
        {
            read_next = 0;
            int symbol = -1;

            for(i = 0; i < 14; i++)
            {
                if(c == special_symbols[i])
                {
                    symbol = i;
                }
            }

            switch(symbol)
            {
                // +
                case 0:
                    lex_list[lex_list_index].tokenID = plussym;
                    if(print == 1)
                    	printf("%12c  %d\n", c, lex_list[lex_list_index].tokenID);
                    lex_list_index++;
                    break;
                // -
                case 1:
                    lex_list[lex_list_index].tokenID = minussym;
                    if(print == 1)
                    	printf("%12c  %d\n", c, lex_list[lex_list_index].tokenID);
                    lex_list_index++;
                    break;
                // *
                case 2:
                    lex_list[lex_list_index].tokenID = multsym;
                    if(print == 1)
                    	printf("%12c  %d\n", c, lex_list[lex_list_index].tokenID);
                    lex_list_index++;
                    break;
                // Comments or /
                case 3:
                    c = inputfile[z++];
					if (i >= input_size)
						break;
                    read_next = 1;
                    if(c == '*')
                    {
                        comment = 1;
                        read_next = 0;
                        c = inputfile[z++];
						if (i >= input_size)
							break;
                        while(comment == 1)
                        {
                            if(c == '*')
                            {
                                c = inputfile[z++];
								if (i >= input_size)
									break;
                                if(c == '/')
                                {
                                    comment = 0;
                                }
                            }
                            else
                            {
                                c = inputfile[z++];
								if (i >= input_size)
									break;
                            }
                        }
                    }
                    else
                    {
                        lex_list[lex_list_index].tokenID = slashsym;
                        if(print == 1)
                    		printf("%12c  %d\n", '/', lex_list[lex_list_index].tokenID);
                        lex_list_index++;
                    }
                    break;
                // (
                case 4:
                    lex_list[lex_list_index].tokenID = lparentsym;
                    if(print == 1)
                    	printf("%12c  %d\n", c, lex_list[lex_list_index].tokenID);
                    lex_list_index++;
                    break;
                // )
                case 5:
                    lex_list[lex_list_index].tokenID = rparentsym;
                    if(print == 1)
                    	printf("%12c  %d\n", c, lex_list[lex_list_index].tokenID);
                    lex_list_index++;
                    break;
                // =
                case 6:
                    lex_list[lex_list_index].tokenID = eqsym;
                    if(print == 1)
                    	printf("%12c  %d\n", c, lex_list[lex_list_index].tokenID);
                    lex_list_index++;
                    break;
                // ,
                case 7:
                    lex_list[lex_list_index].tokenID = commasym;
                    if(print == 1)
                    	printf("%12c  %d\n", c, lex_list[lex_list_index].tokenID);
                    lex_list_index++;
                    break;
                // .
                case 8:
                    lex_list[lex_list_index].tokenID = periodsym;
                    if(print == 1)
                    	printf("%12c  %d\n", c, lex_list[lex_list_index].tokenID);
                    lex_list_index++;
                    break;
                // <
                case 9:
                    c = inputfile[z++];
					if (i >= input_size)
						break;
                    read_next = 1;
                    if(c == '>')
                    {
                        lex_list[lex_list_index].tokenID = neqsym;
                        if(print == 1)
                    		printf("%12s  %d\n", "<>", lex_list[lex_list_index].tokenID);
                        read_next = 0;
                    }
                    else if(c == '=')
                    {
                        lex_list[lex_list_index].tokenID = leqsym;
                        if(print == 1)
                    		printf("%12s  %d\n", "<=", lex_list[lex_list_index].tokenID);
                        read_next = 0;
                    }
                    else
                    {
                        lex_list[lex_list_index].tokenID = lessym;
                        if(print == 1)
                    		printf("%12c  %d\n", '<', lex_list[lex_list_index].tokenID);
                    }
                    
                    
                    lex_list_index++;
                    break;
                // >
                case 10:
                    c = inputfile[z++];
					if (i >= input_size)
						break;
                    read_next = 1;
                    if(c == '=')
                    {
                        lex_list[lex_list_index].tokenID = geqsym;
                        if(print == 1)
                    		printf("%12s  %d\n", ">=", lex_list[lex_list_index].tokenID);
                        read_next = 0;
                    }
                    else
                    {
                        lex_list[lex_list_index].tokenID = gtrsym;
                        if(print == 1)
                    		printf("%12c  %d\n", '>', lex_list[lex_list_index].tokenID);
                    }
                    lex_list_index++;
                    break;
                // ;
                case 11:
                    lex_list[lex_list_index].tokenID = semicolonsym;
                    if(print == 1)
                    		printf("%12c  %d\n", c, lex_list[lex_list_index].tokenID);
                    lex_list_index++;
                    break;
                // :
                case 12:
                    c = inputfile[z++];
					if (i >= input_size)
						break;
                    if(c == '=')
                    {
                        lex_list[lex_list_index].tokenID = becomessym;
                        if(print == 1)
                    		printf("%12s  %d\n", ":=", lex_list[lex_list_index].tokenID);
                        lex_list_index++;
                    }
                    else
                    {
                        if(print == 1)
                    		printf("Error: Invalid Symbol case 12\n");
                    }
                    
                    break;
                // %
                case 13:
                    lex_list[lex_list_index].tokenID = modsym;
					if(print == 1)
                    	printf("%12c  %d\n", c, lex_list[lex_list_index].tokenID);
                    lex_list_index++;
                    break;

                default:
                    printf("Error: Invalid Symbol default\n");
            }
        }

        if(read_next == 0)
        {
            c = inputfile[z++];
			if (i >= input_size)
				break;
        }
    }

	if(print == 1)
	{
		printf("Token List:\n");
		for (i = 0; i < lex_list_index; i++)
		{
			printf("%d ", lex_list[i].tokenID);
			if(lex_list[i].tokenID == 2)
			{
				printf("%s ", lex_list[i].name);
			}
			if(lex_list[i].tokenID == 3)
			{
				printf("%d ", lex_list[i].number_value);
			}
		}
		printf("\n");
	}
	printf("lex is analyzed\n");
	return lex_list;
}