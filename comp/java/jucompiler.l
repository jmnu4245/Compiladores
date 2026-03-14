/* Gabriela Mendoza 2022227025
   Juan Cruz 2025269252
*/

%{
        /* place here any C variables and definitions */
    #include <stdio.h>
    #include <string.h>
    #include <stdlib.h>
    int LineCounter = 1;
    int ColumnCounter = 1;
    int print_tokens = 0;
    int str_start_line, str_start_col;
    int string_erro = 0;

    char *str=NULL;
%}
%X COMMENT STRING

digit   [0-9]
dec        {digit}(({digit}|"_")*{digit})?
letter [a-zA-Z$_]
dotlength ".length"
identifier {letter}({letter}|{digit})*  
natural    0|([1-9](({digit}|"_")*{digit})?)
exponent   [eE][+-]?{dec}
decimal   ({dec}"."{dec}?{exponent}?)|("."{dec}{exponent}?)|({dec}{exponent})
boollit "true"|"false"
and "&&"
assign "="
star "*"
comma ","
eq "=="
ge ">="
gt ">"
lbrace "{"
le "<="
lpar "("
lsq "["
lt "<"
minus "-"
mod "%"
ne "!="
or "||"
plus "+"
div "/"
rbrace "}"
rpar ")"
rsq "]"
semicolon ";"
arrow "->"
lshift "<<"
rshift ">>"
xor "^"
not "!"
bool "boolean"
class "class"
else "else"
if "if"
int "int"
double "double"
print "System.out.print"
parseInt "Integer.parseInt"
public "public"
return "return"
static "static"
string "String"
void "void"
while "while"
reserved "++"|"--"|"null"|"Integer"|"System"|"abstract"|"assert"|"break"|"byte"|"case"|"catch"|"char"|"const"|"continue"|"default"|"do"|"enum"|"extends"|"final"|"finally"|"float"|"for"|"goto"|"implements"|"import"|"instanceof"|"interface"|"long"|"native"|"new"|"package"|"private"|"protected"|"short"|"strictfp"|"super"|"switch"|"synchronized"|"this"|"throw"|"throws"|"transient"|"try"|"volatile"
whitespace [ \f]
tab "\t"

%%
"/*" { 
    BEGIN(COMMENT); 
    str_start_line = LineCounter; 
    str_start_col = ColumnCounter; 
    ColumnCounter += 2; 
}
<COMMENT>"*/" { 
    ColumnCounter += 2; 
    BEGIN(0); 
}
<COMMENT>\n|\r\n|\r { LineCounter++; ColumnCounter = 1; }

<COMMENT>[^*\n\r]+ { ColumnCounter += yyleng; }
<COMMENT>"*"       { ColumnCounter++; }

<COMMENT><<EOF>> { 
    printf("Line %d, col %d: unterminated comment\n", str_start_line, str_start_col); 
    return 0; 
}

"\""               { BEGIN(STRING); str_start_line = LineCounter; str_start_col = ColumnCounter; ColumnCounter++; string_erro = 0; if(str) free(str); str = strdup(""); }
<STRING>\"         { if(print_tokens && !string_erro) printf("STRLIT(\"%s\")\n",str); if(str){ free(str); str = NULL; } ColumnCounter++; BEGIN(0);}
<STRING>(\\n|\\f|\\r|\\t|\\\\|\\\") { 
    char *tmp = malloc(strlen(str) + strlen(yytext) + 1);
    strcpy(tmp, str);
    strcat(tmp, yytext);
    free(str);
    str = tmp;
    ColumnCounter +=2; }
<STRING>\\(.|[\n]|[\r]) {  
    string_erro = 1;
    if(yytext[1] == '\n' || yytext[1] == '\r') {
        printf("Line %d, col %d: invalid escape sequence (\\)\n", LineCounter, ColumnCounter);
        yyless(1);
    } else {
        printf("Line %d, col %d: invalid escape sequence (%s)\n", LineCounter, ColumnCounter, yytext);
        ColumnCounter += 2;
    }
}
<STRING>\n|\r\n|\r { 
    printf("Line %d, col %d: unterminated string literal\n", str_start_line, str_start_col);
    if(str) { free(str); str = NULL; }
    LineCounter++; 
    ColumnCounter = 1; 
    BEGIN(0);
}
<STRING><<EOF>> { 
    printf("Line %d, col %d: unterminated string literal\n", str_start_line, str_start_col);
    if(str) { free(str); str = NULL; }
    return 0;
}
<STRING>. { 
    char *tmp = malloc(strlen(str) + yyleng + 1);
    strcpy(tmp, str);
    strcat(tmp, yytext);
    free(str);
    str = tmp;
    ColumnCounter += yyleng;
}

"//"[^\r\n]* {}

{whitespace}    { ColumnCounter++; }
{tab}           { ColumnCounter++; }
\n|\r\n|\r    { LineCounter++; ColumnCounter = 1; }

