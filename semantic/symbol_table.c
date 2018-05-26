#include <stdio.h>
#include "symbol_table.h"
#include "hash.h"
#include <stdlib.h>

Symbolt *hash[SIZE];
Stack *sta;

Symbolele* symbol_for_nonfunc(Type *type, char *name, int lineno)
{
	Symbolele *symbol=malloc(sizeof(Symbolele));
	symbol->name=malloc((sizeof(char))*(strlen(name)+1));
	strcpy(symbol->name, name);
	symbol->funcornot=0;
	symbol->lineno=lineno;
	symbol->type=type;
	return symbol;
}

Symbolele* symbol_for_func(Func *func, char *name, int lineno)
{
	Symbolele *symbol=malloc(sizeof(Symbolele));
	symbol->name=malloc((sizeof(char))*(strlen(name)+1));
	strcpy(symbol->name, name);
	symbol->funcornot=1;
	symbol->lineno=lineno;
	symbol->type=func;
	return symbol;
}

Type* type_for_struct()
{
	Type *type=malloc(sizeof(Type));
	type->kind=TSTRUCT;
	type->structure=NULL;
}

Type* type_for_array(Type *elem, int size)
{
	Type *type=malloc(sizeof(Type));
	type->kind=TARRAY;
	type->array=malloc(sizeof(Array));
	type->array->elem=elem;
	type->array->size=size;
	return type;
}

int type_equiv_detect(Type *t1,Type *t2)
{
	if(t1->kind!=t2->kind) return 0;
	switch(t1->kind){
		case TINT:
			return 1;
			break;
		case TFLOAT:
			return 1;
			break;
		case TARRAY:
			if(t1->array->size==t2->array->size)
				return 1;
			else
				return 0;
			break;
		case TSTRUCT:
			FieldList *f1=t1->structure;
			FieldList *f2=t2->structure;
			while(f1!=NULL &&f2!=NULL){
				if(type_equiv_detect(f1->s->type,f2->s->type)==0)
					return 0;
				f1=f1->next;f2=f2->next;
			}
			if(f1==NULL&&f2==NULL) 
				return 1;
			else
				return 0;
			break;
		default:
			break;
	}	
}

Symbolele *stack_search(char *name, Stack *stack)
{
	uint32_t hashvalue=hash_pjw(name);
	for(Symbolt *p=hash[hashvalue]->hash_next;p!=NULL;p=p->hash_next)
	{
		if(strcmp(p->s->name,name)==0&&p->stack==stack)
			return p->s;
	}
	return NULL;
}

Symbolele *hash_search(char *name)
{
	uint32_t hashval=hash_pjw(name);
	for(Symbolt *p=hash[hashvalue]->hash_next;p!=NULL;p=p->hash_next)
	{
		if(strcmp(p->s->name,name)==0)
			return p->s;
	}
	return NULL;
}

void insertToStack(Symbolele *symbol, Stack *stack)
{
	Symbolt *sbt=malloc(sizeof(Symbolt));
	sbt->s=symbol;
	sbt->stack=stack;
	sbt->hash_pre=NULL;
	sbt->hash_next=NULL;
	sbt->stack_next=NULL;
	if(sta->firstele==NULL)
		sta->firstele=sbt;
	else{
		sbt->sta_next=sta->firstele;
		sta->firstele=sbt;
	}
	Symbolt *hasht=hash[hash_pjw(symbol->name)]->hash_next;
	sbt->hash_next=hasht;
	if(hasht!=NULL)
		hasht->hash_pre=sbt;
	sbt->hash_pre=hash[hash_pjw(symbol->name)];
	hash[hash_pjw(symbol->name)]->hash_next=sbt;
}

void insertVarToStack(Symbolele *symbol, Stack *stack)
{
	if(stack_search(symbol->name,stack)==NULL)
	{
		insertToStack(symbol,stack);
	}
	else{
		//error
	}
}

void insertStructToStack(Symbolele *symbol, Stack *stack)
{
	if(stack_search(symbol->name,stack)==NULL)
	{
		insertToStack(symbol,stack);
	}
	else{
		//error
	}
}

void insertParamsToFuncarg(Symbolele *symbol, Stack *stack)
{	
	
}
