#include <stdio.h>
#include "symbol_table.h"
#include "semantic.h"
#include <string.h>
#include <stdlib.h>


int struct_varassign;
Intercodes *in_head;

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
	else if(strcmp(name,"RETURN")==0) return VRETURN;
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

int error_search(int errortype, int lineno)
{
	int pan=0;
	int mark=0;
	for(int i=0;i<500;i++){
		if(errortable[errortype-1][i]==lineno)
			return 1;
		if(errortable[errortype-1][i]==0){
			mark=i;
			break;
		}
	}
	errortable[errortype-1][mark]=lineno;
	return 0;
}

void Program_analysis(Node *root)
{
	int i;
	memset(errortable,0,sizeof(errortable));
	struct_varassign=0;
	for(i=0;i<SIZE;i++)
	{
		Symbolt *sbt=malloc(sizeof(Symbolt));
		sbt->s=NULL;
		sbt->stack=NULL;
		sbt->hash_pre=NULL;
		sbt->hash_next=NULL;
		sbt->stack_pre=NULL;
		sbt->stack_next=NULL;
		hash[i]=sbt;
	}
	Stack *stack=malloc(sizeof(Stack));
	stack->firstele=NULL;	
	stack->next=sta;
	sta=stack;
	in_head=NULL;
	in_head=ExtDefList_analysis(root->child[0]);
	for(Symbolt *p=sta->firstele;p!=NULL;p=p->stack_next)
	{
		if(p->s->funcornot==1 && p->s->func->defordec==VDEC){
			//error 18
			if(error_search(18,p->s->lineno)==0)
				printf("Error type 18 at Line %d: Undefined function \"%s\".\n",p->s->lineno,p->s->name);
		}
	}
}

Intercodes* ExtDefList_analysis(Node *s)
{	
	Intercodes *in;
	while(s!=NULL)
	{
		in=combine_code(in,ExtDef_analysis(s->child[0]));
		s=s->child[1];
	}
	return in;
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
					func=FunDec_analysis(s->child[1],type,VDEF);
					CompSt_analysis(s->child[2],func);
					break;
				case VSEMI:
					func=FunDec_analysis(s->child[1],type,VDEC);
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
}

void ExtDecList_analysis(Node *s, Type *type)
{
	Symbolele *symbol=VarDec_analysis(s->child[0],type);
	if(stack_search(symbol->name,sta)==NULL)
	{
		insertToStack(symbol,sta);
	}
	else{
		//error 3
		if(error_search(3,symbol->lineno)==0)
			printf("Error type 3 at Line %d: Redefined variable \"%s\".\n",symbol->lineno,symbol->name);
	}
	while(s->childnum==3)
	{
		s=s->child[2];
		symbol=VarDec_analysis(s->child[0],type);
		if(stack_search(symbol->name,sta)==NULL)
		{
			insertToStack(symbol,sta);
		}
		else{
			//error 3
			if(error_search(3,symbol->lineno)==0)
				printf("Error type 3 at Line %d: Redefined variable \"%s\".\n",symbol->lineno,symbol->name);
		}
	}	
}

