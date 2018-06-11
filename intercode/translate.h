#ifndef __TRANSLATE_H_
#define __TRANSLATE_H_

#include "../lexical_syntax/treeop.h"
#include "../semantic/semantic.h"
#include "intercode.h"

typedef Operand Operand;
typedef Intercode Intercode;
typedef Operands Operands;
typedef Intercodes Intercodes;

struct Operand{
	enum{OVAR,OCONSTANT,OTEMP,OLABEL,OFUNC,OAD,OST} kind;
	union{
		int value;
		char *name;
	};
	int n;
};

struct Intercode{
	enum{ILABEL,IFUNC,IASSIGN,IADD,ISUB,IMUL,IDIV,ILEA,ILD,IST,IJMP,IJCC,IRETURN,IDEC,IARG,ICALL,IPARAM,IREAD,IWRITE}kind;
	Operand *re,*op1,*op2;
	char *relop;
	
};

struct Operands{
	Operand* op;
	Operands *pre,*next;
};

struct Indercodes{
	Intercode* intercode;
	Intercodes *pre,*next; 
};


#endif
