%{
#include <stdio.h>
extern int yylex(void);
void yyerror(char *);
extern char *yytext;
int results[100];
int n=0;
%}

%token NATURAL
%nonassoc '(' ')'
%left ','
%left '+' '-'
%left '*' '/'


%%

calculator: expression                  {
    for(int i=0;i<n-1;i++){
         printf("%d,", results[i]); 
    }
    printf("%d\n", results[n-1]); 
}
          ;

expression: NATURAL                     { $$ = $1; }
          | '(' expression ')' {$$ = $2;}
          | expression '+' expression   { $$ = $1 + $3; }
          | expression '-' expression   { $$ = $1 - $3; }
          | expression '*' expression   { $$ = $1 * $3; }
          | expression '/' expression   { $$ = $1 / $3; }
          | expression ',' expression   {   results[n]=$1; results[n+1]=$3;n=n+2;}
          ;

%%

void yyerror(char *error) {
    printf("%s '%s'\n", error, yytext);
}
