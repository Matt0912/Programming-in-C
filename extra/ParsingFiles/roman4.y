%{
# include <stdio.h>
# include <ctype.h>
%}

%start list

%token NUMBER

%%
list	: /*empty*/
	|list stat '\n'
		{printf(" %d\n", $2);}
	|list error '\n'
		{ yyerror("Oh dear  -- syntax error. Baling out now\n");exit(1);}
	;


stat	:	 '&'
			{printf("End of input-- Bye!\n");exit(0);}
	|	 stat '+' term
			{$$ =  $1 + $3;}
	|	 stat '-' term
			{$$ =  $1 - $3;}
	|	 term
			{$$ = $1;}
	;


term	:	 term '*' primary
			{$$ =  $1 * $3;}
	|	 term '/' primary
			{$$ =  $1 / $3;}
	|	 primary
			{$$ = $1;}
	;

primary :	'(' stat ')'
			{$$ = $2;}
	|	NUMBER
			{$$ = $1;}
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
