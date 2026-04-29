/* Gabriela Mendoza 2022227025
   Juan Manuel Flores de la Cruz 2025269252
*/
%{
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"


int yylex(void);
void yyerror(char *s); /* 1. Volvemos a un solo argumento */
    
#define YYERROR_CALL(msg) yyerror(msg) 
struct node *ast;

struct node *create_multiple_decls(enum category decl_type, struct node *type_node, char *first_id_token,  struct node *extra_ids_container, YYLTYPE decl_pos, YYLTYPE id_pos);
int count_children(struct node *n);
void unpack_nodes(struct node *parent, struct node *container);
%}

%locations

%union {
    char *lexeme;
    struct node *node;
}

%token CLASS PUBLIC STATIC LBRACE RBRACE LPAR RPAR LSQ RSQ SEMICOLON COMMA
%token BOOL INT DOUBLE VOID STRING IF WHILE RETURN PRINT PARSEINT 
%token ASSIGN PLUS MINUS STAR DIV MOD AND OR XOR LSHIFT RSHIFT EQ GE GT LE LT NE NOT DOTLENGTH ARROW
%token <lexeme> IDENTIFIER NATURAL DECIMAL BOOLLIT STRLIT RESERVED

%type <node> Program ProgramBody Element MethodDecl FieldDecl FieldList
%type <node> Type TypeArray TypeVoid MethodHeader MethodParams FormalParamsList
%type <node> MethodBody MethodBodyContent BodyElement VarDecl 
%type <node> Statement MethodInvocation Args ExprList StmtList
%type <node> Assignment ParseArgs Expr SimpleExpr

/* precedencias y asociatividad */
%right ASSIGN
%left OR
%left AND
%left XOR
%left EQ NE
%left LT GT LE GE
%left LSHIFT RSHIFT
%left PLUS MINUS
%left STAR DIV MOD
%right NOT UNARY

%nonassoc IF_PREC
%right ELSE


%%

Program: CLASS IDENTIFIER LBRACE ProgramBody RBRACE {
    $$ = ast = newnode(Program, NULL, @$);
    addchild($$, newnode(Identifier, $2, @2));
    unpack_nodes(ast, $4);
};

ProgramBody: ProgramBody Element {  
        $$ = $1;
        if ($2 != NULL) {
            if ($2->category == MethodBody) { unpack_nodes($$, $2); }
            else { addchild($$, $2); }
        }
    }
    | /* empty */ { $$ = newnode(Aux, NULL, @$); }
;

Element: MethodDecl { $$ = $1; }
       | FieldDecl { $$ = $1; }
       | SEMICOLON { $$ = NULL; }
;

MethodDecl: PUBLIC STATIC MethodHeader MethodBody {
    $$ = newnode(MethodDecl, NULL, @$);
    addchild($$, $3);
    addchild($$, $4);
};

FieldDecl: PUBLIC STATIC Type IDENTIFIER FieldList SEMICOLON {
    $$ = create_multiple_decls(FieldDecl, $3, $4, $5, @$, @4);
}
| error SEMICOLON { $$ = NULL; };

FieldList: FieldList COMMA IDENTIFIER {
    if ($1 == NULL) $$ = newnode(FieldDecl, NULL, @$);
    else $$ = $1;
    addchild($$, newnode(Identifier, $3, @3));
}
| /* empty */ { $$ = NULL; };

Type: BOOL { $$ = newnode(Bool, NULL, @1); }
    | INT { $$ = newnode(Int, NULL, @1); }
    | DOUBLE { $$ = newnode(Double, NULL, @1); };

TypeArray: STRING LSQ RSQ { $$ = newnode(StringArray, NULL, @1); };

TypeVoid: VOID { $$ = newnode(VoidNode, NULL, @1); };

MethodHeader: Type IDENTIFIER LPAR MethodParams RPAR {
    $$ = newnode(MethodHeader, NULL, @$);
    addchild($$, $1);
    addchild($$, newnode(Identifier, $2, @2));
    addchild($$, $4);
}
| TypeVoid IDENTIFIER LPAR MethodParams RPAR {
    $$ = newnode(MethodHeader, NULL, @$);
    addchild($$, $1);
    addchild($$, newnode(Identifier, $2, @2));
    addchild($$, $4);
};

MethodParams: FormalParamsList { $$ = $1; }
            | TypeArray IDENTIFIER {
                $$ = newnode(MethodParams, NULL, @$);
                struct node *p = newnode(ParamDecl, NULL, @$);
                addchild(p, $1);
                addchild(p, newnode(Identifier, $2, @2));
                addchild($$, p);
            }
            | /* empty */ { $$ = newnode(MethodParams, NULL, @$); }
;

