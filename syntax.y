%{
#include <stdio.h>
#include "lex.yy.c"
#include "treeop.h"

void yyerror(char* msg);
void printerror(char* msg, int lineno);
int errorlineno=-1;
int mark=-1;

%}

%union{
    struct Node* node;
}

%token <node> INT FLOAT ID SEMI COMMA ASSIGNOP RELOP PLUS MINUS STAR DIV OR AND DOT NOT
%token <node> TYPE LP RP LB RB LC RC STRUCT RETURN IF ELSE WHILE

%type <node> Program ExtDefList ExtDef Specifier ExtDecList VarDec
%type <node> StructSpecifier OptTag Tag FunDec VarList ParamDec CompSt StmtList Stmt
%type <node> Exp DefList Def DecList Dec Args

%right ASSIGNOP
%left PLUS MINUS
%left STAR DIV
%left LP RP LB RB
%left OR
%left AND
%left DOT
%right NOT
%left RELOP

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE
%nonassoc LOWER_THAN_SEMI
%nonassoc LOWER_THAN_RC LOWER_THAN_RB
%nonassoc SEMI RC

%%
Program: ExtDefList     {$$=add_node(TTOKEN, "Program", @$.first_line, 1, $1);root=$$;}
    ;

ExtDefList: ExtDef ExtDefList   {$$=add_node(TTOKEN, "ExtDefList", @$.first_line, 2, $1,$2);}
            |                   {$$=NULL;}
    ;

ExtDef: Specifier ExtDecList SEMI   {$$=add_node(TTOKEN, "ExtDef", @$.first_line, 3,$1,$2,$3);}
    | Specifier SEMI  {$$=add_node(TTOKEN, "ExtDef", @$.first_line, 2,$1,$2);}
    | Specifier FunDec CompSt {$$=add_node(TTOKEN, "ExtDef", @$.first_line, 3,$1,$2,$3);}
	| Specifier ExtDecList error %prec LOWER_THAN_SEMI	{printerror("Missing \";\".", errorlineno-1);}
    | Specifier ExtDecList error SEMI {
		if(mark!=errorlineno){ 
			printerror("Syntax error before \";\".", errorlineno);mark=errorlineno;
		}
	 }
    | Specifier error %prec LOWER_THAN_SEMI {
		if(mark!=errorlineno){
			printerror("Missing \";\".", errorlineno-1);mark=errorlineno;}
	 }
    | Specifier error SEMI  {
		if(mark!=errorlineno){
			printerror("Syntax error before \";\".", errorlineno);mark=errorlineno;}
	 }
    ;

ExtDecList: VarDec  {$$=add_node(TTOKEN, "ExtDecList", @$.first_line,1,$1);}
    | VarDec COMMA ExtDecList   {$$=add_node(TTOKEN, "ExtDecList", @$.first_line,3,$1,$2,$3);}
    ;

Specifier: TYPE {$$=add_node(TTOKEN, "Specifier", @$.first_line,1,$1);}
    | StructSpecifier   {$$=add_node(TTOKEN, "Specifier", @$.first_line,1,$1);}
    ;

StructSpecifier: STRUCT OptTag LC DefList RC {$$=add_node(TTOKEN,"StructSpecifier",@$.first_line,5,$1,$2,$3,$4,$5);}
    | STRUCT Tag    {$$=add_node(TTOKEN,"StructSpecifier",@$.first_line,2,$1,$2);}
	| STRUCT OptTag LC DefList error	{
		if(mark!=errorlineno){
			printerror("Missing \"}\".", errorlineno-1);mark=errorlineno;
		}
	 }
    ;

OptTag: ID  {$$=add_node(TTOKEN,"OptTag", @$.first_line, 1, $1);}
      | {$$ = NULL;}
      ;

Tag: ID {$$=add_node(TTOKEN,"Tag", @$.first_line, 1, $1);}
   ;

