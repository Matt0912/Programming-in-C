
%%
"the"	{yylval=1; return(ARTICLE);}
"a"	{yylval=2; return(ARTICLE);}
"dog"	{yylval=3; return(NOUN);}
"cat"	{yylval=4; return(NOUN);}
"man"	{yylval=5; return(NOUN);}
"woman"	{yylval=6; return(NOUN);}
"robot"	{yylval=7; return(NOUN);}
"bit"	{yylval=8; return(VERB);}
"kicked"	{yylval=9; return(VERB);}
"stroked"	{yylval=10; return(VERB);}
"two furry dice"	{yylval=11; return(PHRASE);}
"the robot"	{yylval=0; return (PHRASE);}
"&"     {return('&');}
"\n"    {return('\n');}
%%
