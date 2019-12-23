%{
# include <stdio.h>
# include <ctype.h>

char *tokens[] = {"the robot", "the", "a", "dog", "cat", "man", "woman", \
"robot", "bit", "kicked", "stroked", "two furry dice"};
char subjstring[100],verbstring[100],objstring[100];

void initparts ()
{ /* initialises the three strings holding pointers into 
various accumulated components of a yoda sentence.  */
int i;
for (i=0 ; i < 100; i++)
	subjstring[i] = verbstring[i] = objstring[i] = '0';
}

int dummy;
void printarg(x,y) 
	int x,y;
{
if (x>0) {printf("%c",(char)x);y=0;}
}
%}

%start list

%token ID VERB ARTICLE NOUN PHRASE 

%%
list	: /*empty*/  {initparts();}
	|list sentence '\n'
		{ initparts(); /* clear out buffers for next sentence */
		  printf("\nNext furry sentence please!\n\n");
		} 
		
	|list error '\n'
		{ yyerror("Oh dear  -- syntax error. Baling out now");exit(1);}
	;


sentence	:	 '&'
			{printf("End of input-- Bye!\n");exit(1);}
	|	 subj verb obj
			{ printf("\n\nYoda says: %s %s %s \n", objstring,\ 
			subjstring, verbstring); 
			}
	;

subj	:	ARTICLE	NOUN
				{
				  printf("Rule 2: %s %s", tokens[$1],tokens[$2]);
				  strcpy(subjstring, tokens[$1]);
				  strcat(subjstring, " ");
				  strcat(subjstring, tokens[$2]);
				} 
	|	PHRASE
				{printf("Rule 3: %s ", tokens[$1]);
				 strcpy(subjstring, tokens[$1]);
				} 	
	;


verb : 	 VERB	

			{printf("Rule 4:  %s ", tokens[$1]);
			 strcpy(verbstring, tokens[$1]); 	
			}
	;


obj	:	 ARTICLE  NOUN
			{printf("Rule 5: %s %s\n", tokens[$1], tokens[$2]);
			  strcpy(objstring, tokens[$1]);
			  strcat(objstring, " ");
			  strcat(objstring, tokens[$2]);
 
		}
		
	|	PHRASE
			{ printf("Rule 6: %s\n", tokens[$1]);
			  strcpy(objstring, tokens[$1]);

		}	
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
