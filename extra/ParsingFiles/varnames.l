%{
#include <stdio.h>
%}
%%
[0-9]+			{ECHO; printf("\tNUMBER\n");}
[A-Za-z][A-Za-z0-9]*	{ECHO; printf("\tVARNAME\n");}


