#ifndef __SYMBOLTABLE_H_
#define __SYMBOLTABLE_H_

#include <stdint.h>
#define SIZE 0x3fff

typedef struct Type Type;
typedef struct FieldList FieldList;
typedef struct Symbolele Symbolele;
typedef struct Args Args;
typedef struct Func Func;
typedef struct Symbolt Symbolt;
typedef struct Stack Stack;
typedef struct Array Array;
typedef struct Exp Exp;
typedef struct Arg Arg;

enum{VDEF,VDEC};
enum{VL,VR};

struct Type_
{
	enum{TINT,TFLOAT,TARRAY,TSTRUCT}kind;
	union
	{
		struct Array
		{
			Type *elem;
			int size;
		}*array;
		FieldList *structure;
	}u;
};

struct FieldList_
{
	Symbolele *s;
	FieldList *next;
};


struct Func
{
	Type *type;
	int cnt;
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

struct Arg
{
	Type *type;
	Arg *next; 
}

struct Symbolele
{
	char *name;
	int funcornot;
	int lineno;
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
Type* type_for_struct();
Type* type_for_array(Type *elem, int size);
int type_equiv_detect(Type *t1,Type *t2);
Symbolele *stack_search(char *name, Stack *stack);
Symbolele *hash_search(char *name);
FieldList *field_search(char *name, Type *type);
void insertToStack(Symbolele *symbol, Stack *stack);
void insertVarToStack(Symbolele *symbol, Stack *stack);
void insertStructToStack(Symbolele *symbol, Stack *stack);
void insertParamsToFuncarg(Symbolele *symbol, Func *func);

extern Symbolt *hash[SIZE];


#endif
