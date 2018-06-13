#include <stdio.h>
#include "lexical_syntax/treeop.h"
#include "lexical_syntax/syntax.tab.h"
#include "semantic/semantic.h"
#include "intercode/intercode.h"
#include "intercode/translate.h"

extern int errornot;
extern int mark;
extern int errorlineno;
extern int yylineno;
extern void yyrestart(FILE* input_file);
extern Intercodes *in_head;

int main(int argc, char** argv){
	if(argc<=1) return 1;
	int i;
	for(i=1;i<argc;i++){
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
			print_Intercodes(in_head);
		}
		fclose(f);
	}
	return 0;
}
