#include "intercode.h"
#include <stdio.h>
#include <stdlib.h>

nvar=0;
ntemp=0;
nlabel=0;

void combine_code(Intercodes *in1,Intercodes *in2)
{
	if(in1==NULL)return in2;
	if(in2==NULL)return in1;
	Intercodes *p=in1;
	while(p->next!=NULL){
		p=p->next;
	}
	p->next=in2;
	in2->pre=p;
}

Operands* combine_ops(Operand *op, Operands *ops)
{
	Operands  *ops1=malloc(sizeof(Operands));
	ops1->op=op;
	ops1->pre=NULL;
	ops1->next=ops;
	ops->pre=ops1;
	return ops1;
}

Intercodes* prepare_intercode(Intercode *c)
{
	if(c==NULL)return NULL;
	Intercodes *in=malloc(sizeof(Intercodes));
	in->intercode=c;
	in->pre=in->next=NULL;
	return in;
}


Intercode* Intercode_func(char *name)
{
	Intercode *c=malloc(sizeof(Intercode));
	c->kind=IFUNC;
	c->re=(Operand*)malloc(sizeof(Operand));
	c->re->name=name;
	c->re->kind=OFUNC;
	return c;	
}

Intercode* Intercode_1(Operand *op, int kind)
{
	Intercode *c=malloc(sizeof(Intercode));
	c->kind=kind;
	c->re=op;	
	return c;
}

Intercode* Intercode_2(Operand *op1, Operand *op2, int kind)
{
	Intercode *c=malloc(sizeof(Intercode));
	c->kind=kind;
	c->re=op1;
	c->op1=op2;
	return c;
}

Intercode* Intercode_3(Operand *re,Operand *op1,Operand *op2, int kind)
{
	Intercode *c=malloc(sizeof(Intercode));
	if(kind==VPLUS){
		c->kind=IADD;
	}else if(kind==VMINUS){
		c->kind=ISUB;
	}else if(kind==VSTAR){
		c->kind=IMUL;
	}else if(kind==VDIV){
		c->kind=IDIV;
	}
	c->re=re;
	c->op1=op1;
	c->op2=op2;
	return c;	
}

Intercode* Intercode_if(Operand *re,Operand *op1, Operand *op2, int relop)
{
	Intercode *c=malloc(sizeof(Intercode));
	c->kind=IIF;
	c->relop=relop;
	c->re=re;
	c->op1=op1;
	c->op2=op2;
	return c;
}

Operand* Operand_constant(int value)
{
	Operand* op=malloc(sizeof(Operand));
	op->kind=OCONSTANT;
	op->value=value;
	return op;
}

Operand* Operand_var()
{
	Operand* op=malloc(sizeof(Operand));
	op->kind=OVAR;
	op->n=nvar;
	nvar++;
	return op;
}

Operand* Operand_temp()
{
	Operand* op=malloc(sizeof(Operand));
	op->kind=OTEMP;
	op->n=ntemp;
	ntemp++;
	return op;
}

Operand* Operand_label()
{
	Operand* op=malloc(sizeof(Operand));
	op->kind=OLABEL;
	op->n=nlabel;
	nlabel++;
	return op;
}

Operand* Operand_char(char *name)
{
	Operand* op=malloc(sizeof(Operand));
	op->name=name;//????
	op->kind=OFUNC;
	return op;
}  

Operand* Operand_ad(char *name)
{
	Operand* op=malloc(sizeof(Operand));
	op->kind=OAD;
	op->name=name
	return op;
}

Operand* Operand_st(char *name)
{
	Operand *op=malloc(sizeof(Operand));
	op->kind=OST;
	op->name=name;
	return op;
}

Operand* Operand_const0()
{
	Operand *op=malloc(sizeof(Operand));
	op->kind=OCONSTANT;
	op->value=0;
	return op;
}

Operand* Operand_const1()
{
	Operand *op=malloc(sizeof(Operand));
	op->kind=OCONSTANT;
	op->value=1;
	return op;
}

char* generate_name(Operand *op)
{
	char *name=malloc(sizeof(char)*20);
	if(op->kind==OVAR){
		sprintf(name,"v%d",op->n);
	}else if(op->kind==OCONSTANT){
		sprintf(name,"#%d",op->value);
	}else if(op->kind==OTEMP){
		sprintf(name,"t%d",op->n);	
	}else if(op->kind==OLABEL){
		sprintf(name,"l%d",op->n);
	}else if(op->kind==OFUNC){
		name=op->name;
	}else if(op->kind==OAD){
		sprintf(name,"&%s",op->name);
	}else if(op->kind==OST){
		sprintf(name,"*%s",op->name);	
	}
	return name;	
}
