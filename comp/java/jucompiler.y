/* Gabriela Mendoza 2022227025
   Juan Manuel Flores de la Cruz 2025269252
*/
%{
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"


int yylex(void);
void yyerror(char *);
struct node *ast;

struct node *create_multiple_decls(enum category decl_type, struct node *type_node, struct node *first_id_node, struct node *extra_ids_container);
int count_children(struct node *n);
void unpack_nodes(struct node *parent, struct node *container);
%}

%union {
    char *lexeme;
    struct node *node;
}

%token CLASS PUBLIC STATIC LBRACE RBRACE LPAR RPAR LSQ RSQ SEMICOLON COMMA
%token BOOL INT DOUBLE VOID STRING IF WHILE RETURN PRINT PARSEINT 
%token ASSIGN PLUS MINUS STAR DIV MOD AND OR XOR LSHIFT RSHIFT EQ GE GT LE LT NE NOT DOTLENGTH // no se que es esto ultimo
%token RESERVED ARROW

%token <node> IDENTIFIER NATURAL DECIMAL BOOLLIT STRLIT
%type <node> Program ProgramBody Element MethodDecl FieldDecl FieldList
%type <node> Type TypeArray TypeVoid MethodHeader MethodParams FormalParams FormalParamsList
%type <node> MethodBody MethodBodyContent BodyElement VarDecl 
%type <node> Statement StmtList MethodInvocation Args ExprList 
%type <node> Assignment ParseArgs Expr SimpleExpr

/* precedencias y asociatividad */
%right ASSIGN
//En java es en este orden: 
%left AND
%left OR
%left XOR

%left EQ NE
%left LT GT LE GE
%left LSHIFT RSHIFT
%left PLUS MINUS
%left STAR DIV MOD
%right NOT UNARY // UNARY para MINUS/PLUS unario
%nonassoc IF_PREC
%right ELSE


%%

Program: CLASS IDENTIFIER LBRACE ProgramBody RBRACE {
$$ = ast = newnode(Program, NULL, 0, 0);
addchild($$, $2);
unpack_nodes(ast, $4);
};

ProgramBody: ProgramBody Element {  
        $$ = $1;
        if ($2 != NULL) {
            if ($2->category == MethodBody) { unpack_nodes($$, $2); }
            else { addchild($$, $2); }
        }
    }
    | /* empty */ { $$ = newnode(MethodBody, NULL, 0, 0); }
;

Element: MethodDecl { $$ = $1; }
       | FieldDecl { $$ = $1; }
       | SEMICOLON { $$ = NULL; } 
;
MethodDecl: PUBLIC STATIC MethodHeader MethodBody {
    $$ = newnode(MethodDecl, NULL, 0, 0);
    addchild($$, $3);
    addchild($$, $4);
}
| PUBLIC STATIC error RBRACE { $$ = NULL; }
| PUBLIC STATIC error SEMICOLON { $$ = NULL; }
| error RBRACE { $$ = NULL; };

FieldDecl: PUBLIC STATIC Type IDENTIFIER FieldList SEMICOLON {
    $$ = create_multiple_decls(FieldDecl, $3, $4, $5);
}
//Innecesaria si esta en MethodDecl | PUBLIC STATIC error SEMICOLON { $$ = NULL; }
| error SEMICOLON {$$=NULL;};

FieldList: FieldList COMMA IDENTIFIER {
    if ($1 == NULL) $$ = newnode(FieldDecl, NULL, 0, 0);
    else $$ = $1;
    addchild($$, $3);
}
| /* empty */ {$$ = NULL;};


Type: BOOL { $$ = newnode(Bool,NULL, 0, 0); }
    | INT { $$ = newnode(Int,NULL, 0, 0); }
    | DOUBLE { $$ = newnode(Double,NULL, 0, 0); };

TypeArray: STRING LSQ RSQ { $$ = newnode(StringArray, NULL, 0, 0); }
;
TypeVoid: VOID { $$ = newnode(VoidNode, NULL, 0, 0); }
;

MethodHeader: Type IDENTIFIER LPAR MethodParams RPAR {
    $$ = newnode(MethodHeader, NULL, 0, 0);
    addchild($$, $1);
    addchild($$, $2);
    addchild($$, $4);
    }
    |TypeVoid IDENTIFIER LPAR MethodParams RPAR {
    $$ = newnode(MethodHeader, NULL, 0, 0);
    addchild($$, $1);
    addchild($$, $2);
    addchild($$, $4);
    };

MethodParams: FormalParamsList {$$=$1;}
    | {$$ = newnode(MethodParams, NULL, 0, 0);};

FormalParamsList: FormalParamsList COMMA FormalParams { 
    $$ = $1;
    addchild($$, $3);
}
|FormalParams {
    $$=newnode(MethodParams, NULL, 0, 0);
    addchild($$, $1);}
