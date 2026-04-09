%{
#include "y.tab.h"
#include <string.h>
extern int yylval;
int yyparse(void);

int line = 1;
int col = 1;
%}

digit   [0-9]

%%

"if"                    { col += yyleng; return IF; }
"then"                  { col += yyleng; return THEN; }
"else"                  { col += yyleng; return ELSE; }
{digit}+                { yylval = atoi(yytext); col += yyleng; return NATURAL; }
[()=,*/+-]              { col += yyleng; return yytext[0]; }
[\t ]+                  { col += yyleng; }
\n                      { line++; col = 1; return 0; }
.                       { col += yyleng; printf("Unrecognized character\n"); }

%%

int main() {
    yyparse();
    printf("\n");
    return 0;
}

int yywrap() {
    return 1;
}