VarDec: ID  {$$=add_node(TTOKEN,"VarDec", @$.first_line, 1, $1);}
      | VarDec LB INT RB    {$$=add_node(TTOKEN,"VarDec", @$.first_line, 4, $1,$2,$3,$4);}
      | VarDec LB INT error RB  {
			if(mark!=errorlineno){
				printerror("Syntax error before \"]\".", errorlineno);mark=errorlineno;}
		}
      | VarDec LB error RB  {
			if(mark!=errorlineno){
				printerror("Syntax error between \"[\" and \"]\".", errorlineno);mark=errorlineno;}
		}
      ;

FunDec: ID LP VarList RP    {$$=add_node(TTOKEN,"FunDec", @$.first_line, 4, $1,$2,$3,$4);}
      | ID LP RP    {$$=add_node(TTOKEN,"FunDec", @$.first_line, 3, $1,$2,$3);}
      | ID LP error RP  {
			if(mark!=errorlineno){
				printerror("Syntax error between \"(\" and \")\".", errorlineno);mark=errorlineno;}
		}
      ;
VarList: ParamDec COMMA VarList {$$=add_node(TTOKEN,"VarList", @$.first_line, 3, $1,$2,$3);}
       | ParamDec   {$$=add_node(TTOKEN,"VarList", @$.first_line, 1, $1);}
       ;

ParamDec: Specifier VarDec  {$$=add_node(TTOKEN,"ParamDec", @$.first_line, 2, $1,$2);}
        ;

CompSt: LC DefList StmtList RC  {$$=add_node(TTOKEN,"CompSt", @$.first_line, 4, $1,$2,$3,$4);}
      | LC error RC {
			if(mark!=errorlineno){
				printerror("Syntax error between \"{\" and \"}\".", errorlineno);mark=errorlineno;}
		}
      | LC error %prec LOWER_THAN_RC {
			if(mark!=errorlineno){
				printerror("Missing \"}\".", errorlineno);mark=errorlineno;}
		}
      ;

StmtList: Stmt StmtList {$$=add_node(TTOKEN,"StmtList", @$.first_line, 2, $1,$2);}
        |   {$$=NULL;}
        ;

Stmt: Exp SEMI  {$$=add_node(TTOKEN,"Stmt", @$.first_line, 2, $1,$2);}
    | CompSt    {$$=add_node(TTOKEN,"Stmt", @$.first_line, 1,$1);}
    | RETURN Exp SEMI   {$$=add_node(TTOKEN,"Stmt", @$.first_line, 3, $1,$2,$3);}
    | IF LP Exp RP Stmt %prec LOWER_THAN_ELSE   {$$=add_node(TTOKEN,"Stmt", @$.first_line, 5, $1,$2,$3,$4,$5);}
    | IF LP Exp RP Stmt ELSE Stmt   {$$=add_node(TTOKEN,"Stmt", @$.first_line, 7, $1,$2,$3,$4,$5,$6,$7);}
    | WHILE LP Exp RP Stmt  {$$=add_node(TTOKEN, "Stmt", @$.first_line, 5, $1,$2,$3,$4,$5);}
	| Exp error	{
		if(mark!=errorlineno){
			printerror("Missing \";\".", errorlineno);mark=errorlineno;}
		}
	| RETURN Exp error	{
		if(mark!=errorlineno){
			printerror("Missing \";\".", errorlineno);mark=errorlineno;}
		}
    | error SEMI    {
		if(mark!=errorlineno){
			printerror("Syntax error before \";\".", errorlineno);mark=errorlineno;}
		}
    | error RP  {
		if(mark!=errorlineno){
			printerror("Syntax error before \")\".", errorlineno);mark=errorlineno;}
		}
    ;

DefList: Def DefList    {$$=add_node(TTOKEN,"DefList", @$.first_line, 2, $1,$2);}
       |    {$$ = NULL;}
       ;

Def: Specifier DecList SEMI {$$=add_node(TTOKEN,"Def", @$.first_line, 3, $1,$2,$3);}
   | Specifier DecList error %prec LOWER_THAN_SEMI {
		if(mark!=errorlineno){
			printerror("Probably missing \";\".", errorlineno-1);mark=errorlineno;}
	}
   | Specifier DecList error SEMI   {
		if(mark!=errorlineno){
			printerror("Syntax error before \";\".", errorlineno);mark=errorlineno;}
	}
   ;

