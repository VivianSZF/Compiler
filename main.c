#include <stdio.h>
#include "treeop.h"
#include "syntax.tab.h"

extern int errornot;

int main(int argc, char** argv){
    if(argc<=1) return 1;
	int i;
	for(i=1;i<argc;i++){
    	FILE *f=fopen(argv[1], "r");
    	if(!f){
        	perror(argv[1]);
        	return 1;
    	}
    	yyrestart(f);
    	errornot=0;
   		yyparse();
    	if(errornot==0){
        	preorderprint(root,0);
    	}
		fclose(f);
	}
    return 0;
}
