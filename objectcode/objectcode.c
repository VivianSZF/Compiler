#include "../intercode/intercode.h"
#include "../intercode/translate.h"
#include "../semantic/symbol_table.h"
#include "../lexical_syntax/treeop.h"
#include "objectcode.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *constcode[]={
	".data\n",
	"_prompt: .asciiz \"Enter an integer:\"\n",
	"_ret: .asciiz \"\\n\"\n",
	".globl main\n",
	".text\n",
	"read:\n",
	"  li $v0, 4\n",
	"  la $a0, _prompt\n",
	"  syscall\n",
	"  li $v0, 5\n",
	"  syscall\n",
	"  jr $ra\n",
	"write:\n",
	"  li $v0, 1\n",
	"  syscall\n",
	"  li $v0, 4\n",
	"  la $a0, _ret\n",
	"  syscall\n",
	"  move $v0, $0\n",
	"  jr $ra\n"
};

const char* relopcode[]={
	"blt","ble","beq","bgt","bge","bne"
};

const char* swd="  sw $t%d, %d($fp)\n";

REG reg[10];

Operand *sp;


void print_constcode(FILE *f)
{
	int i;
	for(i=0;i<20;i++);
		fprintf(f,"%s",constcode[i]);
}


//array
void opsp_offset(Operand *op)
{
	if(op->offset==-1&&(op->kind==OVAR||op->kind==OTEMP)){
		sp->total=sp->total+4;
		op->offset=-sp->total;
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
	return -1;//???
}

//print reg operation for op
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

void objectcode(Intercodes *head,FILE *f)
{
	print_constcode(f);
	Intercodes *in=head;
	int args=0;
	int params=0;
	int stsize;
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
				sp=hash_search(c->re->name)->op;
				sp->total=4;
				params=0;
				fprintf(f,"%s:\n",c->re->name);
				fprintf(f,"  subu $sp, $sp, %d\n", sp->offset);
				fprintf(f,"  sw $fp, %d($sp)\n",sp->offset-4);
				fprintf(f,"  addi $fp, $sp, %d\n",sp->offset);
				stsize=sp->offset;
				break;
			case IASSIGN:
				opsp_offset(c->re);
				opsp_offset(c->op1);
				if(c->re->kind==OST){
					reg1=reg_select();
					reg2=reg_select();
					printforreg(f,reg1,c->re);
					fprintf(f,"  sw $t%d, 0($t%d)\n",reg2,reg1);
				}else if(c->op1->kind==OST){
					reg1=reg_select();
					reg2=reg_select();
					printforreg(f,reg2,c->op1);
					fprintf(f,"  lw $t%d, 0($t%d)\n",reg1,reg2);
					fprintf(f,swd,reg1,c->re->offset);
				}else{
					reg1=reg_select();
					printforreg(f,reg1,c->op1);
					fprintf(f,swd,reg1,c->re);
				}			
				break;
			case IADD:
				opsp_offset(c->re);
				opsp_offset(c->op1);
				opsp_offset(c->op2);	
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
				opsp_offset(c->re);
				opsp_offset(c->op1);
				opsp_offset(c->op2);	
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
				opsp_offset(c->re);
				opsp_offset(c->op1);
				opsp_offset(c->op2);
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
				opsp_offset(c->re);
				opsp_offset(c->op1);
				opsp_offset(c->op2);
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
				fprintf(f,"  %s $t%d, $t%d, %s",relopcode[c->relop],reg1,reg2,generate_name(c->op2));//??
				break;
			case IRETURN:
				reg1=reg_select();
				printforreg(f,reg1,c->re);
				fprintf(f,"  lw $fp, %d($sp)\n", stsize-4);
				fprintf(f,"  addi $sp, $sp, %d\n", stsize);
				fprintf(f,"  move $v0, $t%d\n  jr $ra\n", reg1);
				break;
			case IGOTO:
				fprintf(f,"  j %s\n",generate_name(c->re));
				break;
			case IDEC:
				sp->total=sp->total+c->size;
				c->re->offset=-sp->total;
				break;
			case IARG:
				args++;
				break;
			case ICALL:
				opsp_offset(c->re);
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
						fprintf(f,"  sw $t%d, %d($sp)\n",reg1,(i-3)*4);
					}
				}
				fprintf(f,"  addi $sp, $sp, -4\n");
				fprintf(f,"  sw $ra, 0($sp)\n");
				fprintf(f,"  jal %s\n",c->re->name);
				fprintf(f,"  lw $ra, 0($sp)\n");
				fprintf(f,"  addi $sp, $sp, 4\n");
				if(args>4)
					fprintf(f,"  addi $sp, $sp, %d\n", (args-4)*4);
				fprintf(f,"  sw $v0, %d($fp)\n",c->re->offset);
				args=0;
				break;
			case IPARAM:
				if(params<4){
					opsp_offset(c->re);
					fprintf(f,"  sw $a%d, %d($fp)\n",params,c->re->offset);
				}else{
					c->re->offset=(params-3)*4;
					//fprintf(f,"  lw $t")
				}
				params++;
				break;
			case IREAD:
				fprintf(f, "  addi $sp, $sp, -4\n");
				fprintf(f, "  sw $ra, 0($sp)\n");
				fprintf(f, "  jal read\n");
				fprintf(f, "  lw $ra, 0($sp)\n");
				fprintf(f, "  addi $sp, $sp, 4\n");
				fprintf(f, "  sw $v0, %d($fp)\n", c->re->offset);
				break;		
			case IWRITE:
				if(c->re->kind==OCONSTANT)
					fprintf(f,"  li $a0, %d\n",c->re->offset);
				else
					fprintf(f,"  lw $a0, %d($fp)\n",c->re->offset);
				fprintf(f, "  addi $sp, $sp, -4\n");
				fprintf(f, "  sw $ra, 0($sp)\n");
				fprintf(f, "  jal write\n");
				fprintf(f, "  lw $ra, 0($sp)\n");
				fprintf(f, "  addi $sp, $sp, 4\n");
				break;
		}
		in=in->next;
	}
}

