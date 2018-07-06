#include <stdio.h>
#include "lexical_syntax/treeop.h"
#include "lexical_syntax/syntax.tab.h"
#include "semantic/semantic.h"
#include "intercode/intercode.h"
#include "intercode/translate.h"
#include "objectcode/objectcode.h"

extern int errornot;
extern int mark;
extern int errorlineno;
extern int yylineno;
extern void yyrestart(FILE* input_file);
extern Intercodes *in_head;

int main(int argc, char** argv){
	if(argc<=1) return 1;
	int i;
	for(i=1;i<argc;i=i+2){
		FILE *f=fopen(argv[i], "r");
		if(!f){
			perror(argv[1]);
			return 1;
		}
		yyrestart(f);
		printf("In file %s:\n",argv[i]);
		//initialization
		errornot=0;
		mark=-1;
		errorlineno=-1;
		yylineno=1;
		
		yyparse();
		if(errornot==0){
			//preorderprint(root,0);
			Program_analysis(root);
			FILE *f1=fopen(argv[i+1],"w");
			//print_Intercodes(in_head,f1);
			objectcode(in_head,f1);
			fclose(f1);
		}
		fclose(f);
	}
	return 0;
}