;

FormalParams: Type IDENTIFIER {
    $$ = newnode(ParamDecl,NULL, 0, 0);
    addchild($$, $1);
    addchild($$, $2);
}
    | TypeArray IDENTIFIER  {
    $$ = newnode(ParamDecl,NULL, 0, 0);
    addchild($$, $1);
    addchild($$, $2);
            };


MethodBody: LBRACE MethodBodyContent RBRACE {
    $$ = $2;
};

MethodBodyContent: MethodBodyContent BodyElement {
    $$ = $1;
    if ($2 != NULL) {
        if ($2->category == MethodBody) { unpack_nodes($$, $2); } /* Si es un VarDecl múltiple */
        else { addchild($$, $2); }
    }
}
| /* empty */ { $$ = newnode(MethodBody, NULL, 0, 0); }
;

BodyElement: Statement {$$ = $1;}
| VarDecl {$$ = $1;};

VarDecl: Type IDENTIFIER FieldList SEMICOLON {
    $$ = create_multiple_decls(VarDecl, $1, $2, $3);
};

Statement: LBRACE StmtList RBRACE {
                                            int count = count_children($2);
                                            if (count == 0) {
                                                $$ = NULL;  // bloque vacío explícito o null??
                                            } else if (count == 1) {
                                                struct node_list *curr = $2->children;
                                                while (curr != NULL && curr->node == NULL) curr = curr->next;
                                                $$ = (curr != NULL) ? curr->node : newnode(Block, NULL, 0, 0);
                                            } else {
                                                $$ = newnode(Block, NULL, 0, 0);
                                                unpack_nodes($$, $2);
                                            }
                                        }
         | IF LPAR Expr RPAR Statement %prec IF_PREC {
                                                $$ = newnode(If, NULL, 0, 0);
                                                addchild($$, $3);
                                                    addchild($$, $5 ? $5 : newnode(Block, NULL, 0, 0));
                                                addchild($$, newnode(Block, NULL, 0, 0)); // else branch vacio
                                            }
         | IF LPAR Expr RPAR Statement ELSE Statement {
                                                $$ = newnode(If, NULL, 0, 0);
                                                addchild($$, $3);
                                                addchild($$, $5 ? $5 : newnode(Block, NULL, 0, 0));
                                        addchild($$, $7 ? $7 : newnode(Block, NULL, 0, 0));
                                            }
         | WHILE LPAR Expr RPAR Statement {
                                            $$ = newnode(While, NULL, 0, 0);
                                            addchild($$, $3);
                                                addchild($$, $5 ? $5 : newnode(Block, NULL, 0, 0));
                                                }
         | RETURN SEMICOLON {
    $$ = newnode(Return, NULL, 0, 0);
         }
         | RETURN Expr SEMICOLON {
    $$ = newnode(Return, NULL, 0, 0);
    addchild($$, $2);
         }
         | MethodInvocation SEMICOLON {$$ = $1;}
         | Assignment SEMICOLON {  $$ = $1;    }
         | ParseArgs SEMICOLON {$$ = $1; }
         | SEMICOLON    { $$ = NULL; }
         | PRINT LPAR Expr RPAR SEMICOLON {
    $$ = newnode(Print, NULL, 0, 0);
    addchild($$, $3);
         }
         | PRINT LPAR STRLIT RPAR SEMICOLON {
    $$ = newnode(Print, NULL, 0, 0);
    addchild($$, $3);
         }
         | error SEMICOLON {$$=NULL;}
         | error RBRACE {$$=NULL;}
;

StmtList: StmtList Statement {
    $$ = $1;
        if ($2 != NULL) addchild($$, $2);
}
         | /* empty */ {$$ = newnode(Aux, NULL, 0, 0); };

MethodInvocation: IDENTIFIER LPAR Args RPAR {
        $$ = newnode(Call, NULL, 0, 0);
        addchild($$, $1);
        unpack_nodes($$, $3);
    }
| IDENTIFIER LPAR error RPAR { $$ = NULL; }
;

Args: Expr ExprList {
        $$ = newnode(Aux, NULL, 0, 0); 
        addchild($$, $1);
        unpack_nodes($$, $2);
    }
    | /* empty */ { $$ = NULL; }
;

ExprList: ExprList COMMA Expr {
        if ($1 == NULL) $$ = newnode(Aux, NULL, 0, 0);
        else $$ = $1;
        addchild($$, $3);
    } 
    | /* empty */ { $$ = NULL; }
;
Assignment: IDENTIFIER ASSIGN Expr {
    $$ = newnode(Assign, NULL, 0, 0);
    addchild($$, $1);
    addchild($$, $3);
};


ParseArgs: PARSEINT LPAR IDENTIFIER LSQ Expr RSQ RPAR {
    $$ = newnode(ParseArgs, NULL, 0, 0);
    addchild($$, $3);
     addchild($$, $5);
}
         | PARSEINT LPAR error RPAR {$$ = NULL;}
         ;

