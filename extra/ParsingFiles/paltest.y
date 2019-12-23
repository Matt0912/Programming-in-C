%{
# include <stdio.h>
# include <ctype.h>
%}

%start list

%%
list    : /*empty*/
        |list S '\n'
        |list error '\n'
                { yyerror("Oh dear  -- syntax error. Baling out now\n"); exit(1)
;}
        ;


S       :'&'    /* & marks end of input */
                        {printf("End of input-- Bye! ");exit(0);}

S	: 'a' T 'c' 
		{printf("Parsed OK!\n");}
	;
T	: 'b' T 'b'	{printf("Rule 2. Bingo!\n");} 
	| 'X'           {printf("Rule 3. Yippeee!!\n");}
	|  'b'
	;
%%
#include "lex.yy.c"
main() {
	return (yyparse() );

	}

yyerror(s)
	char *s;
{
	fprintf (stderr, "%s\n", s);

}
