#include <stdio.h>
#include <stdlib.h>
#include "treeop.h"
#include <string.h>
#include <stdarg.h>

Node* root=NULL;
int errornot=0;

int hextodec(char* value)
{
    int t,len,i;
    len = strlen(value);
    long long sum=0;
    for(i=2;i<len;i++){
        if(value[i]<='9')
            t=value[i]-'0';
        else
            t=value[i]-'A'+10;
        sum=sum*16+t;
    }    
    return sum;
}

int octtodec(char* value)
{
    int t,len,i;
    len = strlen(value);
    long long sum=0;
    for(i=1;i<len;i++){
        t=value[i]-'0';
        sum=sum*8+t;
    }    
    return sum;    
}

int hexornot(char* value)
{
    int len,i;
    len=strlen(value);
    for(i=2;i<len;i++){
        if((value[i]>'F'&&value[i]<='Z')||(value[i]>'f'&&value[i]<='z'))
            return 1;
    }
    return 0;
}

int octornot(char* value)
{
    int len,i;
    len=strlen(value);
    for(i=1;i<len;i++){
        if(value[i]>'7')
            return 1;
    }
    return 0;
}

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
            if(strlen(value)>1){
                if(value[0]=='0'&&(value[1]=='x'||value[1]=='X')){
                    int k;
                    k=hexornot(value);
                    if(k==1){
                        errornot=1;
                        printf("Error type A at Line %d: Illegal hexadecimal number \'%s\'.\n", lineno, value);
                    }
                    else
                        newnode->int_value=hextodec(value);
                }
                else if(value[0]=='0'){
                    int k;
                    k=octornot(value);
                    if(k==1){
                        errornot=1;
                        printf("Error type A at Line %d: Illegal octal number \'%s\'.\n", lineno, value);
                    }
                    else
                        newnode->int_value=octtodec(value);
                }               
                else
                    newnode->int_value=atoi(value);
            }
            else     
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
