%{
#include <stdio.h>
#include "syntax.tab.h"
#include "treeop.h"

int yycolumn=1;
#define YY_USER_ACTION \
    yylloc.first_line=yylloc.last_line=yylineno; \
    yylloc.first_column=yycolumn; \
    yylloc.last_column=yycolumn+yyleng-1; \
    yycolumn+=yyleng;


%}

%option yylineno
DEC		    0|([1-9][0-9]*)
WOCT		    0[0-9]+
WHEX		    0[xX][0-9a-zA-Z]+
INT		    {DEC}|{WOCT}|{WHEX}
nfloat		[0-9]+\.[0-9]+
sfloat		(([0-9]*\.[0-9]+)|([0-9]+\.))[eE][+-]?[0-9]+
wfloat      (([0-9]*\.[0-9]+)|([0-9]+\.))[eE]
FLOAT		{nfloat}|{sfloat}|{wfloat}
ID		    [_a-zA-Z][_a-zA-Z0-9]*
RELOP		">"|"<"|">="|"<="|"=="|"!="
LINECOMMENT	\/\/.*\n
BLOCKCOMMENT	\/\*
TYPE		int|float
WS		    [ \t]+

%%
{INT}		{yylval.node=create_node(TINT,"INT",yylineno,yytext);return INT;}
{FLOAT}		{yylval.node=create_node(TFLOAT,"FLOAT",yylineno,yytext);return FLOAT;}
";"		    {yylval.node=create_node(TUN,"SEMI",yylineno,NULL);return SEMI;}
","		    {yylval.node=create_node(TUN,"COMMA",yylineno,NULL);return COMMA;}
"="		    {yylval.node=create_node(TUN,"ASSIGNOP",yylineno,NULL);return ASSIGNOP;}
{RELOP}		{yylval.node=create_node(TUN,"RELOP",yylineno,NULL);return RELOP;}
"+"		    {yylval.node=create_node(TUN,"PLUS",yylineno,NULL);return PLUS;}
"-"		    {yylval.node=create_node(TUN,"MINUS",yylineno,NULL);return MINUS;}
"*"		    {yylval.node=create_node(TUN,"STAR",yylineno,NULL);return STAR;}
"/"		    {yylval.node=create_node(TUN,"DIV",yylineno,NULL);return DIV;}
"&&"		{yylval.node=create_node(TUN,"AND",yylineno,NULL);return AND;}
"||"		{yylval.node=create_node(TUN,"OR",yylineno,NULL);return OR;}
"."		    {yylval.node=create_node(TUN,"DOT",yylineno,NULL);return DOT;}
"!"		    {yylval.node=create_node(TUN,"NOT",yylineno,NULL);return NOT;}
{TYPE}		{yylval.node=create_node(TTYPE,"TYPE",yylineno,yytext);return TYPE;}
"("		    {yylval.node=create_node(TUN,"LP",yylineno,NULL);return LP;}
")"		    {yylval.node=create_node(TUN,"RP",yylineno,NULL);return RP;}
"["		    {yylval.node=create_node(TUN,"LB",yylineno,NULL);return LB;}
"]"		    {yylval.node=create_node(TUN,"RB",yylineno,NULL);return RB;}
"{"		    {yylval.node=create_node(TUN,"LC",yylineno,NULL);return LC;}
"}"		    {yylval.node=create_node(TUN,"RC",yylineno,NULL);return RC;}
"struct"	{yylval.node=create_node(TUN,"STRUCT",yylineno,NULL);return STRUCT;}
"return"	{yylval.node=create_node(TUN,"RETURN",yylineno,NULL);return RETURN;}
"if"		{yylval.node=create_node(TUN,"IF",yylineno,NULL);return IF;}
"else"		{yylval.node=create_node(TUN,"ELSE",yylineno,NULL);return ELSE;}
"while"		{yylval.node=create_node(TUN,"WHILE",yylineno,NULL);return WHILE;}
{ID}        {yylval.node=create_node(TID,"ID",yylineno,yytext);return ID;}
\n		    {yycolumn=1;}
{WS}		{;}
{LINECOMMENT}	{;}
{BLOCKCOMMENT}	{printf("block comment\n");}
.		{
            errornot=1;
   		    printf("Error type A at Line %d: Mysterious characters \"%s\".\n",yylineno,yytext);
		}
%%
