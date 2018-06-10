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

Intercodes* prepare_intercode(Intercode *c)
{
	if(c==NULL)return NULL;
	Intercodes *in=malloc(sizeof(Intercodes));
	in->intercode=c;
	in->pre=in->next=NULL;
	return in;
}


Intercode* intercode_func(char *name)
{
	Intercode *c=malloc(sizeof(Intercode));
	c->kind=IFUNC;
	c->re=(Operand*)malloc(sizeof(Operand));
	c->re->name=name;
	c->re->kind=OFUNC;
	return c;	
}

Intercode* intercode_1(Operand *op, int kind)
{
	Intercode *c=malloc(sizeof(Intercode));
	c->kind=kind;
	c->re=op;	
	return c;
}

Intercode* intercode_2(Operand *op1, Operand *op2, int kind)
{

}

Operand* Operand_constant(int value)
{
	Operand* op=malloc(sizeof(Operand));
	op->kind=OCONSTANT;
	op->value=value;
}

Operand* Operand_var()
{
	Operand* op=malloc(sizeof(Operand));
	op->kind=OVAR;
	op->n=nvar;
	nvar++;
}

Operand* Operand_temp()
{
	Operand* op=malloc(sizeof(Operand));
	op->kind=OTEMP;
	op->n=ntemp;
	ntemp++;
}

Operand* Operand_label()
{
	Operand* op=malloc(sizeof(Operand));
	op->kind=OLABEL;
	op->n=nlabel;
	nlabel++;
}


}
