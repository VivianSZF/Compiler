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

Symbol *stack_search(char *name, Stack *stack)
{
	uint32_t hashvalue=hash_pjw(name);
	for(Symbolt *p=hash[hashvalue]->hash_next;p!=NULL;p=p->hash_next)
	{
		if(strcmp(p->s->name,name)==0&&p->stack==stack)
			return p->s;
	}
	return NULL;
}
void insertToStack(Symbol *symbol, Stack *stack)
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


