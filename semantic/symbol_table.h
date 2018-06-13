#ifndef __SYMBOLTABLE_H_
#define __SYMBOLTABLE_H_

#include <stdint.h>
#define SIZE 0x3fff


typedef struct Type_ Type;
typedef struct FieldList_ FieldList;
typedef struct Symbolele Symbolele;
typedef struct Args Args;
typedef struct Func Func;
typedef struct Symbolt Symbolt;
typedef struct Stack Stack;
typedef struct Array Array;
typedef struct Exp Exp;

enum{VDEF,VDEC};
enum{VL,VR};

struct Type_
{
	enum{VTINT,VTFLOAT,VTARRAY,VTSTRUCT}kind;
	union
	{
		struct Array
		{
			Type *elem;
			int size;
		}*array;
		FieldList *structure;
	};
};

struct FieldList_
{
	Symbolele *s;
	FieldList *next;
};


struct Func
{
	Type *type;
	int defordec;
	struct Args
	{
		Symbolele *a;
		Args *next;
	}*args;
};

struct Exp
{
	Type *type;
	int lorr;
};

#include "../intercode/translate.h"


struct Symbolele
{
	char *name;
	int funcornot;
	int lineno;
	Operand *op;
	union{
		Type *type;
		Func *func;
	};
};

struct Symbolt
{
	Symbolele *s;
	Stack *stack;
	Symbolt *hash_pre;
	Symbolt *hash_next;
	Symbolt *stack_pre;
	Symbolt *stack_next;
};

struct Stack
{
	Symbolt *firstele;
	Stack *next;
};

uint32_t hash_pjw(char* name);
Symbolele* symbol_for_nonfunc(Type *type, char *name, int lineno);
Symbolele* symbol_for_func(Func *func, char *name, int lineno);
int type_equiv_detect(Type *t1,Type *t2);
Symbolele *stack_search(char *name, Stack *stack);
Symbolele *hash_search(char *name);
FieldList *field_search(char *name, Type *type);
void insertToStack(Symbolele *symbol, Stack *stack);

extern Symbolt *hash[SIZE];
extern Stack *sta;

#endif
