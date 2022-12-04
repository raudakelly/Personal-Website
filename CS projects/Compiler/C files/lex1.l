%{
//#include "yaccExample.h"
#include "IOMngr.h"
#include "y.tab.h"

#define YY_INPUT(buf,result,max_size) \
	{ int c = getNextSourceChar(); \
          result = (c == EOF) ? YY_NULL : (buf[0] = c, 1); \
        }

%}


letter [A-Za-z]
digit [0-9]

%%
if					{return IF;}
else					{return Else;}
while					{return While;}
for					{return For;}
int					{return Int;}
print					{return Write;}
read					{return Read;}
printline				{return Printline;}
printspaces				{return Printspaces;}
{letter}({letter}|{digit})*		{return Ident;}
{digit}{digit}*				{return IntLit;}
\=\=					{return EQ;}
\<\=					{return LTE;}
\>\=					{return GTE;}
\!\=					{return NEQ;}
\&\&					{return AND;}
\|\|					{return OR;}
\,					{return ',';}
\!					{return '!';}
\<					{return '<';}
\>					{return '>';}
\=					{return '=';}
\+					{return '+';}
\-					{return '-';}
\*					{return '*';}
\/					{return '/';}
\%					{return '%';}
\^					{return '^';}
\;					{return ';';}
\{					{return '{';}
\}					{return '}';}
\(					{return '(';}
\)					{return ')';}
\[					{return '[';}
\]					{return ']';}

[ ]				{}
\t				{}
\r				{}
\n				{}

.				{writeIndicator(getCurrentColumnNum());
				   writeMessage("Illegal Character in lex");}

%%

	
int yywrap () {
    return 1;
}
			
	
