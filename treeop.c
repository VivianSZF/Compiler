#include <stdio.h>
#include <stdlib.h>
#include "treeop.h"
#include <string.h>
#include <stdarg.h>

Node* root=NULL;
int errornot=0;

Node* create_node(int ntype, char* name, int lineno, char* value)
{
    Node *newnode=malloc(sizeof(struct Node));
    newnode->ntype=ntype;
    newnode->name=malloc((sizeof(char))*(strlen(name)+1));
    strcpy(newnode->name, name);    
    newnode->lineno=lineno;
    newnode->childnum=0;
    newnode->child=malloc(sizeof(struct Node*));
    switch(ntype){
        case TID:
            newnode->id_name=malloc(sizeof(char)*(strlen(value)+1));
            strcpy(newnode->id_name,value);
            break;
        case TTYPE:
            if(strcmp(value,"int")==0)
                newnode->type=type_int;
            else newnode->type=type_float;
            break;
        case TINT:
            newnode->int_value=atoi(value);
            break;
        case TFLOAT:
            newnode->float_value=(float)atof(value);
            break;
        default:
            break;
    }    
    return newnode;
}


Node* add_node(int ntype,char* name, int lineno, int num_of_c, ...)
{
    va_list ap;
    va_start(ap,num_of_c);    
    Node *newnode=create_node(ntype,name,lineno,NULL);
    int i;
    for(i=0;i<num_of_c;i++)
    {
        struct Node* child;
        child=va_arg(ap,struct Node*);
        if (newnode->childnum>0)
        {    
            void *p=realloc(newnode->child,(sizeof(struct Node*)*(newnode->childnum+1)));
            if(!p) printf("cannot reallocate memory\n");
            newnode->child=p;
        }
        newnode->child[newnode->childnum]=child;
        newnode->childnum++;
    }
    va_end(ap);
    return newnode;
}

void preorderprint(Node* node, int cnt)
{
    if(node==NULL) return;
    int i,j;
    for(i=0;i<cnt*2;i++) printf(" ");
    printf("%s",node->name);
    switch(node->ntype){
        case TID:
            printf(": %s\n",node->id_name);
            break;
        case TTYPE:
            if(node->type==type_int)
                printf(": int\n");
            else
                printf(": float\n");
            break;
        case TINT:
            printf(": %d\n",node->int_value);
            break;
        case TFLOAT:
            printf(": %f\n",node->float_value);
            break;
        case TTOKEN:
            printf(" (%d)\n",node->lineno);
            break;
        default:
            printf("\n");
            break;
    }
    for(j=0;j<node->childnum;j++)
        preorderprint(node->child[j],cnt+1);
}