{reserved}      { if(print_tokens) printf("RESERVED(%s)\n", yytext);
                  ColumnCounter += yyleng; }
{boollit}       { if(print_tokens) printf("BOOLLIT(%s)\n", yytext); ColumnCounter += yyleng; }
{and}           { if(print_tokens) printf("AND\n"); ColumnCounter += yyleng; }
{or}            { if(print_tokens) printf("OR\n"); ColumnCounter += yyleng; }
{eq}            { if(print_tokens) printf("EQ\n"); ColumnCounter += yyleng; }
{ne}            { if(print_tokens) printf("NE\n"); ColumnCounter += yyleng; }
{ge}            { if(print_tokens) printf("GE\n"); ColumnCounter += yyleng; }
{le}            { if(print_tokens) printf("LE\n"); ColumnCounter += yyleng; }
{lshift}        { if(print_tokens) printf("LSHIFT\n"); ColumnCounter += yyleng; }
{rshift}        { if(print_tokens) printf("RSHIFT\n"); ColumnCounter += yyleng; }
{arrow}         { if(print_tokens) printf("ARROW\n"); ColumnCounter += yyleng; }
{bool}          { if(print_tokens) printf("BOOL\n"); ColumnCounter += yyleng; }
{class}         { if(print_tokens) printf("CLASS\n"); ColumnCounter += yyleng; }
{else}          { if(print_tokens) printf("ELSE\n"); ColumnCounter += yyleng; }
{if}            { if(print_tokens) printf("IF\n"); ColumnCounter += yyleng; }
{int}           { if(print_tokens) printf("INT\n"); ColumnCounter += yyleng; }
{double}        {if(print_tokens) printf("DOUBLE\n"); ColumnCounter += yyleng; }
{print}         { if(print_tokens) printf("PRINT\n"); ColumnCounter += yyleng; }
{parseInt}      { if(print_tokens) printf("PARSEINT\n"); ColumnCounter += yyleng; }
{public}        { if(print_tokens) printf("PUBLIC\n"); ColumnCounter += yyleng; }
{return}        { if(print_tokens) printf("RETURN\n"); ColumnCounter += yyleng; }
{static}        { if(print_tokens) printf("STATIC\n"); ColumnCounter += yyleng; }
{string}        { if(print_tokens) printf("STRING\n"); ColumnCounter += yyleng; }
{void}          { if(print_tokens) printf("VOID\n"); ColumnCounter += yyleng; }
{while}         { if(print_tokens) printf("WHILE\n"); ColumnCounter += yyleng; }
{dotlength}     { if(print_tokens) printf("DOTLENGTH\n"); ColumnCounter += yyleng; }
{assign}        { if(print_tokens) printf("ASSIGN\n"); ColumnCounter += yyleng; }
{star}          { if(print_tokens) printf("STAR\n"); ColumnCounter += yyleng; }
{comma}         { if(print_tokens) printf("COMMA\n"); ColumnCounter += yyleng; }
{div}           { if(print_tokens) printf("DIV\n"); ColumnCounter += yyleng; }
{gt}            { if(print_tokens) printf("GT\n"); ColumnCounter += yyleng; }
{lt}            { if(print_tokens) printf("LT\n"); ColumnCounter += yyleng; }
{lbrace}        { if(print_tokens) printf("LBRACE\n"); ColumnCounter += yyleng; }
{rbrace}        { if(print_tokens) printf("RBRACE\n"); ColumnCounter += yyleng; }
{lpar}          { if(print_tokens) printf("LPAR\n"); ColumnCounter += yyleng; }
{rpar}          { if(print_tokens) printf("RPAR\n"); ColumnCounter += yyleng; }
{lsq}           { if(print_tokens) printf("LSQ\n"); ColumnCounter += yyleng; }
{rsq}           { if(print_tokens) printf("RSQ\n"); ColumnCounter += yyleng; }
{semicolon}     { if(print_tokens) printf("SEMICOLON\n"); ColumnCounter += yyleng; }
{minus}         { if(print_tokens) printf("MINUS\n"); ColumnCounter += yyleng; }
{mod}           { if(print_tokens) printf("MOD\n"); ColumnCounter += yyleng; }
{plus}          { if(print_tokens) printf("PLUS\n"); ColumnCounter += yyleng; }
{xor}           { if(print_tokens) printf("XOR\n"); ColumnCounter += yyleng; }
{not}           { if(print_tokens) printf("NOT\n"); ColumnCounter += yyleng; }

{decimal}       { if(print_tokens) printf("DECIMAL(%s)\n",yytext); ColumnCounter+=yyleng; }
{natural}       { if(print_tokens) printf("NATURAL(%s)\n",yytext); ColumnCounter+=yyleng; }
{identifier}    { if(print_tokens) printf("IDENTIFIER(%s)\n",yytext); ColumnCounter+=yyleng;}

"."             { printf("Line %d, col %d: illegal character (.)\n", LineCounter, ColumnCounter); ColumnCounter++; }
.               { printf("Line %d, col %d: illegal character (%s)\n", LineCounter, ColumnCounter, yytext); ColumnCounter++; }
%%
extern int yylex();
int main(int argc, char *argv[]) {
    LineCounter=1;
    ColumnCounter=1;
    if (argc > 1 && strcmp(argv[1], "-l") == 0) {
        print_tokens = 1;
    }
    yylex();    /* run the lexical analysis automaton */

    if(str) {
        free(str);
        str = NULL;
    }
    return 0;
}
int yywrap() {  /* called on EOF, return 1 to terminate */
    return 1;
}