Expr: Assignment {$$ = $1;}
    | SimpleExpr {$$ = $1;}
    ;

SimpleExpr: SimpleExpr PLUS SimpleExpr   { $$ = newnode(Add, NULL, 0, 0); addchild($$, $1); addchild($$, $3); }
          | SimpleExpr MINUS SimpleExpr  { $$ = newnode(Sub, NULL, 0, 0); addchild($$, $1); addchild($$, $3); }
          | SimpleExpr STAR SimpleExpr   { $$ = newnode(Mul, NULL, 0, 0); addchild($$, $1); addchild($$, $3); }
          | SimpleExpr DIV SimpleExpr    { $$ = newnode(Div, NULL, 0, 0); addchild($$, $1); addchild($$, $3); }
          | SimpleExpr MOD SimpleExpr    { $$ = newnode(Mod, NULL, 0, 0); addchild($$, $1); addchild($$, $3); }
          | SimpleExpr AND SimpleExpr    { $$ = newnode(And, NULL, 0, 0); addchild($$, $1); addchild($$, $3); }
          | SimpleExpr OR SimpleExpr     { $$ = newnode(Or, NULL, 0, 0); addchild($$, $1); addchild($$, $3); }
          | SimpleExpr XOR SimpleExpr    { $$ = newnode(Xor, NULL, 0, 0); addchild($$, $1); addchild($$, $3); }
          | SimpleExpr LSHIFT SimpleExpr { $$ = newnode(Lshift, NULL, 0, 0); addchild($$, $1); addchild($$, $3); }
          | SimpleExpr RSHIFT SimpleExpr { $$ = newnode(Rshift, NULL, 0, 0); addchild($$, $1); addchild($$, $3); }
          | SimpleExpr EQ SimpleExpr     { $$ = newnode(Eq, NULL, 0, 0); addchild($$, $1); addchild($$, $3); }
          | SimpleExpr NE SimpleExpr     { $$ = newnode(Ne, NULL, 0, 0); addchild($$, $1); addchild($$, $3); }
          | SimpleExpr GT SimpleExpr     { $$ = newnode(Gt, NULL, 0, 0); addchild($$, $1); addchild($$, $3); }
          | SimpleExpr GE SimpleExpr     { $$ = newnode(Ge, NULL, 0, 0); addchild($$, $1); addchild($$, $3); }
          | SimpleExpr LT SimpleExpr     { $$ = newnode(Lt, NULL, 0, 0); addchild($$, $1); addchild($$, $3); }
          | SimpleExpr LE SimpleExpr     { $$ = newnode(Le, NULL, 0, 0); addchild($$, $1); addchild($$, $3); }
          | MINUS SimpleExpr %prec UNARY { $$ = newnode(Minus, NULL, 0, 0); addchild($$, $2); }
          | PLUS SimpleExpr %prec UNARY  { $$ = newnode(Plus, NULL, 0, 0); addchild($$, $2); }
          | NOT SimpleExpr               { $$ = newnode(Not, NULL, 0, 0); addchild($$, $2); }
          | LPAR Expr RPAR               { $$ = $2; }
          | LPAR error RPAR              { $$ = NULL; }
          | MethodInvocation             { $$ = $1; }
          | ParseArgs                    { $$ = $1; }
          | IDENTIFIER                   { $$ = $1; }
          | IDENTIFIER DOTLENGTH         {
              $$ = newnode(Length, NULL, 0, 0);
              addchild($$, $1);
            }   
          | NATURAL                      { $$ = $1; }
          | DECIMAL                      { $$ = $1; }
          | BOOLLIT                      { $$ = $1; }
          ;
%%


void unpack_nodes(struct node *parent, struct node *container) {
    if (container == NULL || container->children == NULL) return;
    struct node_list *curr = container->children;
    while (curr != NULL) {
        
            addchild(parent, curr->node);
        
        curr = curr->next;
    }
}

struct node *create_multiple_decls(enum category decl_type, struct node *type_node, struct node *id_node, struct node *extra_ids_container) {
    struct node *wrapper = newnode(MethodBody, NULL,0,0); 
    
    struct node *first = newnode(decl_type, NULL,0,0);
    addchild(first, type_node);
    addchild(first, id_node);
    addchild(wrapper, first);

    if (extra_ids_container != NULL) {
        struct node_list *curr = extra_ids_container->children;
        while (curr != NULL ) {
            if (curr->node != NULL) {
            struct node *extra = newnode(decl_type, NULL,0,0);
            addchild(extra, newnode(type_node->category, NULL,0,0)); 
            addchild(extra, curr->node);
            addchild(wrapper, extra);
        }
        curr = curr->next;
        }
    }
    return wrapper;
}