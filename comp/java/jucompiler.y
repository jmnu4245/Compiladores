// jucompiler.y
%{
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"


int yylex(void);
void yyerror(char *);
struct node *ast;

struct node *create_multiple_decls(enum category decl_type, struct node *type_node, char *first_id_token, struct node *extra_ids_container);
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
%token <lexeme> IDENTIFIER NATURAL DECIMAL BOOLLIT STRLIT

%type <node> Program ProgramBody Element MethodDecl FieldDecl FieldList
%type <node> Type TypeArray TypeVoid MethodHeader MethodParams FormalParams FormalParamsList
%type <node> MethodBody MethodBodyContent BodyElement VarDecl 
%type <node> Statement StmtList MethodInvocation Args ExprList 
%type <node> Assignment ParseArgs Expr

/* precedencias y asociatividad */
%right ASSIGN
//En java es en este orden: 
%left OR
%left XOR
%left AND

%left EQ NE
%left LT GT LE GE
%left LSHIFT RSHIFT
%left PLUS MINUS
%left STAR DIV MOD
%right NOT UNARY // UNARY para MINUS/PLUS unario
%nonassoc IF_PREC
%nonassoc ELSE

%%

Program: CLASS IDENTIFIER LBRACE ProgramBody RBRACE {
$$ = ast = newnode(Program, NULL);
addchild($$, newnode(Identifier, $2));
unpack_nodes(ast, $4);
};

ProgramBody: ProgramBody Element {  
        $$ = $1;
        if ($2 != NULL) {
            if ($2->category == MethodBody) { unpack_nodes($$, $2); }
            else { addchild($$, $2); }
        }
    }
    | Element { 
        $$ = newnode(MethodBody, NULL); 
        if ($1 != NULL) {
            if ($1->category == MethodBody) { unpack_nodes($$, $1); }
            else { addchild($$, $1); }
        }
    }
    | /* empty */ { $$ = newnode(MethodBody, NULL); }
;

Element: MethodDecl { $$ = $1; }
       | FieldDecl { $$ = $1; }
       | SEMICOLON { $$ = NULL; } 
;
MethodDecl: PUBLIC STATIC MethodHeader MethodBody {
    $$ = newnode(MethodDecl, NULL);
    addchild($$, $3);
    addchild($$, $4);
}
| PUBLIC STATIC error RBRACE { $$ = NULL; }
| PUBLIC STATIC error SEMICOLON { $$ = NULL; }
| error RBRACE { $$ = NULL; };

FieldDecl: PUBLIC STATIC Type IDENTIFIER FieldList SEMICOLON {
    $$ = create_multiple_decls(FieldDecl, $3, $4, $5);
}
| PUBLIC STATIC error SEMICOLON { $$ = NULL; }
    | error SEMICOLON {$$=NULL;};

FieldList: FieldList COMMA IDENTIFIER {
    if ($1 == NULL) $$ = newnode(FieldDecl, NULL);
    else $$ = $1;
    addchild($$, newnode(Identifier, $3));
}
| /* empty */ {$$ = NULL;};


Type: BOOL { $$ = newnode(Bool,NULL); }
    | INT { $$ = newnode(Int,NULL); }
    | DOUBLE { $$ = newnode(Double,NULL); };

TypeArray: STRING LSQ RSQ { $$ = newnode(StringArray, NULL); }
;
TypeVoid: VOID { $$ = newnode(VoidNode, NULL); }
;

MethodHeader: Type IDENTIFIER LPAR MethodParams RPAR {
    $$ = newnode(MethodHeader, NULL);
    addchild($$, $1);
    addchild($$, newnode(Identifier, $2));
    addchild($$, $4);
    }
    | VOID IDENTIFIER LPAR MethodParams RPAR {
    $$ = newnode(MethodHeader, NULL);
    addchild($$, newnode(VoidNode, NULL));
    addchild($$, newnode(Identifier, $2));
    addchild($$, $4);
    };

MethodParams: FormalParamsList {$$=$1;}
    | {$$ = newnode(MethodParams, NULL);};

FormalParamsList: FormalParamsList COMMA FormalParams { 
    $$ = $1;
    addchild($$, $3);
}
|FormalParams {
    $$=newnode(MethodParams, NULL);
    addchild($$, $1);}
;

FormalParams: Type IDENTIFIER {
    $$ = newnode(ParamDecl,NULL);
    addchild($$, $1);
    addchild($$, newnode(Identifier, $2));
}
    | TypeArray IDENTIFIER  {
    $$ = newnode(ParamDecl,NULL);
    addchild($$, $1);
    addchild($$, newnode(Identifier, $2));
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
| BodyElement {
    $$ = newnode(MethodBody, NULL); 
    if ($1 != NULL) {
        if ($1->category == MethodBody) { unpack_nodes($$, $1); }
        else { addchild($$, $1); }
    }
}
| /* empty */ { $$ = newnode(MethodBody, NULL); }
;

BodyElement: Statement {$$ = $1;}
| VarDecl {$$ = $1;};

VarDecl: Type IDENTIFIER FieldList SEMICOLON {
    $$ = create_multiple_decls(VarDecl, $1, $2, $3);
};

Statement: LBRACE StmtList RBRACE {
    int count = count_children($2);
    if (count == 0) {
        $$ = newnode(Block, NULL);  // ← bloque vacío explícito
    } else if (count == 1) {
        struct node_list *curr = $2->children;
        while (curr != NULL && curr->node == NULL) curr = curr->next;
        $$ = (curr != NULL) ? curr->node : newnode(Block, NULL);
    } else {
        $$ = newnode(Block, NULL);
        unpack_nodes($$, $2);
    }
}
         | IF LPAR Expr RPAR Statement %prec IF_PREC {
            $$ = newnode(If, NULL);
            addchild($$, $3);
                addchild($$, $5 ? $5 : newnode(Block, NULL));
            addchild($$, newnode(Block, NULL)); // else branch vacio
         }
         | IF LPAR Expr RPAR Statement ELSE Statement {
            $$ = newnode(If, NULL);
            addchild($$, $3);
            addchild($$, $5 ? $5 : newnode(Block, NULL));
    addchild($$, $7 ? $7 : newnode(Block, NULL));
         }
         | WHILE LPAR Expr RPAR Statement {
    $$ = newnode(While, NULL);
    addchild($$, $3);
        addchild($$, $5 ? $5 : newnode(Block, NULL));
         }
         | RETURN SEMICOLON {
    $$ = newnode(Return, NULL);
         }
         | RETURN Expr SEMICOLON {
    $$ = newnode(Return, NULL);
    addchild($$, $2);
         }
         | MethodInvocation SEMICOLON {$$ = $1;}
         | Assignment SEMICOLON {  $$ = $1;    }
         | ParseArgs SEMICOLON {$$ = $1; }
         | SEMICOLON    { $$ = NULL; }
         | PRINT LPAR Expr RPAR SEMICOLON {
    $$ = newnode(Print, NULL);
    addchild($$, $3);
         }
         | PRINT LPAR STRLIT RPAR SEMICOLON {
    $$ = newnode(Print, NULL);
    addchild($$, newnode(StrLit, $3));
         }
         | error SEMICOLON {$$=NULL;}
         | error RBRACE {$$=NULL;}
;

StmtList: StmtList Statement {
    $$ = $1;
        if ($2 != NULL) addchild($$, $2);
}
         | /* empty */ {$$ = newnode(MethodBody, NULL); };

MethodInvocation: IDENTIFIER LPAR Args RPAR {
        $$ = newnode(Call, NULL);
        addchild($$, newnode(Identifier, $1));
        unpack_nodes($$, $3);
    }
;

Args: Expr ExprList {
        $$ = newnode(Program, NULL); //Temporal
        addchild($$, $1);
        unpack_nodes($$, $2);
    }
    | /* empty */ { $$ = NULL; }
;

ExprList: ExprList COMMA Expr {
        if ($1 == NULL) $$ = newnode(Program, NULL);
        else $$ = $1;
        addchild($$, $3);
    } 
    | /* empty */ { $$ = NULL; }
;
Assignment: IDENTIFIER ASSIGN Expr {
    $$ = newnode(Assign, NULL);
    addchild($$, newnode(Identifier, $1));
    addchild($$, $3);
};

ParseArgs: PARSEINT LPAR IDENTIFIER LSQ Expr RSQ RPAR {
    $$ = newnode(ParseArgs, NULL);
    addchild($$, newnode(Identifier, $3));
     addchild($$, $5);
}
         | PARSEINT LPAR error RPAR {$$ = NULL;}
         ;


Expr: Expr PLUS Expr   { $$ = newnode(Add, NULL); addchild($$, $1); addchild($$, $3); }
    | Expr MINUS Expr  { $$ = newnode(Sub, NULL); addchild($$, $1); addchild($$, $3); }
    | Expr STAR Expr   { $$ = newnode(Mul, NULL); addchild($$, $1); addchild($$, $3); }
    | Expr DIV Expr    { $$ = newnode(Div, NULL); addchild($$, $1); addchild($$, $3); }
    | Expr MOD Expr    { $$ = newnode(Mod, NULL); addchild($$, $1); addchild($$, $3); }
    | Expr AND Expr    { $$ = newnode(And, NULL); addchild($$, $1); addchild($$, $3); }
    | Expr OR Expr     { $$ = newnode(Or, NULL); addchild($$, $1); addchild($$, $3); }
    | Expr XOR Expr    { $$ = newnode(Xor, NULL); addchild($$, $1); addchild($$, $3); }
    | Expr LSHIFT Expr { $$ = newnode(Lshift, NULL); addchild($$, $1); addchild($$, $3); }
    | Expr RSHIFT Expr { $$ = newnode(Rshift, NULL); addchild($$, $1); addchild($$, $3); }
    | Expr EQ Expr     { $$ = newnode(Eq, NULL); addchild($$, $1); addchild($$, $3); }
    | Expr NE Expr     { $$ = newnode(Ne, NULL); addchild($$, $1); addchild($$, $3); }
    | Expr GT Expr     { $$ = newnode(Gt, NULL); addchild($$, $1); addchild($$, $3); }
    | Expr GE Expr     { $$ = newnode(Ge, NULL); addchild($$, $1); addchild($$, $3); }
    | Expr LT Expr     { $$ = newnode(Lt, NULL); addchild($$, $1); addchild($$, $3); }
    | Expr LE Expr     { $$ = newnode(Le, NULL); addchild($$, $1); addchild($$, $3); }
    | MINUS Expr %prec UNARY { $$ = newnode(Minus, NULL); addchild($$, $2); }
    | PLUS Expr %prec UNARY  { $$ = newnode(Plus, NULL); addchild($$, $2); }
    | NOT Expr               { $$ = newnode(Not, NULL); addchild($$, $2); }
    | LPAR Expr RPAR    {$$ = $2;}
    | LPAR error RPAR {$$ = NULL;}
    | MethodInvocation  {$$=$1;}
    | Assignment    {$$=$1;}
    | ParseArgs   {$$=$1;}
    | IDENTIFIER {$$=newnode(Identifier,$1);}
    | IDENTIFIER DOTLENGTH {
        $$ = newnode(Length, NULL);
        addchild($$, newnode(Identifier, $1));}
    | NATURAL {$$=newnode(Natural,$1); }
    | DECIMAL {$$=newnode(Decimal,$1);}
    | BOOLLIT {$$=newnode(BoolLit,$1);}
    | STRLIT {$$=newnode(StrLit,$1); }
;
%%
int count_children(struct node *n) {
    if (n == NULL || n->children == NULL) return 0;
    int count = 0;
    struct node_list *curr = n->children;
    while (curr != NULL) {
        if (curr->node != NULL) count++;
        curr = curr->next;
    }
    return count;
}

void unpack_nodes(struct node *parent, struct node *container) {
    if (container == NULL || container->children == NULL) return;
    struct node_list *curr = container->children;
    while (curr != NULL) {
        if (curr->node != NULL) {
            addchild(parent, curr->node);
        }
        curr = curr->next;
    }
}

struct node *create_multiple_decls(enum category decl_type, struct node *type_node, char *first_id_token, struct node *extra_ids_container) {
    struct node *wrapper = newnode(MethodBody, NULL); 
    
    struct node *first = newnode(decl_type, NULL);
    addchild(first, type_node);
    addchild(first, newnode(Identifier, first_id_token));
    addchild(wrapper, first);

    if (extra_ids_container != NULL) {
        struct node_list *curr = extra_ids_container->children;
        while (curr != NULL ) {
            if (curr->node != NULL) {
            struct node *extra = newnode(decl_type, NULL);
            addchild(extra, newnode(type_node->category, NULL)); 
            addchild(extra, curr->node);
            addchild(wrapper, extra);
        }
        curr = curr->next;
        }
    }
    return wrapper;
}