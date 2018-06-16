#ifndef __INTERCODE_H_
#define __INTERCODE_H_

#include "translate.h"
#include "../semantic/semantic.h"
#include "../lexical_syntax/treeop.h"
#include <stdio.h>

//extern int nvar,ntemp,nlabel;

Intercodes* combine_code(Intercodes *in1,Intercodes *in2);
Operands* combine_ops(Operand *op, Operands *ops);
Intercodes* prepare_code(Intercode *c);
Intercode* Intercode_func(char *name);
Intercode* Intercode_1(Operand *op, int kind);
Intercode* Intercode_2(Operand *op1, Operand *op2, int kind);
Intercode* Intercode_3(Operand *re,Operand *op1,Operand *op2, int kind);
Intercode* Intercode_dec(char *arr, int size);
Intercode* Intercode_if(Operand *re,Operand *op1, Operand *op2, int relop);
Operand* Operand_constant(int value);
Operand* Operand_var();
Operand* Operand_temp();
Operand* Operand_label();
Operand* Operand_func(char *name);
Operand* Operand_ad(char *name);
Operand* Operand_st(char *name);
Operand* Operand_const0();
Operand* Operand_const1();
char* generate_name(Operand *op);
void print_Intercodes(Intercodes *head,FILE *f);


#endif
