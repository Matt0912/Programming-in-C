%%
a	{ECHO;printf("	 rule1\n");}
ab	{ECHO;printf("	 rule 2\n");}
a*b+	{ECHO;printf("	 rule3\n");}
(a|b)*abb	{ECHO;printf("	rule4\n");}
.*	{ECHO;printf("   not recognised\n");}
\n	{printf(" NEXT R.E. PLEASE\n");}
%%
