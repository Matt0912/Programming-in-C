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
"+"	{op = '+';}
"&"	{return('&');}/*end of input*/
@      {return('\n');}
\n	{ if (sum != 0)
		      { yylval=sum;
			printf ("    converts to %d\n", sum);
			sum =0;
			/* need to prepare value of previous roman number when
			 * \n encountered.  But push back \n as @ onto input
			 *  queue using `unput'  so that it gets re-read and
			 *  recognised. This then returns \n to YACC as marker
			 *  for end of statement.
			 */
			unput('@');
			return(NUMBER); /* tell YACC we've found a NUMBER*/
		       }
                else
                      {
                        unput('@');
                        return (op);
                      }
              }
[ \t]	   { /* prepare value of roman no. just encountered when space or
	      * tab is recognised. If sum = 0 then an operator has been seen
	      * so return it to YACC as its own ASCII code.
	      */
	     if (sum != 0)
		      { yylval=sum;
			printf ("    converts to %d\n", sum);
			sum =0;
			return(NUMBER);
		       }
		else return (op);
	      }
%%
