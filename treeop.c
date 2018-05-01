#include <stdio.h>
#include <stdlib.h>
#include "treeop.h"
#include <string.h>

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
        case ID:
            newnode->id_name=malloc(sizeof(char)*(strlen(value)+1));
            break;
        case TYPE:
            if(strcmp(value,"int")==0)
                newnode->type=type_int;
            else newnode->type=type_float;
            break;
        case INT:
            newnode->int_value=atoi(value);
            break;
        case FLOAT:
            newnode->float_value=(float)atof(value);
            break;
        default:
            break;
    }    
    return newnode;
}


Node* add_node(int ntype,char* name, int lineno,char*value, int num_of_c, ...)
{
    va_list ap;
    va_start(ap,num_of_c);    
    Node *newnode=create_node(ntype,name,lineno,value);
    int i;
    for(i=0;i<num_of_c;i++)
    {
        Node* child;
        child=va_arg(ap, Node*);
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
    printf("%s: ",node->name);
    switch(node->ntype){
        case ID:
            printf("%s",node->id_name);
            break;
        case TYPE:
            if(node->type==type_int)
                printf("INT");
            else
                printf("FLOAT");
            break;
        case INT:
            printf("%d",node->int_value);
            break;
        case FLOAT:
            printf("%f",node->float_value);
            break;
        case TOKEN:
            printf("(%d)",node->lineno);
            break;
    }
    for(j=0;j<node->childnum;j++)
    {
        preorderprint(node->child[j],cnt+1);
    }
}
