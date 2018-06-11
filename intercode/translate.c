#include "translate.h"
#include <string.h>

Intercodes* Func_translate(Node *s, Func *func)
{
	Intercodes *in=NULL;
	Intercode *c=intercode_func(s->child[0]->id_name);
	Intercodes *in1=prepare_intercode(c);
	Intercodes *in2=NULL;
	Intercodes *in3=NULL;
	for(Args *args=func->args;args!=NULL;args=args->next){
		in3=NULL;
		Operand *op=malloc(sizeof(Operand));
		op->kind=OVAR;
		op->name=args->a->name;
		Intercode *c1=intercode_1(op,IPARAM);
		in3=prepare_intercode(c1);
		in2=combine_code(in2,in3);
	}
	in=combine_code(in1,in2);
	return in;
}

Intercodes* Args_translate(Node *s, Operands *ops)
{
	Intercodes *in=NULL,*in1,*in2;
	Operand *op1;
	switch(s->childnum){
		case 1:
			op1=Operand_temp();
			in1=Exp_translate(s->child[0],&op1);
			ops=combine_ops(op1,ops);//???
			break;
		case 2:
			op1=Operand_temp();
			in1=Exp_translate(s->child[0],&op1);
			ops=combine_ops(op1,ops);
			in2=Args_translate(s->child[2],&ops);
			in=combine_code(in1,in2);
			break;
	}
	return in;
}

Intercodes* Exp_translate(Node *s, Operand *op)
{
	Intercodes *in=NULL,*in1,*in2,*in3,*in4;
	Intercode *c;
	Operand *op1,*op2,*opc0,*opc1;
	Operands *ops=NULL;
	char *name;
	switch(namemap(s->child[0]->name)){
		case VINT:
			op=Operand_constant(s->child[0]->int_value);
			break;
		case VID:
			symbol=hash_search(s->child[0]->id_name);
			switch(s->childnum){
				case 1:
					op=Operand_var();
					break;
				case 3:
					if(strcmp(symbo1->name,"read")==0){
						c=intercode_1(op,IREAD);
						in=prepare_intercode(c);
					}
					else{
						op1=Operand_func(symbol->name);
						c=intercode_2(op,op1,ICALL);
						in=prepare_intercode(c);
					}
					break;	
				case 4:
					in1=Args_translate(s->child[2],&ops);
					if(strcmp(symbol->name,"write")==0){
						c=Intercode_1(ops->op,IWRITE);
						in2=prepare_intercode(c);
						in=combine_code(in1,in2);
					}
					else{
						in2=NULL;
						for(Operands *opss=ops;opss!=NULL;opss=opss->next){
							c=Intercode_1(opss->op,IARG);
							in3=prepare_intercode(c);
							in2=combine_code(in2,in3);
						}
						op1=Operand_func(symbol->name);
						c=intercode_2(op,op1,ICALL);
						in3=prepare_intercode(c);
						in=combine_code(in1,combine_code(in2,in3));
					}
					break;
				default:
					printf("Error!\n");
					break;						
			}
			break;
		case VExp:
			switch(namemap(s->child[1]->name)){
				case VASSIGNOP:
					if(namemap(s->child[0]->child[0]->name)==VID){
						op1=Operand_temp();
						in1=Exp_translate(s->child[0],&op1);
						op2=Operand_temp();
						in2=Exp_translate(s->child[2],&op2);
						c=Intercode_2(op1,op2,IASSIGN);
						in3=prepare_code(c);
						c=Intercode_2(op,op1,IASSIGN);
						in4=prepare_code(c);
						in=combine_code(in1,combine_code(in2,combine_code(in3,in4)));
					}
					else{
						op1=Operand_temp();
						in1=AS_translate(s->child[0],&op);
						name=generate_name(op1);
						op2=Operand_st(name);
						op3=Operand_temp();
						in2=Exp_translate(s->child[2],&op3);
						c=Intercode_2(op2,op3,IASSIGN);
						in3=prepare_code(c);
						c=Intercode_2(op,op2,IASSIGN);
						in4=prepare_code(c);
						in=combine_code(in1,combine_code(in2,combine_code(in3,in4)));
					}
					break;
				case VAND:
				case VOR:
				case VRELOP:
					op1=Operand_label();
					op2=Operand_label();
					opc0=Operand_const0();
					opc1=Operand_const1();
					c=Intercode_2(op,opc0,IASSIGN);
					in1=prepare_code(c);
					in2=Cond_translate(s,op1,op2);
					c=Intercode_1(op1,ILABEL);
					in3=prepare_code(c);
					c=Intercode_2(op,opc1,IASSIGN);
					in4=prepare_code(c);
					in=combine_code(in1,combine_code(in2,combine_code(in3,in4)));
					break;
				case VPLUS:
				case VMINUS:
				case VSTAR:
				case VDIV:
					op1=Operand_temp();
					op2=Operand_temp();
					in1=Exp_translate(s->child[0],&op1);
					in2=Exp_translate(s->child[2],&op2);
					c=Intercode_3(op,op1,op2,namemap(s->child[1]->name));
					in3=prepare_code(c);
					in=combine_code(in1,combine_code(in2,in3));
					break;
				case VLB:
				case VDOT:
					op1=Operand_temp();	
					in1=AS_translate(s);
					name=generate_name(op1);
					op2=Operand_st(name);
					c=Intercode_2(op,op2,IASSIGN);
					in2=prepare_code(c);
					in=combine_code(in1,in2);
					break;
			}
			break;
		case VLP:
			in=Exp_translate(s->child[1],&op);//?????
			break;
		case VMINUS:
			op1=Operand_temp();
			in1=Exp_translate(s->child[1],&op1);
			opc0=Operand_const0();
			c=Intercode_3(op,opc0,op1,VMINUS);
			in2=prepare_code(c);
			in=combine_code(in1,in2);
			break;
		case VNOT:
			op1=Operand_label();
			op2=Operand_label();
			opc0=Operand_const0();
			opc1=Operand_const1();
			c=Intercode_2(op,opc0,IASSIGN);
			in1=prepare_code(c);
			in2=Cond_translate(s,op1,op2);
			c=Intercode_1(op1,ILABEL);
			in3=prepare_code(c);
			c=Intercode_2(op,opc1,IASSIGN);
			in4=prepare_code(c);
			in=combine_code(in1,combine_code(in2,combine_code(in3,in4)));
			break;
	}			
	return in;
}

