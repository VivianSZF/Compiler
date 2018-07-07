#include "../intercode/intercode.h"
#include "../intercode/translate.h"
#include "../semantic/symbol_table.h"
#include "../lexical_syntax/treeop.h"
#include "objectcode.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SUB_SP fprintf(f, "  addi $sp, $sp, -4\n");
#define ADD_SP fprintf(f, "  addi $sp, $sp, 4\n");
#define ST_RA fprintf(f, "  sw $ra, 0($sp)\n");
#define LD_RA fprintf(f, "  lw $ra, 0($sp)\n");

const char constcode[]={
	".data\n"
	"_prompt: .asciiz \"Enter an integer:\"\n"
	"_ret: .asciiz \"\\n\"\n"
	".globl main\n"
	".text\n"
	"read:\n"
	"  li $v0, 4\n"
	"  la $a0, _prompt\n"
	"  syscall\n"
	"  li $v0, 5\n"
	"  syscall\n"
	"  jr $ra\n"
	"write:\n"
	"  li $v0, 1\n"
	"  syscall\n"
	"  li $v0, 4\n"
	"  la $a0, _ret\n"
	"  syscall\n"
	"  move $v0, $0\n"
	"  jr $ra\n"
};

const char* relopcode[]={
	"blt","ble","beq","bgt","bge","bne"
};

const char *swd="  sw $t%d, %d($fp)\n";

REG reg[10];

Operand *nf;//the newest function


void opsp_offset(Operand *op)
{
	if(op->offset==-1){
		nf->total=nf->total+4;
		op->offset=-nf->total;
	}
}

void init_reg()
{
	int i;
	for(i=0;i<10;i++)
		reg[i].useornot=0;
}

int reg_select()
{
	int i;
	for(i=0;i<10;i++){
		if(reg[i].useornot==0){
			reg[i].useornot=1;
			return i;
		}
	}
	return -1;
}

//print loading reg operation for op
void printforreg(FILE *f, int r, Operand *op)
{
	if(op->kind==OCONSTANT){
		fprintf(f,"  li $t%d, %d\n",r,op->value);
	}else if(op->kind==OAD){
		fprintf(f,"  la $t%d, %d($fp)\n",r,op->opr->offset);
	}else if(op->kind==OST){
		fprintf(f,"  lw $t%d, %d($fp)\n",r, op->opr->offset);		
	}else{
		fprintf(f,"  lw $t%d, %d($fp)\n",r,op->offset);
	}
}

void compute_offset(Intercodes *head)
{
	Intercodes *in=head;
	int params=0;
	while(in!=NULL)
	{	
		Intercode *c=in->intercode;
		switch(c->kind){
			case IFUNC:
				nf=hash_search(generate_name(c->re))->op;
				if(nf==NULL){
					nf=Operand_func(c->re->name);
					nf->offset=0;
					hash_search(c->re->name)->op=nf;
				}
				nf->total=4;
				params=0;
				break;
			case IASSIGN:
				opsp_offset(c->re);
				if(c->op1->kind!=OCONSTANT)
					opsp_offset(c->op1);
				break;
			case IADD:
			case ISUB:
			case IMUL:
			case IDIV:
				opsp_offset(c->re);
				if(c->op1->kind!=OCONSTANT) opsp_offset(c->op1);
				if(c->op2->kind!=OCONSTANT) opsp_offset(c->op2);
				break;
			case ICALL:
				opsp_offset(c->re);
				break;
			case IDEC:
				nf->total=nf->total+c->size;
				c->re->offset=-nf->total;
				break;
			case IPARAM:
				if(params<4){
					opsp_offset(c->re);
				}else{
					c->re->offset=(params-3)*4;
				}
				params++;
				break;
			case IREAD:
				opsp_offset(c->re);
				break;
			
		}
		in=in->next;
	}
}

