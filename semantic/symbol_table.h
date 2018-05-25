#ifndef __SYMBOLTABLE_H_
#define __SYMBOLTABLE_H_

typedef struct Type Type;
typedef struct FieldList FieldList;
typedef struct Symbolele Symbolele;
typedef struct Args Args;
typedef struct Func Func;
typedef struct Symbolt Symbolt;
typedef struct Stack Stack;

enum {SVAR,SARRAY,SSTRUCT};

struct Type_
{
	enum{TINT,TFLOAT,TARRAY,TSTRUCT}kind;
	union
	{
		struct Array
		{
			Type *elem;
			int size;
		}array;
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
	struct Args
	{
		Symbolele *a;
		Args *next;
	}*args;
};

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

Symbolele* symbol_for_nonfunc(Type *type, char *name, int lineno);
Symbolele* symbol_for_func(Func *func, char *name, int lineno);

#endif
