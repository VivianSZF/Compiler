//#ifndef __TREEOP_H__
//#define __TREEOP_H__

enum{ID,TYPE,INT,FLOAT,UN,TOKEN};
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

Node* root;

struct Node* create_node(int ntype, char* name, int lineno, char* value);
struct Node* add_node(int ntype,char* name, int lineno,char*value, int num_of_c, ...);
void preorderprint(struct Node* node, int cnt);
