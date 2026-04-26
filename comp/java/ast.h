#ifndef _AST_H
#define _AST_H
#include "sym_table.h"

//He puesto todas las que he encontrado, vamos adaptando
enum category {
    Program, FieldDecl, VarDecl, MethodDecl, MethodHeader, MethodParams,
    ParamDecl, MethodBody, Block, If, While, Return, Call, Print, ParseArgs,
    Assign, Or, And, Eq, Ne, Lt, Gt, Le, Ge, Add, Sub, Mul, Div, Mod, Lshift,
    Rshift, Xor, Not, Minus, Plus, Length, Bool, BoolLit, Double, Decimal,
    Identifier, Int, Natural, StrLit, StringArray, VoidNode, Reserved
};
struct node {
    enum category category;
    char *token;
    int line, col;
    BasicType annot_type;
    struct node_list *children;
};

struct node_list {
    struct node *node;
    struct node_list *next;
};

struct node *newnode(enum category category, char *token , int line, int col);
void addchild(struct node *parent, struct node *child);
void show_annotated(struct node *n, int depth);
struct node *get_child(struct node *n, int index);
void show(struct node *n, int depth);
#endif
