#include <stdio.h>
#include "symbol_table.h"
#include <stdlib.h>
#include <string.h>
#include "../intercode/intercode.h"

Symbolt *hash[SIZE];
Stack *sta;

uint32_t hash_pjw(char* name)
{
	uint32_t val = 0, i;
	for(; *name;++name)
	{
		val=(val<<2)+*name;
		if(i=val&~SIZE) val=(val^(i>>12))&SIZE;
	}
	return val;
}

Symbolele* symbol_for_nonfunc(Type *type, char *name, int lineno)
{
	Symbolele *symbol=malloc(sizeof(Symbolele));
	symbol->name=malloc((sizeof(char))*(strlen(name)+1));
	strcpy(symbol->name, name);
	symbol->funcornot=0;
	symbol->lineno=lineno;
	symbol->type=type;
	symbol->op=Operand_var();
	return symbol;
}

Symbolele* symbol_for_func(Func *func, char *name, int lineno)
{
	Symbolele *symbol=malloc(sizeof(Symbolele));
	symbol->name=malloc((sizeof(char))*(strlen(name)+1));
	strcpy(symbol->name, name);
	symbol->funcornot=1;
	symbol->lineno=lineno;
	symbol->func=func;
	symbol->op=NULL;
	return symbol;
}

int type_equiv_detect(Type *t1,Type *t2)
{
	FieldList *f1,*f2;
	if(t1==NULL||t2==NULL) return 0;
	if(t1->kind!=t2->kind) return 0;
	switch(t1->kind){
		case VTINT:
			return 1;
			break;
		case VTFLOAT:
			return 1;
			break;
		case VTARRAY:
			if(t1->array->size==t2->array->size)
				return 1;
			else
				return 0;
			break;
		case VTSTRUCT:
			f1=t1->structure;
			f2=t2->structure;
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
			printf("Error!\n");
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
	for(Symbolt *p=hash[hashval]->hash_next;p!=NULL;p=p->hash_next)
	{
		if(strcmp(p->s->name,name)==0)
			return p->s;
	}
	return NULL;
}

FieldList *field_search(char *name, Type *type)
{
	for(FieldList *p=type->structure;p!=NULL;p=p->next){
		if (strcmp(p->s->name,name)==0) 
			return p;
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
		sbt->stack_next=sta->firstele;
		sta->firstele=sbt;
	}
	Symbolt *hasht=hash[hash_pjw(symbol->name)]->hash_next;
	sbt->hash_next=hasht;
	if(hasht!=NULL)
		hasht->hash_pre=sbt;
	sbt->hash_pre=hash[hash_pjw(symbol->name)];
	hash[hash_pjw(symbol->name)]->hash_next=sbt;
}

