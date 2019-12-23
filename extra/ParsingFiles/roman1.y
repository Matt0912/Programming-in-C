%{
# include <stdio.h>
# include <ctype.h>
%}

%start list

%token NUMBER

%%
list	: /*empty*/
	|list stat '\n'
	|list error '\n'
		{ yyerror("Oh dear  -- syntax error. Baling out now\n"); exit(1);}
	;


stat	:	 '&'	/* & marks end of input */
			{printf("End of input-- Bye! ");exit(0);}
	|	 NUMBER '+' NUMBER
			{printf(" Answer is %d\n", $1 + $3);}
	;
%%
#include "lex.yy.c"

main() {  /* This is the main program. All it does is to call the parser
	   * which yacc has created. You tinker with this at your peril!
	   */

	return (yyparse() );

	}

yyerror(s)
	char *s;

/* routine called when an error occurs.
 * Prints out the string that you supply
 */

{
	fprintf (stderr, "%s\n", s);

}