FormalParamsList: FormalParamsList COMMA Type IDENTIFIER { 
                $$ = $1;
                struct node *p = newnode(ParamDecl, NULL, @$);
                addchild(p, $3);
                addchild(p, newnode(Identifier, $4, @4));
                addchild($$, p);
            }
            | Type IDENTIFIER {
                $$ = newnode(MethodParams, NULL, @$);
                struct node *p = newnode(ParamDecl, NULL, @$);
                addchild(p, $1);
                addchild(p, newnode(Identifier, $2, @2));
                addchild($$, p);
            }
;

MethodBody: LBRACE MethodBodyContent RBRACE {
    $$ = $2;
};

MethodBodyContent: MethodBodyContent BodyElement {
    $$ = $1;
    if ($2 != NULL) {
        if ($2->category == MethodBody) { unpack_nodes($$, $2); }
        else { addchild($$, $2); }
    }
}
| /* empty */ { $$ = newnode(MethodBody, NULL, @$); }
;

BodyElement: Statement {$$ = $1;}
           | VarDecl {$$ = $1;}
;

VarDecl: Type IDENTIFIER FieldList SEMICOLON {
    $$ = create_multiple_decls(VarDecl, $1, $2, $3, @$, @2);
}
;

StmtList: StmtList Statement { 
    $$ = $1; 
    if ($2 != NULL) addchild($$, $2); 
}
| /* empty */ { $$ = newnode(Aux, NULL, @$); }
;

Statement: LBRACE StmtList RBRACE {
               int count = count_children($2);
               if (count == 0) {
                   $$ = NULL;
               } else if (count == 1) {
                   struct node_list *curr = $2->children;
                   while (curr != NULL && curr->node == NULL) curr = curr->next;
                   $$ = curr->node;
               } else {
                   $$ = newnode(Block, NULL, @$);
                   unpack_nodes($$, $2);
               }
           }
         | IF LPAR Expr RPAR Statement %prec IF_PREC {
               $$ = newnode(If, NULL, @1);
               addchild($$, $3);
               addchild($$, $5 ? $5 : newnode(Block, NULL, @$));
               addchild($$, newnode(Block, NULL, @$)); 
           }
         | IF LPAR Expr RPAR Statement ELSE Statement {
               $$ = newnode(If, NULL, @1);
               addchild($$, $3);
               addchild($$, $5 ? $5 : newnode(Block, NULL, @$));
               addchild($$, $7 ? $7 : newnode(Block, NULL, @$));
           }
         | WHILE LPAR Expr RPAR Statement {
               $$ = newnode(While, NULL, @1);
               addchild($$, $3);
               addchild($$, $5 ? $5 : newnode(Block, NULL, @$));
           }
         | RETURN SEMICOLON {
               $$ = newnode(Return, NULL, @1);
           }
         | RETURN Expr SEMICOLON {
               $$ = newnode(Return, NULL, @1);
               addchild($$, $2);
           }
         | MethodInvocation SEMICOLON {$$ = $1;}
         | Assignment SEMICOLON { $$ = $1; }
         | ParseArgs SEMICOLON {$$ = $1; }
         | SEMICOLON { $$ = NULL; }
         | PRINT LPAR Expr RPAR SEMICOLON {
               $$ = newnode(Print, NULL, @1);
               addchild($$, $3);
           }
         | PRINT LPAR STRLIT RPAR SEMICOLON {
               $$ = newnode(Print, NULL, @1);
               addchild($$, newnode(StrLit, $3, @3));
           }
         | error SEMICOLON { $$ = NULL; }
;

MethodInvocation: IDENTIFIER LPAR Args RPAR {
        $$ = newnode(Call, NULL, @1);
        addchild($$, newnode(Identifier, $1, @1));
        unpack_nodes($$, $3);
    }
    | IDENTIFIER LPAR error RPAR { $$ = NULL; }
;

Args: Expr ExprList {
        $$ = newnode(Aux, NULL, @$);
        addchild($$, $1);
        unpack_nodes($$, $2);
    }
    | /* empty */ { $$ = NULL; }
;

ExprList: ExprList COMMA Expr {
        if ($1 == NULL) $$ = newnode(Aux, NULL, @$);
        else $$ = $1;
        addchild($$, $3);
    } 
    | /* empty */ { $$ = NULL; }
;

Assignment: IDENTIFIER ASSIGN Expr {
    $$ = newnode(Assign, NULL, @2);
    addchild($$, newnode(Identifier, $1, @1));
    addchild($$, $3);
};

ParseArgs: PARSEINT LPAR IDENTIFIER LSQ Expr RSQ RPAR {
    $$ = newnode(ParseArgs, NULL, @1);
    addchild($$, newnode(Identifier, $3, @3));
    addchild($$, $5);
}
| PARSEINT LPAR error RPAR {$$ = NULL;}
;

