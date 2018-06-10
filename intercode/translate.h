#ifndef __TRANSLATE_H_
#define __TRANSLATE_H_

typedef Operand Operand;
typedef Intercode Intercode;
typedef Operands Operands;
typedef Intercodes Intercodes;

struct Operand{
	enum{OVAR,OCONSTANT,OTEMP,OLABEL,OFUNC,ONULL} kind;
	union{
		int value;
		char *name;
	};
	int num;
};

struct Intercode{
	enum{ILABEL,IFUNC,IASSIGN,IADD,ISUB,IMUL,IDIV,ILEA,ILD,IST,IJMP,IJCC,IRETURN,IDEC,IARG,ICALL,IPARAM,IREAD,IWRITE}kind;
	Operand *op1,*op2,*op3;
	char *relop;
	int size,type;
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