Intercodes* Cond_translate(Node *s, Operand *label_true, Operand *label_false)
{
	Intercodes *in=NULL,*in1,*in2,*in3,*in4;
	Operand *op1,*op2,*label1,*opc0;
	int relop;
	switch(namemap(s->name)){
		case VEXP:
			switch(namemap(s->child[1]->name)){
				case VRELOP:
					op1=Operand_temp();
					op2=Operand_temp();
					in1=Exp_translate(s->child[0],&op1);
					in2=Exp_translate(s->child[2],&op2);
					relop=s->child[1]->relop;
					c=Intercode_if(op1,op2,label_true,relop);
					in3=prepare_code(c);
					c=Intercode_1(label_false,IGOTO);
					in4=prepare_code(c);
					in=combine_code(in1,combine_code(in2,combine_code(in3,in4)));
					break;
				case VAND:
					label1=Operand_label();
					in1=Cond_translate(s->child[0],label1,label_false);
					c=Intercode_1(label1,ILABEL);					
					in2=prepare_code(c);
					in3=Cond_translate(s->child[2],label_true,label_false);
					in=combine_code(in1,combine_code(in2,in3));
					break;
				case VOR:
					label1=Operand_label();
					in1=Cond_translate(s->child[0],label_true,label1);
					c=Intercode_1(label1,ILABEL);					
					in2=prepare_code(c);
					in3=Cond_translate(s->child[2],label_true,label_false);
					in=combine_code(in1,combine_code(in2,in3));
					break;					
			}
			break;
		case VNOT:
			in=Cond_translate(s->child[1],label_false,label_true);
			break;
		default:
			op1=Operand_temp();
			in1=Exp_translate(s,&op1);
			opc0=Operand_const0();
			c=Intercode_if(op1,opc0,label_true,RNE);
			in2=prepare_code(c);
			c=Intercode_1(label_false,IGOTO);
			in3=prepare_code(c);
			in=combine_code(in1,combine_code(in2,in3));
			break;	
	}
	return in;
}
