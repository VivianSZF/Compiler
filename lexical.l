%{
#include <stdio.h>
%}

%option yylineno
DEC		    0|([1-9][0-9]*)
OCT		    0[0-7]+
HEX		    0[xX][0-9a-fA-F]+
INT		    {DEC}|{OCT}|{HEX}
nfloat		[0-9]+\.[0-9]+
sfloat		(([0-9]*\.[0-9]+)|([0-9]+\.))[eE][+-]?[0-9]+
FLOAT		{nfloat}|{sfloat}
ID		    [_a-zA-Z][_a-zA-Z0-9]*
RELOP		">"|"<"|">="|"<="|"=="|"!="
LINECOMMENT	\/\/.*\n
BLOCKCOMMENT	\/\*
TYPE		int|float
WS		    [ \t]+

%%
{INT}		{printf("intnum\n");}
{FLOAT}		{printf("floatnum\n");}
";"		    {printf("sem\n");}
","		    {printf("comma\n");}
"="		    {printf("assignop\n");}
{RELOP}		{printf("relop\n");}
"+"		    {printf("plus\n");}
"-"		    {printf("minus\n");}
"*"		    {printf("star\n");}
"/"		    {printf("devide\n");}
"&&"		{printf("and\n");}
"||"		{printf("or\n");}
"."		    {printf("dot\n");}
"!"		    {printf("not\n");}
{TYPE}		{printf("type\n");}
"("		    {printf("lp\n");}
")"		    {printf("rp\n");}
"["		    {printf("lb\n");}
"]"		    {printf("rb\n");}
"{"		    {printf("lc\n");}
"}"		    {printf("rc\n");}
"struct"	{printf("struct\n");}
"return"	{printf("return\n");}
"if"		{printf("if\n");}
"else"		{printf("else\n");}
"while"		{printf("while\n");}
{ID}        {printf("id\n");}
\n		    {printf("newline\n");}
{WS}		{printf("ws\n");}
{LINECOMMENT}	{printf("line comment\n");}
{BLOCKCOMMENT}	{printf("block comment\n");}
.		{
		    printf("Error type A at Line %d: Mysterious characters \'%s\'\n",yylineno,yytext);
		}
%%
