/* START definitions section -- C code delimited by %{ ... %} and token declarations */

%{

#include <stdio.h>
#include "ast.h"

int yylex(void);
void yyerror(YYLTYPE *loc, const char *msg);
struct node *ast;

%}

%locations

%token INTEGER DOUBLE IF THEN ELSE
%token<lexeme> IDENTIFIER NATURAL DECIMAL
%type<node> program function parameters parameter arguments expression
%type<node_list> functions

%left LOW
%left '+' '-'
%left '*' '/'

%union{
    char *lexeme;
    struct node *node;
    struct node_list *node_list;
}

/* START grammar rules section -- BNF grammar */

%%

program: functions                  { ast = $$ = newnode(Program, NULL, @1);
                                      addchildren($$, $1); }
    ;

functions: function                 { $$ = newlist();
                                      append($$, $1); }
    | functions function            { append($$, $2); }
    ;

function: IDENTIFIER '(' parameters ')' '=' expression
                                    { /* Usa directamente @1 como acordamos */
                                      $$ = newnode(Function, NULL, @1);
                                      addchild($$, newnode(Identifier, $1, @1));
                                      /* addchild no necesita ubicación, el nodo $3 ya la tiene */
                                      addchild($$, $3);
                                      addchild($$, $6); }
    ;

parameters: parameter               { $$ = newnode(Parameters, NULL, @1);
                                      addchild($$, $1); }
    | parameters ',' parameter      { $$ = $1;
                                      addchild($$, $3); }
    ;

parameter: INTEGER IDENTIFIER       { $$ = newnode(Parameter, NULL, @1);
                                      addchild($$, newnode(Integer, NULL, @1));
                                      addchild($$, newnode(Identifier, $2, @2)); }
    | DOUBLE IDENTIFIER             { $$ = newnode(Parameter, NULL, @1);
                                      addchild($$, newnode(Double, NULL, @1));
                                      addchild($$, newnode(Identifier, $2, @2)); }
    ;

arguments: expression               { $$ = newnode(Arguments, NULL, @1);
                                      addchild($$, $1); }
    | arguments ',' expression      { $$ = $1;
                                      addchild($$, $3); }
    ;

expression: IDENTIFIER              { $$ = newnode(Identifier, $1, @1);}
    | NATURAL                       { $$ = newnode(Natural, $1, @1); $$->type = integer_type;}
    | DECIMAL                       { $$ = newnode(Decimal, $1, @1); $$->type = double_type; }
    | IDENTIFIER '(' arguments ')'  { $$ = newnode(Call, NULL, @1);
                                      addchild($$, newnode(Identifier, $1, @1));
                                      addchild($$, $3); }
    | IF expression THEN expression ELSE expression  %prec LOW
                                    { $$ = newnode(If, NULL, @1);
                                      addchild($$, $2);
                                      addchild($$, $4);
                                      addchild($$, $6); }
    | expression '+' expression     { $$ = newnode(Add, NULL, @2);
                                      addchild($$, $1);
                                      addchild($$, $3); }
    | expression '-' expression     { $$ = newnode(Sub, NULL, @2);
                                      addchild($$, $1);
                                      addchild($$, $3); }
    | expression '*' expression     { $$ = newnode(Mul, NULL, @2);
                                      addchild($$, $1);
                                      addchild($$, $3); }
    | expression '/' expression     { $$ = newnode(Div, NULL, @2);
                                      addchild($$, $1);
                                      addchild($$, $3); }
    | '(' expression ')'            { $$ = $2; }  
    ;

%%
/* START subroutines section */

// all needed functions are collected in the .l and ast.* files