Expr: Assignment {$$ = $1;}
    | SimpleExpr {$$ = $1;}
;

SimpleExpr: SimpleExpr PLUS SimpleExpr   { $$ = newnode(Add, NULL, @2); addchild($$, $1); addchild($$, $3); }
          | SimpleExpr MINUS SimpleExpr  { $$ = newnode(Sub, NULL, @2); addchild($$, $1); addchild($$, $3); }
          | SimpleExpr STAR SimpleExpr   { $$ = newnode(Mul, NULL, @2); addchild($$, $1); addchild($$, $3); }
          | SimpleExpr DIV SimpleExpr    { $$ = newnode(Div, NULL, @2); addchild($$, $1); addchild($$, $3); }
          | SimpleExpr MOD SimpleExpr    { $$ = newnode(Mod, NULL, @2); addchild($$, $1); addchild($$, $3); }
          | SimpleExpr AND SimpleExpr    { $$ = newnode(And, NULL, @2); addchild($$, $1); addchild($$, $3); }
          | SimpleExpr OR SimpleExpr     { $$ = newnode(Or, NULL, @2); addchild($$, $1); addchild($$, $3); }
          | SimpleExpr XOR SimpleExpr    { $$ = newnode(Xor, NULL, @2); addchild($$, $1); addchild($$, $3); }
          | SimpleExpr LSHIFT SimpleExpr { $$ = newnode(Lshift, NULL, @2); addchild($$, $1); addchild($$, $3); }
          | SimpleExpr RSHIFT SimpleExpr { $$ = newnode(Rshift, NULL, @2); addchild($$, $1); addchild($$, $3); }
          | SimpleExpr EQ SimpleExpr     { $$ = newnode(Eq, NULL, @2); addchild($$, $1); addchild($$, $3); }
          | SimpleExpr NE SimpleExpr     { $$ = newnode(Ne, NULL, @2); addchild($$, $1); addchild($$, $3); }
          | SimpleExpr GT SimpleExpr     { $$ = newnode(Gt, NULL, @2); addchild($$, $1); addchild($$, $3); }
          | SimpleExpr GE SimpleExpr     { $$ = newnode(Ge, NULL, @2); addchild($$, $1); addchild($$, $3); }
          | SimpleExpr LT SimpleExpr     { $$ = newnode(Lt, NULL, @2); addchild($$, $1); addchild($$, $3); }
          | SimpleExpr LE SimpleExpr     { $$ = newnode(Le, NULL, @2); addchild($$, $1); addchild($$, $3); }
          | MINUS SimpleExpr %prec UNARY { $$ = newnode(Minus, NULL, @1); addchild($$, $2); }
          | PLUS SimpleExpr %prec UNARY  { $$ = newnode(Plus, NULL, @1); addchild($$, $2); }
          | NOT SimpleExpr               { $$ = newnode(Not, NULL, @1); addchild($$, $2); }
          | LPAR Expr RPAR               { $$ = $2; }
          | LPAR error RPAR              { $$ = NULL; }
          | MethodInvocation             { $$ = $1; }
          | ParseArgs                    { $$ = $1; }
          | IDENTIFIER                   { $$ = newnode(Identifier, $1, @1); }
          | IDENTIFIER DOTLENGTH         {
              $$ = newnode(Length, NULL, @2);
              addchild($$, newnode(Identifier, $1, @1));
            }   
          | NATURAL                      { $$ = newnode(Natural, $1, @1); }
          | DECIMAL                      { $$ = newnode(Decimal, $1, @1); }
          | BOOLLIT                      { $$ = newnode(BoolLit, $1, @1); }
          ;
%%

struct node *create_multiple_decls(enum category decl_type, struct node *type_node, char *first_id_token, struct node *extra_ids_container, YYLTYPE decl_pos, YYLTYPE id_pos) {
    struct node *wrapper = newnode(Aux, NULL, decl_pos); 
    
    struct node *first = newnode(decl_type, NULL, decl_pos);
    addchild(first, type_node);
    addchild(first, newnode(Identifier, first_id_token, id_pos));
    addchild(wrapper, first);

    if (extra_ids_container != NULL) {
        struct node_list *curr = extra_ids_container->children;
        while (curr != NULL ) {
            if (curr->node != NULL) {
                struct node *extra = newnode(decl_type, NULL, decl_pos);
                addchild(extra, newnode(type_node->category, NULL, decl_pos)); 
                addchild(extra, curr->node);
                addchild(wrapper, extra);
            }
            curr = curr->next;
        }
    }
    return wrapper;
}

