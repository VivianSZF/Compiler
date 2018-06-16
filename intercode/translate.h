#ifndef __TRANSLATE_H_
#define __TRANSLATE_H_

#include "../lexical_syntax/treeop.h"

typedef struct Operand Operand;
typedef struct Intercode Intercode;
typedef struct Operands Operands;
typedef struct Intercodes Intercodes;

struct Operand{
	enum{OVAR,OCONSTANT,OTEMP,OLABEL,OFUNC,OAD,OST,ONULL} kind;
	union{
		int value;
		char *name;
	};
	int adornot;
	int n;
};

struct Intercode{
	enum{ILABEL,IFUNC,IASSIGN,IADD,ISUB,IMUL,IDIV,IAD,IST,IIF,IRETURN,IGOTO,IDEC,IARG,ICALL,IPARAM,IARGAD,IREAD,IWRITE}kind;
	Operand *re,*op1,*op2;
	int relop,size;	
};

struct Operands{
	Operand* op;
	Operands *pre,*next;
};

struct Intercodes{
	Intercode* intercode;
	Intercodes *pre,*next; 
};

#include "../semantic/symbol_table.h"

Intercodes* Func_translate(Node *s, Func *func);
Intercodes* Args_translate(Node *s, Operands **ops);
Intercodes* Exp_translate(Node *s, Operand **op);
Intercodes* AS_translate(Node *s, Operand **op, Type **type);
Intercodes* Cond_translate(Node *s, Operand *label_true, Operand *label_false);
int get_size(Type *type);
int get_field_offset(char *name, Type *type);
Intercodes* VarDec_translate(Node *s, Type *type);



#endif
