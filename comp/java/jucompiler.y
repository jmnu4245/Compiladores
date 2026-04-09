// jucompiler.y
%{
#include <stdio.h>
#include <stdlib.h>

int yylex(void);
void yyerror(char *);
extern char *yytext;
%}

%union {
    char *lexeme;
}

%token CLASS PUBLIC STATIC LBRACE RBRACE LPAR RPAR LSQ RSQ SEMICOLON COMMA
%token BOOL INT DOUBLE VOID STRING IF WHILE RETURN PRINT PARSEINT
%token ASSIGN PLUS MINUS STAR DIV MOD AND OR XOR LSHIFT RSHIFT EQ GE GT LE LT NE NOT DOTLENGTH
%token <lexeme> IDENTIFIER NATURAL DECIMAL BOOLLIT STRLIT

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

Program: CLASS IDENTIFIER LBRACE ProgramBody RBRACE ;

ProgramBody: ProgramBody Element | /* empty */ ;

Element: MethodDecl | FieldDecl | SEMICOLON ;

MethodDecl: PUBLIC STATIC MethodHeader MethodBody ;

FieldDecl: PUBLIC STATIC Type IDENTIFIER FieldList SEMICOLON
         | error SEMICOLON ;

FieldList: FieldList COMMA IDENTIFIER |;

Type: BOOL | INT | DOUBLE ;

MethodHeader: Type IDENTIFIER LPAR MethodParams RPAR
            | VOID IDENTIFIER LPAR MethodParams RPAR ;

MethodParams: 
            | FormalParams ;

FormalParams: Type IDENTIFIER FormalParamsList
            | STRING LSQ RSQ IDENTIFIER ;

FormalParamsList: FormalParamsList COMMA Type IDENTIFIER |  ;

MethodBody: LBRACE MethodBodyContent RBRACE ;

MethodBodyContent: MethodBodyContent BodyElement | ;

BodyElement: Statement | VarDecl ;

VarDecl: Type IDENTIFIER FieldList SEMICOLON ;

Statement: LBRACE StmtList RBRACE
         | IF LPAR Expr RPAR Statement %prec IF_PREC
         | IF LPAR Expr RPAR Statement ELSE Statement
         | WHILE LPAR Expr RPAR Statement
         | RETURN SEMICOLON
         | RETURN Expr SEMICOLON
         | MethodInvocation SEMICOLON
         | Assignment SEMICOLON
         | ParseArgs SEMICOLON
         | SEMICOLON
         | PRINT LPAR Expr RPAR SEMICOLON
         | PRINT LPAR STRLIT RPAR SEMICOLON
         | error SEMICOLON ;

StmtList: StmtList Statement 
         | /* empty */ ;

MethodInvocation: IDENTIFIER LPAR Args RPAR
                | IDENTIFIER LPAR error RPAR ;

Args: Expr ExprList | ;

ExprList: ExprList COMMA Expr |;

Assignment: IDENTIFIER ASSIGN Expr ;

ParseArgs: PARSEINT LPAR IDENTIFIER LSQ Expr RSQ RPAR
         | PARSEINT LPAR error RPAR ;

Expr: Expr PLUS Expr
    | Expr MINUS Expr
    | Expr STAR Expr
    | Expr DIV Expr
    | Expr MOD Expr
    | Expr AND Expr
    | Expr OR Expr
    | Expr XOR Expr
    | Expr LSHIFT Expr
    | Expr RSHIFT Expr
    | Expr EQ Expr
    | Expr NE Expr
    | Expr GT Expr
    | Expr GE Expr
    | Expr LT Expr
    | Expr LE Expr
    | MINUS Expr %prec UNARY
    | PLUS Expr %prec UNARY
    | NOT Expr
    | LPAR Expr RPAR
    | LPAR error RPAR
    | MethodInvocation
    | Assignment
    | ParseArgs
    | IDENTIFIER
    | IDENTIFIER DOTLENGTH
    | NATURAL
    | DECIMAL
    | BOOLLIT ;

%%

void yyerror(char *s) {
    
}