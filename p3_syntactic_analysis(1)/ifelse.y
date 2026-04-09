%{
#include <stdio.h>
#include <string.h>
extern int yylex(void);
void yyerror(char *);
extern char *yytext;
extern int line;
extern int col;
%}

%token NATURAL IF THEN ELSE

%nonassoc LOWEST
%left '+' '-'
%left '*' '/'

%%

calculator: expr_list
          ;

expr_list: expression                   { printf("%d", $1); }
         | expr_list ',' expression     { printf(", %d", $3); }
         ;

expression: NATURAL                     { $$ = $1; }
          | '(' expression ')'          { $$ = $2; }
          | expression '+' expression   { $$ = $1 + $3; }
          | expression '-' expression   { $$ = $1 - $3; }
          | expression '*' expression   { $$ = $1 * $3; }
          | expression '/' expression   { $$ = $1 / $3; }
          | IF expression THEN expression ELSE expression %prec LOWEST { $$ = $2 ? $4 : $6; }
          ;

%%

void yyerror(char *error) {
    int error_col = col - strlen(yytext);
    printf("%s '%s' at line %d, column %d\n", error, yytext, line, error_col);
}