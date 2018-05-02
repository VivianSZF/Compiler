#include <stdio.h>
#include "treeop.h"
#include "syntax.tab.h"

extern int errornot;

int main(int argc, char** argv){
    if(argc<=1) return 1;
    FILE *f=fopen(argv[1], "r");
    if(!f){
        perror(argv[1]);
        return 1;
    }
    printf("begin\n");
    yyrestart(f);
    errornot=0;
    yyparse();
    if(errornot==0){
        preorderprint(root,0);
    }
    printf("finish!!!\n");
    return 0;
}
