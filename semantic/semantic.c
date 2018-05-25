#include <stdio.h>
#include "symbol_table.h"
#include "semantic.h"
#include <string.h>

int namemap(char *name)
{
	if (strcmp(name,"INT")==0) return VINT;
	else if(strcmp(name,"FLOAT")==0) return VFLOAT;
	else if(strcmp(name,"ID")==0) return VID;
	else if(strcmp(name,"SEMI")==0) return VSEMI;
	else if(strcmp(name,"COMMA")==0) return VCOMMA;
	else if(strcmp(name,"ASSIGNOP")==0) return VASSIGNOP;
	else if(strcmp(name,"RELOP")==0) return VRELOP;
	else if(strcmp(name,"PLUS")==0) return VPLUS;
	else if(strcmp(name,"MINUS")==0) return VMINUS;
	else if(strcmp(name,"STAR")==0) return VSTAR;
	else if(strcmp(name,"DIV")==0) return VDIV;
	else if(strcmp(name,"AND")==0) return VAND;
	else if(strcmp(name,"OR")==0) return VOR;
	else if(strcmp(name,"DOT")==0) return VDOT;
	else if(strcmp(name,"NOT")==0) return VNOT;
	else if(strcmp(name,"TYPE")==0) return VTYPE;
	else if(strcmp(name,"LP")==0) return VLP;
	else if(strcmp(name,"RP")==0) return VRP;
	else if(strcmp(name,"LB")==0) return VLB;
	else if(strcmp(name,"RB")==0) return VRB;
	else if(strcmp(name,"LC")==0) return VLC;
	else if(strcmp(name,"RC")==0) return VRC;
	else if(strcmp(name,"STRUCT")==0) return VSTRUCT;
	else if(strcmp(name,"RETURN")==0) return VRETURE;
	else if(strcmp(name,"IF")==0) return VIF;
	else if(strcmp(name,"ELSE")==0) return VELSE;
	else if(strcmp(name,"WHILE")==0) return VWHILE;
	else if(strcmp(name,"Program")==0) return VProgram;
	else if(strcmp(name,"ExtDefList")==0) return VExtDefList;
	else if(strcmp(name,"ExtDef")==0) return VExtDef;
	else if(strcmp(name,"ExtDecList")==0) return VExtDecList;
	else if(strcmp(name,"Specifier")==0) return VSpecifier;
	else if(strcmp(name,"StructSpecifier")==0) return VStructSpecifier;
	else if(strcmp(name,"OptTag")==0) return VOptTag;
	else if(strcmp(name,"Tag")==0) return VTag;
	else if(strcmp(name,"VarDec")==0) return VVarDec;
	else if(strcmp(name,"FunDec")==0) return VFunDec;
	else if(strcmp(name,"VarList")==0) return VVarList;
	else if(strcmp(name,"ParamDec")==0) return VParamDec;
	else if(strcmp(name,"CompSt")==0) return VCompSt;
	else if(strcmp(name,"StmtList")==0) return VStmtList;
	else if(strcmp(name,"Stmt")==0) return VStmt;
	else if(strcmp(name,"DefList")==0) return VDefList;
	else if(strcmp(name,"Def")==0) return VDef;
	else if(strcmp(name,"DecList")==0) return VDecList;
	else if(strcmp(name,"Dec")==0) return VDec;
	else if(strcmp(name,"Exp")==0) return VExp;
	else if(strcmp(name,"Args")==0) return VArgs;
	else return VUnknown;
}

void Program_analysis(Node *root)
{
	ExtDefList_analysis(root->child[0]);
}

void ExtDefList_analysis(Node *s)
{
	while(s!=NULL)
	{
		ExtDef_analysis(s->child[0]);
		s=s->child[1];
	}
}

void ExtDef_analysis(Node *s)
{
	Type *type=Specifier_analysis(s->child[0]);
	Func *func;
	switch(namemap(s->child[1]->name)){
		case VExtDecList:
			ExtDecList_analysis(s->child[1],type);
			break;
		case VSEMI:
			break;
		case VFunDec:
			switch(namemap(s->child[2]->name)){
				case VCompSt:
					func=Func_analysis(s->child[1],type,1);
					CompSt_analysis(s->child[2],func);
					break;
				case VSEMI:
					func=Func_analysis(s->child[1],type,0);// change the chanshengshi
					break;
				default:
					printf("Error!\n");
					break;
			}
			break;
		default:
			printf("Error!\n");
			break;	
}

void ExtDecList_analysis(Node *s, Type *type)
{
	Symbol *symbol=VarDec_analysis(s->child[0],type);
	insertVarToStack(symbol,sta);
	while(s->childnum==3)
	{//???????????????? ==1???
		s=s->child[2];
		symbol=VarDec_analysis(s->child[0],type);
		insertVarToStack(symbol,sta);
	}	
}

Type* Specifier_analysis(Node *s)
{
	Type *type=NULL;
	switch(namemap(s->child[0]->name)){
		case VTYPE:
			type=(Type *)malloc(sizeof(Type);
			if(s->child[0]->type==0)
				type.kind=TINT;
			else if(s->child[0]->type==1)
				type.kind=TFLOAT;
			break;
		case VStructSpecifier:
			type=StructSpecifier_analysis(s->child[0]);
			break;
		default:
			printf("Error!\n");
			break;
	}
	return type;
}

Type* StructSpecifier_analysis(Node *s)
{
	char *name;
	
}	
