	char op;
	int sum = 0;
%%
(cm|CM) {ECHO; sum = sum + 900;}
(cd|CD) {ECHO; sum = sum + 400;}
(xc|XC) {ECHO; sum = sum + 90;}
(xl|XL) {ECHO; sum = sum + 40;}
(ix|IX)  {ECHO;sum = sum + 9;}
(iv|IV)  {ECHO;sum = sum +4;}
(m|M)	 {ECHO;sum = sum + 1000;}
(d|D)	 {ECHO;sum = sum + 500;}
(c|C)	 {ECHO;sum = sum + 100;}
(l|L)	 {ECHO;sum = sum + 50;}
(x|X)	 {ECHO;sum = sum + 10;}
(v|V)	 {ECHO;sum = sum + 5;}
(i|I)	 {ECHO;sum = sum + 1;}
"+"	 {op = '+';printf(" + ");}
"-"	 {op = '-';printf(" - ");}
"*"	 {op = '*';printf(" * ");}
"/"	 {op = '/';printf(" / ");}
"("	 {op = '(';printf(" ( ");}
")"	 {op = ')';printf(" ) ");}
"&"	  {return('&');}
@      { return('\n');}
\n	{ printf (" = ");
          unput('@');
          if (sum != 0)
		      { yylval=sum;
			sum =0;
			return(NUMBER);
		       }
		else return (op);
	      }
[ \t]	   { if (sum != 0)
		      { yylval=sum;
			sum =0;
			return(NUMBER);
		       }
		else return (op);
	      }
%%
