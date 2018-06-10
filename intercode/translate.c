#include "translate.h"


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

Intercodes* Exp_translate(Node *s, Operand *op)
{
	Intercodes *in=NULL;
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
					
					break;	
				case 4:
					arglist=Args_analysis(s->child[2]);
					if(symbol==NULL){
						//error 2
						if(error_search(2,s->lineno)==0)
							printf("Error type 2 at Line %d: Undefined function \"%s\".\n",s->lineno,s->child[0]->id_name);
						exp->type=NULL;
					}
					else if(symbol->funcornot==0){
						//error 11
						if(error_search(11,s->lineno)==0)
							printf("Error type 11 at Line %d: \"%s\" is not a function.\n",s->lineno,s->child[0]->id_name);
						exp->type=NULL;
					}
					else{
						ar1=symbol->func->args;
						ar2=arglist;
						int pan=0;
						while(ar1!=NULL&&ar2!=NULL){
							if(type_equiv_detect(ar1->a->type,ar2->a->type)==0){
								pan=0;
								break;
							}
							ar1=ar1->next;ar2=ar2->next;
						}
						if(ar1==NULL && ar1==NULL) pan=1;
						if(pan==0){
							//error 9
							if(error_search(9,s->lineno)==0)
								printf("Error type 9 at Line %d: Argument(s) is(are) incorrect for function \"%s\".\n",s->lineno,s->child[0]->id_name);
							exp->type=NULL;
						}
						else{
							exp->type=symbol->func->type;
							exp->lorr=VR;
						}
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
					
					break;
				case VAND:
				case VOR:
					expr=Exp_analysis(s->child[2]);
					if(expl->type==NULL||expr->type==NULL||expl->type->kind!=VTINT||expr->type->kind!=VTINT){
						//error 7
						if(error_search(7,s->lineno)==0)
							printf("Error type 7 at Line %d: Type mismatched for operands.\n",s->lineno);
						exp->type=NULL;
					}
					else{
						exp->type=expl->type;
						exp->lorr=VR;
					}
					break;
				case VRELOP:
				case VPLUS:
				case VMINUS:
				case VSTAR:
				case VDIV:
				
					
					break;
				case VLB:
					
					break;
				case VDOT:
					
					break;
				default:
					printf("Error!\n");
					break;
			}
			break;
		case VLP:
			exp=Exp_analysis(s->child[1]);
			break;
		case VMINUS:
			expr=Exp_analysis(s->child[1]);
			if(expr->type==NULL||!(expr->type->kind==VTINT||expr->type->kind==VTFLOAT)){
				//error 7
				if(error_search(7,s->lineno)==0)
					printf("Error type 7 at Line %d: Type mismatched for operands.\n",s->lineno);
				exp->type=NULL;
			}
			else{
				exp->type=expr->type;
				exp->lorr=VR;
			}
			break;
		case VNOT:
			expr=Exp_analysis(s->child[1]);
			if(expr==NULL||expr->type->kind!=VTINT){
				//error 7
				if(error_search(7,s->lineno)==0)
					printf("Error type 7 at Line %d: Type mismatched for operands.\n",s->lineno);
				exp->type=NULL;
			}
			else{
				exp->type=expr->type;
				exp->lorr=VR;
			}
			break;			

		case VINT:
			exp->type=(Type*)malloc(sizeof(Type));
			exp->type->kind=VTINT;
			exp->lorr=VR;
			break;
		case VFLOAT:
			exp->type=(Type*)malloc(sizeof(Type));
			exp->type->kind=VTFLOAT;
			exp->lorr=VR;
			break;
		default:
			printf("Error!\n");
			break;
	}
	return exp;
}

