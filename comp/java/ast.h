#include "y.tab.h"
#ifndef _AST_H
#define _AST_H
#include "sym_table.h"

enum category {
    Program, FieldDecl, VarDecl, MethodDecl, MethodHeader, MethodParams,
    ParamDecl, MethodBody, Block, If, While, Return, Call, Print, ParseArgs,
    Assign, Or, And, Eq, Ne, Lt, Gt, Le, Ge, Add, Sub, Mul, Div, Mod, Lshift,
    Rshift, Xor, Not, Minus, Plus, Length, Bool, BoolLit, Double, Decimal,
    Identifier, Int, Natural, StrLit, StringArray, VoidNode, Aux
};
struct node {
    enum category category;
    char *token;
    int line, col;
    BasicType annot_type;
    char     *annot_params; 
    struct node_list *children;
};

struct node_list {
    struct node *node;
    struct node_list *next;
};

struct node *newnode(enum category category, char *token , YYLTYPE position);
int count_children(struct node *n);
void addchild(struct node *parent, struct node *child);
void show_annotated(struct node *n, int depth);
struct node *get_child(struct node *n, int index);
void show(struct node *n, int depth);
void check_node(struct node *n);
int count_children(struct node *n);
void unpack_nodes(struct node *parent, struct node *container);
#endif
