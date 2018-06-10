#include "intercode.h"
#include <stdio.h>

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

void init_code(Intercodes *in1)
{
	in1=NULL;
}