void objectcode(Intercodes *head,FILE *f)
{
	compute_offset(head);
	fprintf(f,constcode);
	Intercodes *in=head;
	int args=0;
	int params=0;
	while(in!=NULL)
	{
		init_reg();
		Intercode *c=in->intercode;
		int reg1,reg2,reg3;		
		int i;
		Intercodes *in1=in;
		switch(c->kind){
			case ILABEL:
				fprintf(f,"%s:\n", generate_name(c->re));
				break;
			case IFUNC:
				params=0;
				nf=hash_search(generate_name(c->re))->op;
				fprintf(f,"\n%s:\n",generate_name(c->re));
				fprintf(f,"  sw $fp, -4($sp)\n");
				fprintf(f,"  move $fp, $sp\n");
				fprintf(f,"  addi $sp, $sp, %d\n", -nf->total);
				break;
			case IASSIGN:
				if(c->re->kind==OST){
					reg1=reg_select();
					reg2=reg_select();
					printforreg(f,reg1,c->re);
					printforreg(f,reg2,c->op1);
					fprintf(f,"  sw $t%d, 0($t%d)\n",reg2,reg1);
				}else if(c->op1->kind==OST){
					reg1=reg_select();
					reg2=reg_select();
					printforreg(f,reg1,c->re);
					printforreg(f,reg2,c->op1);
					fprintf(f,"  lw $t%d, 0($t%d)\n",reg1,reg2);
					fprintf(f,swd,reg1,c->re->offset);
				}else{
					reg1=reg_select();
					printforreg(f,reg1,c->op1);
					fprintf(f,swd,reg1,c->re->offset);
				}			
				break;
			case IADD:	
				if(c->op2->kind==OCONSTANT){
					reg1=reg_select();
					reg2=reg_select();
					printforreg(f,reg1,c->re);
					printforreg(f,reg2,c->op1);
					fprintf(f,"  addi $t%d, $t%d, %d\n",reg1,reg2,c->op2->value);
				}else{
					reg1=reg_select();
					reg2=reg_select();
					reg3=reg_select();
					printforreg(f,reg1,c->re);
					printforreg(f,reg2,c->op1);
					printforreg(f,reg3,c->op2);
					fprintf(f,"  add $t%d, $t%d, $t%d\n",reg1,reg2,reg3);					
				}	
				fprintf(f,swd,reg1,c->re->offset);
				break;
			case ISUB:	
				if(c->op2->kind==OCONSTANT){
					reg1=reg_select();
					reg2=reg_select();
					printforreg(f,reg1,c->re);
					printforreg(f,reg2,c->op1);
					fprintf(f,"  addi $t%d, $t%d, %d\n",reg1,reg2,-c->op2->value);
				}else{
					reg1=reg_select();
					reg2=reg_select();
					reg3=reg_select();
					printforreg(f,reg1,c->re);
					printforreg(f,reg2,c->op1);
					printforreg(f,reg3,c->op2);
					fprintf(f,"  sub $t%d, $t%d, $t%d\n",reg1,reg2,reg3);					
				}	
				fprintf(f,swd,reg1,c->re->offset);
				break;	
			case IMUL:
				reg1=reg_select();
				reg2=reg_select();
				reg3=reg_select();
				printforreg(f,reg1,c->re);
				printforreg(f,reg2,c->op1);
				printforreg(f,reg3,c->op2);
				fprintf(f,"  mul $t%d, $t%d, $t%d\n",reg1,reg2,reg3);
				fprintf(f,swd,reg1,c->re->offset);
				break;
			case IDIV:
				reg1=reg_select();
				reg2=reg_select();
				reg3=reg_select();
				printforreg(f,reg1,c->re);
				printforreg(f,reg2,c->op1);
				printforreg(f,reg3,c->op2);
				fprintf(f,"  div $t%d, $t%d\n  mflo $t%d\n",reg2,reg3,reg1);
				fprintf(f,swd,reg1,c->re->offset);
				break;			
			case IIF:
				reg1=reg_select();
				reg2=reg_select();
				printforreg(f,reg1,c->re);
				printforreg(f,reg2,c->op1);				
				fprintf(f,"  %s $t%d, $t%d, %s\n",relopcode[c->relop],reg1,reg2,generate_name(c->op2));
				break;
			case IRETURN:
				reg1=reg_select();
				printforreg(f,reg1,c->re);
				fprintf(f,"  addi $sp, $sp, %d\n", nf->total);
				fprintf(f,"  lw $fp, -4($sp)\n");
				fprintf(f,"  move $v0, $t%d\n", reg1);
				fprintf(f,"  jr $ra\n\n");
				break;
			case IGOTO:
				fprintf(f,"  j %s\n",generate_name(c->re));
				break;
			case IDEC:
				break;
			case IARG:
				args++;
				break;
			case ICALL:
				if(args<5){
					for(i=0;i<args;i++){
						in1=in1->pre;
						reg1=reg_select();
						printforreg(f,reg1,in1->intercode->re);
						fprintf(f,"  move $a%d, $t%d\n",i,reg1);
					}
				}else{
					fprintf(f,"  addi $sp, $sp, %d\n",-(args-4)*4);
					for(i=0;i<4;i++){
						in1=in1->pre;
						reg1=reg_select();
						printforreg(f,reg1,in1->intercode->re);
						fprintf(f,"  move $a%d, $t%d\n",i,reg1);
					}
					for(i=4;i<args;i++){
						in1=in1->pre;
						reg1=reg_select();
						printforreg(f,reg1,in1->intercode->re);
						fprintf(f,"  sw $t%d, %d($sp)\n",reg1,(i-4)*4);
					}
				}
				SUB_SP;
				ST_RA;
				fprintf(f,"  jal %s\n",c->op1->name);
				LD_RA;
				ADD_SP;
				if(args>4)
					fprintf(f,"  addi $sp, $sp, %d\n", (args-4)*4);
				fprintf(f,"  sw $v0, %d($fp)\n",c->re->offset);
				args=0;
				break;
			case IPARAM:
				if(params<4){
					fprintf(f,"  sw $a%d, %d($fp)\n",params,c->re->offset);
				}
				///else{
				
					//fprintf(f,"  lw $t")
				//}
				params++;
				break;
			case IREAD:
				SUB_SP;
				ST_RA;
				fprintf(f, "  jal read\n");
				LD_RA;
				ADD_SP;
				fprintf(f, "  sw $v0, %d($fp)\n", c->re->offset);
				break;		
			case IWRITE:
				if(c->re->kind==OCONSTANT)
					fprintf(f,"  li $a0, %d\n",c->re->offset);
				else
					fprintf(f,"  lw $a0, %d($fp)\n",c->re->offset);
				SUB_SP;
				ST_RA;
				fprintf(f, "  jal write\n");
				LD_RA;
				ADD_SP;
				break;
		}
		in=in->next;
	}
}

