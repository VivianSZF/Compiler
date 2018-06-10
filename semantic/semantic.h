#ifndef __SEMANTIC_H_
#define __SEMANTIC_H_

#include "../lexical_syntax/treeop.h"
#include "../intercode/intercodes.h"
#include "../intercode/translate.h"
#include "symbol_table.h"

enum {
	VINT,VFLOAT,VID,VSEMI,VCOMMA,VASSIGNOP,VRELOP,VPLUS,VMINUS,VSTAR,VDIV,VAND,VOR,
	VDOT,VNOT,VTYPE,VLP,VRP,VLB,VRB,VLC,VRC,VSTRUCT,VRETURN,VIF,VELSE,VWHILE,VProgram,
	VExtDefList,VExtDef,VExtDecList,VSpecifier,VStructSpecifier,VOptTag,VTag,
	VVarDec,VFunDec,VVarList,VParamDec,VCompSt,VStmtList,VStmt,VDefList,VDef,
	VDecList,VDec,VExp,VArgs,VUnknown
};


int errortable[19][500];

int namemap(char *name);
int error_search(int errortype, int lineno);
void Program_analysis(Node *root);
Intercodes* ExtDefList_analysis(Node *s);
Intercodes* ExtDef_analysis(Node *s);
void ExtDecList_analysis(Node *s, Type *type);
Type* Specifier_analysis(Node *s);
Type* StructSpecifier_analysis(Node *s);
Symbolele* VarDec_analysis(Node *s, Type *type);
Func* FunDec_analysis(Node *s, Type *type, int defordec);
Func* VarList_analysis(Node *s,Type *type);
Symbolele* ParamDec_analysis(Node *s);
Intercodes* CompSt_analysis(Node *s, Func *func);
Intercodes* StmtList_analysis(Node *s, Type *type);
Intercodes* Stmt_analysis(Node *s,Type *type);
Intercodes* DefList_analysis(Node *s,Type *type);
Intercodes* Def_analysis(Node *s, Type *type);
Intercodes* DecList_analysis(Node *s,Type *type,Type *type1);
Intercodes* Dec_analysis(Node *s,Type *type,Type *type1);
Exp* Exp_analysis(Node *s);
Args* Args_analysis(Node *s);


#endif
