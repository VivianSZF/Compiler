#ifndef __SYMBOLTABLE_H_
#define __SYMBOLTABLE_H_

typedef struct Type Type;
typedef struct FieldList FieldList;
typedef struct Symboltable Symboltable;
typedef struct Args Args;
typedef struct Func Func;
typedef struct Symbolele Symboele;
typedef struct Stack Stack;

struct Type_
{
	enum{TINT,TFLOAT,TARRAY,TSTRUCTURE}kind;
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
	char* name;
	Type *type;
	FieldList *next;
};


struct Func
{
	Type *type;
	int cnt;
	struct Args
	{
		Symboltable *a;
		Args *next;
	}*args;
};

struct Symboltable
{
	char *name;
	int funcornot;
	int lineno;
	union{
		Type *type;
		Func *func;
	};
};

struct Symbolele
{
	Symboltable *s;
	Stack *stack;
	Symbolele *hash_pre;
	Symbolele *hash_next;
	Symbolele *stack_next;
};

struct Stack
{
	Symboele *ele;
	Stack *next;
};

#endif