DecList: Dec    {$$=add_node(TTOKEN,"DecList", @$.first_line, 1, $1);}
       | Dec COMMA DecList  {$$=add_node(TTOKEN,"DecList", @$.first_line, 3, $1,$2,$3);}
       ;

Dec: VarDec {$$=add_node(TTOKEN,"Dec", @$.first_line, 1, $1);}
   | VarDec ASSIGNOP Exp {$$=add_node(TTOKEN,"Dec", @$.first_line, 3, $1,$2,$3);}
   ;

Exp: Exp ASSIGNOP Exp   {$$=add_node(TTOKEN,"Exp", @$.first_line, 3, $1,$2,$3);}
   | Exp AND Exp    {$$=add_node(TTOKEN,"Exp", @$.first_line, 3, $1,$2,$3);}
   | Exp OR Exp {$$=add_node(TTOKEN,"Exp", @$.first_line, 3, $1,$2,$3);}
   | Exp RELOP Exp  {$$=add_node(TTOKEN,"Exp", @$.first_line, 3, $1,$2,$3);}
   | Exp PLUS Exp   {$$=add_node(TTOKEN,"Exp", @$.first_line, 3, $1,$2,$3);}
   | Exp MINUS Exp  {$$=add_node(TTOKEN, "Exp", @$.first_line, 3, $1,$2,$3);}
   | Exp STAR Exp   {$$=add_node(TTOKEN,"Exp", @$.first_line, 3, $1,$2,$3);}
   | Exp DIV Exp    {$$=add_node(TTOKEN,"Exp", @$.first_line, 3, $1,$2,$3);}
   | LP Exp RP  {$$=add_node(TTOKEN,"Exp", @$.first_line, 3, $1,$2,$3);}
   | MINUS Exp  {$$=add_node(TTOKEN,"Exp", @$.first_line, 2, $1,$2);}
   | NOT Exp    {$$=add_node(TTOKEN,"Exp", @$.first_line, 2, $1,$2);}
   | ID LP Args RP  {$$=add_node(TTOKEN,"Exp", @$.first_line, 4, $1,$2,$3,$4);}
   | ID LP RP   {$$=add_node(TTOKEN,"Exp", @$.first_line, 3, $1,$2,$3);}
   | Exp LB Exp RB  {$$=add_node(TTOKEN,"Exp", @$.first_line, 4, $1,$2,$3,$4);}
   | Exp DOT ID     {$$=add_node(TTOKEN,"Exp", @$.first_line, 3, $1,$2,$3);}
   | ID     {$$=add_node(TTOKEN,"Exp", @$.first_line, 1, $1);}
   | INT    {$$=add_node(TTOKEN,"Exp", @$.first_line, 1, $1);}
   | FLOAT  {$$=add_node(TTOKEN,"Exp", @$.first_line, 1, $1);}
   | ID LP Args error RP {
		if(mark!=errorlineno){
			printerror("Syntax error before \")\".", errorlineno);mark=errorlineno;}
		}
   | ID LP error RP {
		if(mark!=errorlineno){
			printerror("Syntax error before \")\".", errorlineno);mark=errorlineno;}
		}
   | Exp LB Exp error  {
		if(mark!=errorlineno){
			printerror("Missing \"]\".", errorlineno);mark=errorlineno;}	
		}
   | LP Exp error RP    {
		if(mark!=errorlineno){
			printerror("Syntax error before \")\".", errorlineno);mark=errorlineno;}
		}
   ;

Args: Exp COMMA Args    {$$=add_node(TTOKEN,"Args", @$.first_line, 3, $1,$2,$3);}
    | Exp   {$$=add_node(TTOKEN,"Args", @$.first_line, 1, $1);}
    ;

%%


void yyerror(char* msg){
    errornot=1;  
    errorlineno=yylineno;  
    //printf("Error Type B at Line %d: %s\n", yylineno, msg);
}

void printerror(char* msg, int lineno){
    printf("Error Type B at Line %d: %s\n", lineno, msg);
}






