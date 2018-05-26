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
					func=Func_analysis(s->child[1],type,VDEF);
					CompSt_analysis(s->child[2],func);
					break;
				case VSEMI:
					func=Func_analysis(s->child[1],type,VDEC);// change the chanshengshi
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
	Type *type;
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
	Type *type;
	Symbolele* symbol;
	switch(namemap(s->child[1]->name)){
		case VOptTag:
			Type *type1=type_for_struct();
			char *name=malloc(strlen(s->child[1]->id_name)+2);
			*name='!';strcpy(name+1,s->child[1]->id_name);
			symbol=symbol_for_nonfunc(type1,name,s->lineno);
			type=symbol->type;
			insertStructToStack(symbol,sta);
			DefList_analysis(s->child[3]);
			break;
		case VUnknown:
			Type *type1=type_for_struct();
			symbol=symbol_for_nonfunc(type1,"NULL",s->lineno);
			type=symbol->type;
			DefList_analysis(s->child[3]);
			break;
		case VTag:
			char *name=malloc(strlen(s->child[1]->id_name)+2);
			*name='!';strcpy(name+1,s->child[1]->id_name);
			symbol=hash_search(name);
			if(symbol==NULL){ 
				type=NULL;
				//error
				printf("eeee\n");
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
	switch(namemap(s->child[0])){
		case VID:
			symbol=symbol_for_nonfunc(type,s->child[0]->id_name,s->lineno);
			break;
		case VVarDec:
			Type *type1,*type2;
			type1=type_for_array(type,s->child[2]->int_value);
			s=s->child[0];
			while(namemap(s->child[0]->name)==VVarDec){
				type2=type_for_array(type1,s->child[2]->int_value);
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
	Func* func;
	switch(namemap(s->child[2]->name)){
		case VVarList:
			func=VarList_analysis(s->child[2],type);
			break;
		case VRP:
			Func* func1=malloc(sizeof(Func));
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
				//error  redefined function
				printf("eeeee\n");
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
					//error inconsistent func def
					printf("eeeee\n");
				}
				symbolold->func->defordec=VDEF;
			}
		}
		else{
			//error redefined func				
			printf("eeeeee\n");
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
				//error inconsistent func def
				printf("eeeee\n");
			}		
		}
		else{
			//error redefined func
			printf("eeeee\n");
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
	insertParamsToFuncarg(symbol,func);
	while(s->childnum==3)
	{
		s=s->child[2];
		symbol=ParamDec_analysis(s->child[0]);
		insertParamsToFuncarg(symbol,func);
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
			insertVarToStack(symbol,sta);
		}
	}
	DefList_analysis(s->child[1],NULL);
	StmtList_analysis(s->child[2]);
	Stack *stack1=sta;
	sta=sta->next;
	for(Symbolt *sbt=stack1->firstele,*ne;sbt!=NULL;sbt=ne)//??
	{	
		if(sbt->hash_pre!=NULL)
			sbt->hash_pre->hash_next=sbt->hash_next;
		if(sbt->hash_next!=NULL)
			sbt->hash_next->hash_pre=sbt->hash_pre;
		ne=sbt->hash_next;
		if(sbt!=NULL&&sbt->symbol!=NULL) free(sbt->symbol);
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
	switch(namemap(s->child[0]->name)){
		case VExp:
			Exp_analysis(s->child[0]);
			break;
		case VCompSt:
			CompSt_analysis(s->child[0],NULL);
			break;
		case VRETURN:
			Type *type1=Exp_analysis(s->child[1])->type;
			if(type_equiv_detect(type,type1)==0){
				//error
				printf("eeeeee\n");
			}
			break;
		case VIF:
			Type *type1=Exp_analysis(s->child[2])->type;
			if(type1->kind!=TINT){
				//error
				printf("eeeeee\n");
			}
			Stmt_analysis(s->child[4],type);
			if(s->childnum==7){
				Stmt_analysis(s->child[6],type);
			}
			break;
		case VWHILE:
			Type *type1=Exp_analysis(s->child[2])->type;
			if(type1->kind!=TINT){
				//error
				printf("eeeeee\n");
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
		insertVarToStack(symbol,sta);
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
			//error
			printf("eeeeee\n");
		}
	}
	if(s->childnum==3)
	{	
		Exp_analysis(s->child[2]);
	}
}

Exp* Exp_analysis(Node *s)
{
	Exp *exp=malloc(sizeof(Exp));
	switch(namemap(s->child[0]->name)){
		case VExp:
			Exp *expl=Exp_analysis(s->child[0]);
			switch(namemap(s->child[1]->name)){
				case VASSIGNOP:
					if(expl->lorr==VR){
						//error
						exp->type=NULL;
						printf("eeeee\n");
					}
					else{
						Exp *expr=Exp_analysis(s->child[2]);
						if(type_equiv_detect(expl->type,expr->type)==0){
							//error
							exp->type==NULL;
							printf("eeeeee\n");
						}
						else{
							exp->type=expl->type;
							exp->lorr=VR;//????
						}
					}
					break;
				case VAND:
				case VOR:
					Exp *expr=Exp_analysis(s->child[2]);
					if(expl->type->kind!=TINT||expr->type->kind!=TINT){
						//error
						printf("eeee\n");
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
					Exp *expr=Exp_analysis(s->child[2]);
					if((!(expl->type->kind==TINT||expl->type->kind==TFLOAT))||(!(expr->type->kind==TINT||expr->type->kind==TFLOAT))){
						//error
						printf("eeee\n");
						exp->type=NULL;
					}
					else{
						exp->type=expl->type;
						exp->lorr=VR;
					}
					break;
				case VLB:
					Exp *expr=Exp_analysis(s->child[2]);
					if(expl->type->kind!=TARRAY){
						//error
						printf("eeeee\n");
						exp->type=NULL;
					}
					else if(expr->type->kind!=TINT){
						//error
						printf("eeeee\n");
						exp->type=NULL;
					}
					else{
						exp->type=expl->type->array->elem;
						exp->lorr=expl->lorr;
					}
					break;
				case VDOT:
					if(expl->type->kind!=TSTRUCT){
						//error
						printf("eeeee\n");
						exp->type=NULL;
					}
					else{
						FieldList *fi=field_search(s->child[2]->id_name,expl->type);
						if(fi==NULL){
							//error
							printf("eeeeee\n");
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
			Exp *expr=Exp_analysis(s->child[1]);
			if(!(expr->type->kind==TINT||expr->type->kind==TFLOAT)){
				//error
				printf("eeee\n");
				exp->type=NULL;
			}
			else{
				exp->type=expr->type;
				exp->lorr=VR;
			}
			break;
		case VNOT:
			Exp *expr=Exp_analysis(s->child[1]);
			if(expr->type->kind!=TINT){
				//error
				printf("eeee\n");
				exp->type=NULL;
			}
			else{
				exp->type=expr->type;
				exp->lorr=VR;
			}
			break;			
		case VID:
			Symbolele *symbol=hash_search(s->child[0]->id_name);
			switch(s->childnum){
				case 1:
					if(symbol==NULL){
						//error
						printf("eeeee\n");
						exp->type=NULL;
					}
					else{
						exp->type=symbol->type;
						exp->lorr=VL;
					}
					break;
				case 3:
					if(symbol==NULL){
						//error
						printf("eeeee\n");
						exp->type=NULL;
					}
					else if(symbol->funcornot==0){
						//error
						printf("eeeee\n");
						exp->type=NULL;
					}
					else if(symbol->func->args!=NULL){
						//error
						printf("eeeee\n");
						exp->type=NULL;
					}
					else{
						exp->type=symbol->func->type;
						exp->lorr=VR;
					}
					break;	
				case 4:
					Arg *arglist=Args_analysis(s->child[2]);
					if(symbol==NULL){
						//error
						printf("eeeeee\n");
						exp->type=NULL;
					}
					else if(symbol->funcornot==0){
						//error
						printf("eeeee\n");
						exp->type=NULL;
					}
					else{
						symbol->func->args,arglist
						Args *ar1=symbol->func->args;
						Arg *ar2=arglist;
						int pan=0;
						while(ar1!=NULL&&ar2!=NULL){
							if(type_equiv_detest(ar1->a->type,ar2->type)==0){
								pan=0;
								break;
							}
							ar1=ar1->next;ar2=ar2->next;
						}
						if(ar1==NULL && ar1==NULL) pan=1;
						if(pan==0){
							//error
							printf("eeeee\n");
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
			exp->type->kind=TINT;
			exp->lorr=VR;
			break;
		case VFLOAT:
			exp->type->kind=TFLOAT;
			exp->lorr=VR;
			break;
		default:
			printf("Error!\n");
			break;
	}
}








