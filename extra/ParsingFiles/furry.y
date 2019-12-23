%{
# include <stdio.h>
# include <ctype.h>

char *tokens[] = {"the robot", "the", "a", "dog", "cat", "man", "woman", \
"robot", "bit", "kicked", "stroked", "two furry dice"};

int dummy;
void printarg(x,y) 
	int x,y;
{
if (x>0) {printf("%c",(char)x);y=0;}
}
%}

%start list

%token VERB ARTICLE NOUN PHRASE 

%%
list	: /*empty*/
	|list sentence '\n'
		{printf("\nNext furry sentence please!\n\n");} 
	|list error '\n'
		{ yyerror("Oh dear  -- syntax error. Baling out now");exit(1);}
	;


sentence	:	 '&'
			{printf("End of input-- Bye!\n");exit(1);}
	|	 subj verb obj
	;

subj	:	ARTICLE	NOUN
				{printf("Rule 2: %s %s", tokens[$1],tokens[$2]);} 
	|	PHRASE
				{printf("Rule 3: %s ", tokens[$1]);}
	;


verb : 	 VERB	

			{printf("Rule 4:  %s ", tokens[$1]);}
	;


obj	:	 ARTICLE  NOUN
			{printf("Rule 5: %s %s\n", tokens[$1], tokens[$2]);}
		
	|	PHRASE
			{printf("Rule 6: %s\n", tokens[$1]);}

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
