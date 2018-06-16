#include "intercode.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int nvar=0,ntemp=0,nlabel=0;

const char* Relop[]={"<","<=","==",">",">=","!="};

Intercodes* combine_code(Intercodes *in1,Intercodes *in2)
{
	if(in1==NULL)return in2;
	if(in2==NULL)return in1;
	Intercodes *p=in1;
	while(p->next!=NULL){
		p=p->next;
	}
	p->next=in2;
	return in1;
}

Operands* combine_ops(Operand *op, Operands *ops)
{
	Operands  *ops1=malloc(sizeof(Operands));
	ops1->op=op;
	ops1->pre=NULL;
	ops1->next=ops;
	return ops1;
}

Intercodes* prepare_code(Intercode *c)
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
	if(op1==NULL)return NULL;
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

Intercode* Intercode_dec(char *arr, int size)
{
	Intercode *c=malloc(sizeof(Intercode));
	c->kind=IDEC;
	Operand *op=malloc(sizeof(Operand));
	op->kind=ONULL;
	op->name=arr;
	c->re=op;
	c->size=size;
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
	op->adornot=0;
	return op;
}

Operand* Operand_var()
{
	Operand* op=malloc(sizeof(Operand));
	op->kind=OVAR;
	op->n=nvar;
	nvar++;
	op->adornot=0;
	return op;
}

Operand* Operand_temp()
{
	Operand* op=malloc(sizeof(Operand));
	op->kind=OTEMP;
	op->n=ntemp;
	ntemp++;
	op->adornot=0;
	return op;
}

Operand* Operand_label()
{
	Operand* op=malloc(sizeof(Operand));
	op->kind=OLABEL;
	op->n=nlabel;
	nlabel++;
	op->adornot=0;
	return op;
}

Operand* Operand_func(char *name)
{
	Operand* op=malloc(sizeof(Operand));
	op->name=name;//????
	op->kind=OFUNC;
	op->adornot=0;
	return op;
}  

Operand* Operand_ad(char *name)
{
	Operand* op=malloc(sizeof(Operand));
	op->kind=OAD;
	op->name=name;
	op->adornot=0;
	return op;
}

Operand* Operand_st(char *name)
{
	Operand *op=malloc(sizeof(Operand));
	op->kind=OST;
	op->name=name;
	op->adornot=0;
	return op;
}

Operand* Operand_const0()
{
	Operand *op=malloc(sizeof(Operand));
	op->kind=OCONSTANT;
	op->value=0;
	op->adornot=0;
	return op;
}

Operand* Operand_const1()
{
	Operand *op=malloc(sizeof(Operand));
	op->kind=OCONSTANT;
	op->value=1;
	op->adornot=0;
	return op;
}

Operand* Operand_null(char *name)
{
	Operand *op=malloc(sizeof(Operand));
	op->kind=ONULL;
	op->name=name;
	op->adornot=0;
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
	}else if(op->kind==ONULL){
		name=op->name;
	}
	return name;	
}

void print_Intercodes(Intercodes *head, FILE *f)
{
	if(head==NULL)
		return;
	Intercodes *t=head;
	Intercode *c;
	int k=0;
	while(t!=NULL)
	{
		c=t->intercode;
		k=c->kind;
		if(k==ILABEL)
			fprintf(f,"LABEL %s :\n",generate_name(c->re));
		else if(k==IFUNC)
			fprintf(f,"FUNCTION %s :\n",generate_name(c->re));
		else if(k==IASSIGN){if(c->re==NULL) c->re=c->op1;
			fprintf(f,"%s := %s\n",generate_name(c->re),generate_name(c->op1));}
		else if(k==IADD)
			fprintf(f,"%s := %s + %s\n",generate_name(c->re),generate_name(c->op1),generate_name(c->op2));
		else if(k==ISUB)
			fprintf(f,"%s := %s - %s\n",generate_name(c->re),generate_name(c->op1),generate_name(c->op2));
		else if(k==IMUL)
			fprintf(f,"%s := %s * %s\n",generate_name(c->re),generate_name(c->op1),generate_name(c->op2));
		else if(k==IDIV)
			fprintf(f,"%s := %s / %s\n",generate_name(c->re),generate_name(c->op1),generate_name(c->op2));
		else if(k==IAD)	
			fprintf(f,"%s := &%s\n",generate_name(c->re),generate_name(c->op1));
		else if(k==IST)
			fprintf(f,"%s := *%s\n",generate_name(c->re),generate_name(c->op1));
		else if(k==IIF)
			fprintf(f,"IF %s %s %s GOTO %s\n",generate_name(c->re),Relop[c->relop],generate_name(c->op1),generate_name(c->op2));
		else if(k==IRETURN)
			fprintf(f,"RETURN %s\n", generate_name(c->re));
		else if(k==IGOTO)
			fprintf(f,"GOTO %s\n",generate_name(c->re));
		else if(k==IDEC)
			fprintf(f,"DEC %s %d\n",generate_name(c->re),c->size);
		else if(k==IARG)
			fprintf(f,"ARG %s\n",generate_name(c->re));
		else if(k==ICALL)
			fprintf(f,"%s := CALL %s\n",generate_name(c->re),generate_name(c->op1));
		else if(k==IPARAM)
			fprintf(f,"PARAM %s\n",generate_name(c->re));
		else if(k==IARGAD){
			c->re->kind=ONULL;
			fprintf(f,"ARG &%s\n",generate_name(c->re));
		}
		else if(k==IPARAMAD){
			c->re->kind=ONULL;
			fprintf(f,"PARAM %s\n",generate_name(c->re));
		}
		else if(k==IREAD)
			fprintf(f,"READ %s\n",generate_name(c->re));
		else if(k==IWRITE)
			fprintf(f,"WRITE %s\n",generate_name(c->re));
		t=t->next;
	}	
}
