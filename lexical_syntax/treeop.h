#ifndef __TREEOP_H__
#define __TREEOP_H__

enum{TID,TTYPE,TINT,TFLOAT,TUN,TTOKEN};
#define type_int 0
#define type_float 1

struct Node{
    int ntype;
    char* name;
    int lineno;    
    int childnum;
    struct Node **child;
    union{
        char* id_name;
        int type;
        int int_value;
        float float_value;
    }; 
};

typedef struct Node Node;

extern Node* root;

extern int errornot;

extern struct Node* create_node(int ntype, char* name, int lineno, char* value);
extern struct Node* add_node(int ntype,char* name, int lineno, int num_of_c, ...);
extern void preorderprint(struct Node* node, int cnt);

#endif