Type* Specifier_analysis(Node *s)
{
	Type *type;
	switch(namemap(s->child[0]->name)){
		case VTYPE:
			type=(Type *)malloc(sizeof(Type));
			if(s->child[0]->type==0)
				type->kind=VTINT;
			else if(s->child[0]->type==1)
				type->kind=VTFLOAT;
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
	Type *type;
	Symbolele* symbol;
	char *name;
	switch(namemap(s->child[1]->name)){
		case VOptTag:
			type=(Type*)malloc(sizeof(Type));
			type->kind=VTSTRUCT;
			type->structure=NULL;
			name=(char *)malloc(strlen(s->child[1]->child[0]->id_name)+2);
			*name='!';strcpy(name+1,s->child[1]->child[0]->id_name);
			symbol=symbol_for_nonfunc(type,name,s->lineno);
			if(stack_search(symbol->name,sta)==NULL){
				insertToStack(symbol,sta);
			}
			else{
				//error 16
				char *name=malloc(strlen(symbol->name));
				strcpy(name,symbol->name+1);
				if(error_search(16,symbol->lineno)==0)
					printf("Error type 16 at Line %d: Duplicated name \"%s\".\n",symbol->lineno,name);
			}
			struct_varassign=1;
			DefList_analysis(s->child[3],type);
			struct_varassign=0;
			break;
		case VUnknown:
			type=(Type*)malloc(sizeof(Type));
			type->kind=VTSTRUCT;
			type->structure=NULL;
			symbol=symbol_for_nonfunc(type,"NULL",s->lineno);
			DefList_analysis(s->child[3],type);
			break;
		case VTag:
			name=(char*)malloc(strlen(s->child[1]->child[0]->id_name)+2);
			*name='!';strcpy(name+1,s->child[1]->child[0]->id_name);
			symbol=hash_search(name);
			if(symbol==NULL){ 
				type=NULL;
				//error 17
				if(error_search(17,s->lineno)==0)
					printf("Error type 17 at Line %d: Undefined structure \"%s\".\n",s->lineno,s->child[1]->child[0]->id_name);
			}
			else type=symbol->type;	
			break;
		default:
			printf("Error!\n");
			break;
	}
	return type;
}	

Symbolele* VarDec_analysis(Node *s, Type *type)
{
	Symbolele *symbol;
	Type *type1,*type2;
	switch(namemap(s->child[0]->name)){
		case VID:
			symbol=symbol_for_nonfunc(type,s->child[0]->id_name,s->lineno);
			break;
		case VVarDec:
			type1=(Type*)malloc(sizeof(Type));
			type1->kind=VTARRAY;
			type1->array=malloc(sizeof(Array));
			type1->array->elem=type;
			type1->array->size=s->child[2]->int_value;
			s=s->child[0];
			while(namemap(s->child[0]->name)==VVarDec){
				type2=(Type*)malloc(sizeof(Type));
				type2->kind=VTARRAY;
				type2->array=malloc(sizeof(Array));
				type2->array->elem=type1;
				type2->array->size=s->child[2]->int_value;
				type1=type2;
				s=s->child[0];
			}
			symbol=symbol_for_nonfunc(type1,s->child[0]->id_name,s->lineno);
			break;
		default:
			printf("Error!\n");
			break;
	}
	return symbol;
}

Func* FunDec_analysis(Node *s, Type *type, int defordec)
{
	Func *func,*func1;
	switch(namemap(s->child[2]->name)){
		case VVarList:
			func=VarList_analysis(s->child[2],type);
			break;
		case VRP:
			func1=(Func*)malloc(sizeof(Func));
			func1->type=type;
			func1->args=NULL;
			func=func1;
			break;
		default:
			printf("Error!\n");
			break;
	}
	func->defordec=defordec;
	Symbolele *symbol;
	symbol=symbol_for_func(func,s->child[0]->id_name,s->lineno);
	if(defordec==VDEF){
		Symbolele *symbolold=hash_search(symbol->name);
		if(symbolold==NULL){
			insertToStack(symbol,sta);
		}
		else if(symbolold->funcornot==1){
			if(symbolold->func->defordec==VDEF){
				//error 4
				if(error_search(4,symbol->lineno)==0)
					printf("Error type 4 at Line %d: Redefined function \"%s\".\n",symbol->lineno,symbol->name);
			}
			else{
				Args *ar1,*ar2;
				ar1=symbol->func->args;ar2=symbolold->func->args;
				int pan=0;
				while(ar1!=NULL && ar2!=NULL){
					if(type_equiv_detect(ar1->a->type,ar2->a->type)==0){
						pan=0;break;
					}
					ar1=ar1->next;ar2=ar2->next;
				}
				if(ar1==NULL && ar2==NULL) pan=1;
				if(pan==0){
					//error 19
					if(error_search(19,symbol->lineno)==0)
						printf("Error type 19 at Line %d: Inconsistent declaration of function \"%s\".\n",symbol->lineno,symbol->name);
				}
				symbolold->func->defordec=VDEF;
			}
		}
		else{
			//error 4				
			if(error_search(4,symbol->lineno)==0)
				printf("Error type 4 at Line %d: Redefined function \"%s\".\n",symbol->lineno,symbol->name);
		}
	}
	else{
		Symbolele *symbolold=hash_search(symbol->name);
		if(symbolold==NULL){
			insertToStack(symbol,sta);
		}
		else if(symbolold->funcornot==1){
			Args *ar1,*ar2;
			ar1=symbol->func->args;ar2=symbolold->func->args;
			int pan=0;
			while(ar1!=NULL && ar2!=NULL){
				if(type_equiv_detect(ar1->a->type,ar2->a->type)==0){
					pan=0;break;
				}
				ar1=ar1->next;ar2=ar2->next;
			}
			if(ar1==NULL && ar2==NULL) pan=1;
			if(pan==0){
				//error 19
				if(error_search(19,symbol->lineno)==0)
					printf("Error type 19 at Line %d: Inconsistent declaration of function \"%s\".\n",symbol->lineno,symbol->name);
			}		
		}
		else{
			//error 4
			if(error_search(4,symbol->lineno)==0)
				printf("Error type 4 at Line %d: Redefined function \"%s\".\n",symbol->lineno,symbol->name);
		}
	}
	return func;
}

Func* VarList_analysis(Node *s,Type *type)
{
	Func *func=malloc(sizeof(Func));
	func->type=type;
	func->args=NULL;
	Symbolele *symbol=ParamDec_analysis(s->child[0]);
	Args *ar=malloc(sizeof(Args));
	ar->a=symbol;
	ar->next=NULL;
	if(func->args==NULL){
		func->args=ar;
	}	
	else{
		Args *temp=func->args;
		ar->next=temp;
		func->args=ar;
	}	
	while(s->childnum==3)
	{
		s=s->child[2];
		symbol=ParamDec_analysis(s->child[0]);
		ar=(Args*)malloc(sizeof(Args));
		ar->a=symbol;
		ar->next=NULL;
		if(func->args==NULL){
			func->args=ar;
		}	
		else{
			Args *temp=func->args;
			ar->next=temp;
			func->args=ar;
		}
	}
	return func;
}

Symbolele* ParamDec_analysis(Node *s)
{
	Type *type=Specifier_analysis(s->child[0]);
	Symbolele *symbol=VarDec_analysis(s->child[1],type);
	return symbol;
}

void CompSt_analysis(Node *s, Func *func)
{
	Stack *stack=malloc(sizeof(Stack));
	stack->firstele=NULL;
	stack->next=sta;
	sta=stack;
	if(func!=NULL){
		for(Args *ar=func->args;ar!=NULL;ar=ar->next){
			Symbolele *symbol=ar->a;
			if(stack_search(symbol->name,sta)==NULL)
			{
				insertToStack(symbol,sta);
			}
			else{
				//error 3
				if(error_search(3,symbol->lineno)==0)
					printf("Error type 3 at Line %d: Redefined variable \"%s\".\n",symbol->lineno,symbol->name);
			}
		}
	}
	DefList_analysis(s->child[1],NULL);
	if(func==NULL)
		StmtList_analysis(s->child[2],NULL);
	else
		StmtList_analysis(s->child[2],func->type);
	Stack *stack1=sta;
	sta=sta->next;
	for(Symbolt *sbt=stack1->firstele,*ne;sbt!=NULL;sbt=ne)
	{	
		if(sbt->hash_pre!=NULL)
			sbt->hash_pre->hash_next=sbt->hash_next;
		if(sbt->hash_next!=NULL)
			sbt->hash_next->hash_pre=sbt->hash_pre;
		ne=sbt->hash_next;
		if(sbt!=NULL&&sbt->s!=NULL) free(sbt->s);
		if(sbt!=NULL) free(sbt);
	}
	free(stack1);
}

void StmtList_analysis(Node *s, Type *type)
{
	while(s!=NULL)
	{
		Stmt_analysis(s->child[0],type);
		s=s->child[1];
	}
}

void Stmt_analysis(Node *s,Type *type)
{
	Type *type1;
	switch(namemap(s->child[0]->name)){
		case VExp:
			Exp_analysis(s->child[0]);
			break;
		case VCompSt:
			CompSt_analysis(s->child[0],NULL);
			break;
		case VRETURN:
			type1=Exp_analysis(s->child[1])->type;
			if(type_equiv_detect(type,type1)==0){
				//error 8
				if(error_search(8,s->lineno)==0)
					printf("Error type 8 at Line %d: Type mismatched for return.\n", s->lineno);
			}
			break;
		case VIF:
			type1=Exp_analysis(s->child[2])->type;
			if(type1==NULL||type1->kind!=VTINT){
				//error 7
				if(error_search(7,s->lineno)==0)
					printf("Error type 7 at Line %d: Type mismatched for operands.\n",s->lineno);
			}
			Stmt_analysis(s->child[4],type);
			if(s->childnum==7){
				Stmt_analysis(s->child[6],type);
			}
			break;
		case VWHILE:
			type1=Exp_analysis(s->child[2])->type;
			if(type1==NULL||type1->kind!=VTINT){
				//error 7
				if(error_search(7,s->lineno)==0)
					printf("Error type 7 at Line %d:Type mismatched for operands.\n",s->lineno);
			}
			Stmt_analysis(s->child[4],type);
			break;
		default:
			printf("Error!\n");
			break;
	}
}

void DefList_analysis(Node *s,Type *type)
{
	while(s!=NULL)
	{
		Def_analysis(s->child[0],type);
		s=s->child[1];
	}
}

void Def_analysis(Node *s, Type *type)
{
	Type *type1=Specifier_analysis(s->child[0]);
	DecList_analysis(s->child[1],type,type1);
}

void DecList_analysis(Node *s,Type *type,Type *type1)
{
	Dec_analysis(s->child[0],type,type1);
	while(s->childnum==3)
	{
		s=s->child[2];
		Dec_analysis(s->child[0],type,type1);
	}
}

void Dec_analysis(Node *s,Type *type,Type *type1)
{
	Symbolele* symbol=VarDec_analysis(s->child[0],type1);
	if(type==NULL){
		if(stack_search(symbol->name,sta)==NULL)
		{
			insertToStack(symbol,sta);
		}
		else{
			//error 3
			if(error_search(3,symbol->lineno)==0)
				printf("Error type 3 at Line %d: Redefined variable \"%s\".\n",symbol->lineno,symbol->name);
		}
	}
	else{
		if(field_search(symbol->name,type)==NULL){
			FieldList *fi=malloc(sizeof(FieldList));
			fi->s=symbol;
			fi->next=NULL;
			if(type->structure==NULL){
				type->structure=fi;
			}
			else{
				FieldList *temp=type->structure;
				fi->next=temp;
				type->structure=fi;
			}
		}
		else{
			//error 15
			if(error_search(15,symbol->lineno)==0)
				printf("Error type 15 at Line %d: Redefined field \"%s\".\n",symbol->lineno,symbol->name);
		}
	}
	if(s->childnum==3)
	{	
		if(struct_varassign==1){
			//error 15
			if(error_search(15,symbol->lineno*(-1))==0)
				printf("Error type 15 at Line %d: Invalid assignment to field \"%s\".\n",symbol->lineno,symbol->name);	
		}
		Exp_analysis(s->child[2]);
	}
}

Exp* Exp_analysis(Node *s)
{
	Exp *exp=malloc(sizeof(Exp));
	Exp *expl,*expr;
	Symbolele *symbol;
	Args *arglist,*ar1,*ar2;
	switch(namemap(s->child[0]->name)){
		case VExp:
			expl=Exp_analysis(s->child[0]);
			switch(namemap(s->child[1]->name)){
				case VASSIGNOP:
					if(expl->lorr==VR){
						//error 6
						exp->type=NULL;
						if(error_search(6,s->lineno)==0)
							printf("Error type 6 at Line %d: The left-hand side of an assignment must be a variable.\n", s->lineno);
					}
					else{
						expr=Exp_analysis(s->child[2]);
						if(type_equiv_detect(expl->type,expr->type)==0){
							//error 5
							exp->type==NULL;
							if(error_search(5,s->lineno)==0)
								printf("Error type 5 at Line %d: Type mismatched for assignment.\n",s->lineno);
						}
						else{
							exp->type=expl->type;
							exp->lorr=VR;
						}
					}
					break;
				case VAND:
				case VOR:
					expr=Exp_analysis(s->child[2]);
					if(expl->type==NULL||expr->type==NULL||expl->type->kind!=VTINT||expr->type->kind!=VTINT){
						//error 7
						if(error_search(7,s->lineno)==0)
							printf("Error type 7 at Line %d: Type mismatched for operands.\n",s->lineno);
						exp->type=NULL;
					}
					else{
						exp->type=expl->type;
						exp->lorr=VR;
					}
					break;
				case VRELOP:
				case VPLUS:
				case VMINUS:
				case VSTAR:
				case VDIV:
					expr=Exp_analysis(s->child[2]);
					if((!(expl->type!=NULL&&(expl->type->kind==VTINT||expl->type->kind==VTFLOAT)))||(!(expr->type!=NULL&&(expr->type->kind==VTINT||expr->type->kind==VTFLOAT)))||type_equiv_detect(expl->type,expr->type)==0){
						//error 7
						if(error_search(7,s->lineno)==0)
							printf("Error type 7 at Line %d: Type mismatched for operands.\n",s->lineno);
						exp->type=NULL;
					}
					else{
						exp->type=expl->type;
						exp->lorr=VR;
					}
					break;
				case VLB:
					expr=Exp_analysis(s->child[2]);
					if(expl->type==NULL||expl->type->kind!=VTARRAY){
						//error 10
						if(error_search(10,s->lineno)==0)
							printf("Error type 10 at Line %d: \"%s\" is not an array.\n",s->lineno,s->child[0]->child[0]->id_name);
						exp->type=NULL;
					}
					else if(expl->type==NULL||expr->type->kind!=VTINT){
						//error 12
						if(error_search(12,s->lineno)==0)
							printf("Error type 12 at Line %d: \"%g\" is not an integer.\n",s->lineno,s->child[2]->child[0]->float_value);
						exp->type=NULL;
					}
					else{
						exp->type=expl->type->array->elem;
						exp->lorr=expl->lorr;
					}
					break;
				case VDOT:
					if(expl->type==NULL||expl->type->kind!=VTSTRUCT){
						//error 13
						if(error_search(13,s->lineno)==0)
							printf("Error type 13 at Line %d: Illegal use of \".\".\n",s->lineno);
						exp->type=NULL;
					}
					else{
						FieldList *fi=field_search(s->child[2]->id_name,expl->type);
						if(fi==NULL){
							//error 14
							if(error_search(14,s->lineno)==0)
								printf("Error type 14 at Line %d: Non-existent field \"%s\".\n",s->lineno,s->child[2]->id_name);
							exp->type=NULL;
						}
						else{
							exp->type=fi->s->type;
							exp->lorr=expl->lorr;
						}
					}
					break;
				default:
					printf("Error!\n");
					break;
			}
			break;
		case VLP:
			exp=Exp_analysis(s->child[1]);
			break;
		case VMINUS:
			expr=Exp_analysis(s->child[1]);
			if(expr->type==NULL||!(expr->type->kind==VTINT||expr->type->kind==VTFLOAT)){
				//error 7
				if(error_search(7,s->lineno)==0)
					printf("Error type 7 at Line %d: Type mismatched for operands.\n",s->lineno);
				exp->type=NULL;
			}
			else{
				exp->type=expr->type;
				exp->lorr=VR;
			}
			break;
		case VNOT:
			expr=Exp_analysis(s->child[1]);
			if(expr==NULL||expr->type->kind!=VTINT){
				//error 7
				if(error_search(7,s->lineno)==0)
					printf("Error type 7 at Line %d: Type mismatched for operands.\n",s->lineno);
				exp->type=NULL;
			}
			else{
				exp->type=expr->type;
				exp->lorr=VR;
			}
			break;			
		case VID:
			symbol=hash_search(s->child[0]->id_name);
			switch(s->childnum){
				case 1:
					if(symbol==NULL){
						//error 1
						if(error_search(1,s->lineno)==0)
							printf("Error type 1 at Line %d: Undefined variable \"%s\".\n",s->lineno,s->child[0]->id_name);
						exp->type=NULL;
					}
					else{
						exp->type=symbol->type;
						exp->lorr=VL;
					}
					break;
				case 3:
					if(symbol==NULL){
						//error 2
						if(error_search(2,s->lineno)==0)
							printf("Error type 2 at Line %d: Undefined function \"%s\".\n",s->lineno,s->child[0]->id_name);
						exp->type=NULL;
					}
					else if(symbol->funcornot==0){
						//error 11
						if(error_search(11,s->lineno)==0)
							printf("Error type 11 at Line %d: \"%s\" is not a function.\n",s->lineno,s->child[0]->id_name);
						exp->type=NULL;
					}
					else if(symbol->func->args!=NULL){
						//error 9
						if(error_search(9,s->lineno)==0)
							printf("Error type 9 at Line %d: Function \"%s\" should not have arguments.\n",s->lineno,s->child[0]->id_name);
						exp->type=NULL;
					}
					else{
						exp->type=symbol->func->type;
						exp->lorr=VR;
					}
					break;	
				case 4:
					arglist=Args_analysis(s->child[2]);
					if(symbol==NULL){
						//error 2
						if(error_search(2,s->lineno)==0)
							printf("Error type 2 at Line %d: Undefined function \"%s\".\n",s->lineno,s->child[0]->id_name);
						exp->type=NULL;
					}
					else if(symbol->funcornot==0){
						//error 11
						if(error_search(11,s->lineno)==0)
							printf("Error type 11 at Line %d: \"%s\" is not a function.\n",s->lineno,s->child[0]->id_name);
						exp->type=NULL;
					}
					else{
						ar1=symbol->func->args;
						ar2=arglist;
						int pan=0;
						while(ar1!=NULL&&ar2!=NULL){
							if(type_equiv_detect(ar1->a->type,ar2->a->type)==0){
								pan=0;
								break;
							}
							ar1=ar1->next;ar2=ar2->next;
						}
						if(ar1==NULL && ar1==NULL) pan=1;
						if(pan==0){
							//error 9
							if(error_search(9,s->lineno)==0)
								printf("Error type 9 at Line %d: Argument(s) is(are) incorrect for function \"%s\".\n",s->lineno,s->child[0]->id_name);
							exp->type=NULL;
						}
						else{
							exp->type=symbol->func->type;
							exp->lorr=VR;
						}
					}
					break;
				default:
					printf("Error!\n");
					break;						
			}
			break;
		case VINT:
			exp->type=(Type*)malloc(sizeof(Type));
			exp->type->kind=VTINT;
			exp->lorr=VR;
			break;
		case VFLOAT:
			exp->type=(Type*)malloc(sizeof(Type));
			exp->type->kind=VTFLOAT;
			exp->lorr=VR;
			break;
		default:
			printf("Error!\n");
			break;
	}
	return exp;
}

Args *Args_analysis(Node *s)
{
	Args *arglist=NULL;
	Exp *exp=Exp_analysis(s->child[0]);
	Args *ar=malloc(sizeof(Args));
	ar->a=(Symbolele*)malloc(sizeof(Symbolele));
	ar->a->type=exp->type;
	ar->next=NULL;
	if(arglist==NULL){
		arglist=ar;
	}
	else{
		Args *temp=arglist;
		ar->next=temp;
		arglist=ar;
	}
	while(s->childnum==3)
	{
		s=s->child[2];
		exp=Exp_analysis(s->child[0]);
		Args *arn=malloc(sizeof(Args));
		arn->a=(Symbolele*)malloc(sizeof(Symbolele));
		arn->a->type=exp->type;
		arn->next=NULL;
		if(arglist==NULL){
			arglist=arn;
		}
		else{
			Args *temp1=arglist;
			arn->next=temp1;
			arglist=arn;
		}
	}
	return arglist;
}






