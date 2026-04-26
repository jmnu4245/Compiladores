/* Gabriela Mendoza 2022227025
   Juan Manuel Flores de la Cruz 2025269252
*/

%{
        /* place here any C variables and definitions */
    #include <stdio.h>
    #include <string.h>
    #include <stdlib.h>

    #include "sym_table.h"
    #include "y.tab.h"
    #include "ast.h"
    #include "sym_table.h"  
    #include "semantics.h"

    void check_semantics(struct node *n);
    void print_sym_table(SymTable *table);

    #define PASS_TOKEN(cat) yylval.node = newnode(cat, strdup(yytext), LineCounter, ColumnCounter);
    int yyparse(void);

    int LineCounter = 1;
    int ColumnCounter = 1;
    int LastColumnCounter = 1; 

    int print_tokens = 0;
    int str_start_line, str_start_col;
    int string_erro = 0;
    int syntax_error = 0;

    char *str=NULL;

    extern struct node *ast;
    extern int yychar;

%}
%option noyywrap
%X COMMENT STR_STATE

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

"\""               { LastColumnCounter = ColumnCounter;  BEGIN(STR_STATE); str_start_line = LineCounter; str_start_col = ColumnCounter; ColumnCounter++; string_erro = 0; if(str) free(str); str = strdup(""); }
<STR_STATE>\" {
    ColumnCounter++;
    BEGIN(0);
    if (!string_erro) {
        char *with_quotes = malloc(strlen(str) + 3);
        sprintf(with_quotes, "\"%s\"", str);
        if(print_tokens) printf("STRLIT(\"%s\")\n", str);
        yylval.lexeme = with_quotes;
        if(str){ free(str); str = NULL; }
        return STRLIT;
    }
    if(str){ free(str); str = NULL; }
}
<STR_STATE>(\\n|\\f|\\r|\\t|\\\\|\\\") { 
    char *tmp = malloc(strlen(str) + strlen(yytext) + 1);
    strcpy(tmp, str);
    strcat(tmp, yytext);
    free(str);
    str = tmp;
    ColumnCounter +=2; }
<STR_STATE>\\(.|[\n]|[\r]) {  
    string_erro = 1;
    if(yytext[1] == '\n' || yytext[1] == '\r') {
        printf("Line %d, col %d: invalid escape sequence (\\)\n", LineCounter, ColumnCounter);
        yyless(1);
    } else {
        printf("Line %d, col %d: invalid escape sequence (%s)\n", LineCounter, ColumnCounter, yytext);
        ColumnCounter += 2;
    }
}
<STR_STATE>\n|\r\n|\r { 
    printf("Line %d, col %d: unterminated string literal\n", str_start_line, str_start_col);
    if(str) { free(str); str = NULL; }
    LineCounter++; 
    ColumnCounter = 1; 
    BEGIN(0);
}
<STR_STATE><<EOF>> { 
    printf("Line %d, col %d: unterminated string literal\n", str_start_line, str_start_col);
    if(str) { free(str); str = NULL; }
    return 0;
}
<STR_STATE>. { 
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

{boollit}       { LastColumnCounter = ColumnCounter; if(print_tokens) printf("BOOLLIT(%s)\n", yytext); PASS_TOKEN(BoolLit); ColumnCounter += yyleng; return BOOLLIT; }
{and}           { LastColumnCounter = ColumnCounter; if(print_tokens) printf("AND\n"); ColumnCounter += yyleng; return AND; }
{or}            { LastColumnCounter = ColumnCounter; if(print_tokens) printf("OR\n"); ColumnCounter += yyleng; return OR; }
{eq}            { LastColumnCounter = ColumnCounter; if(print_tokens) printf("EQ\n"); ColumnCounter += yyleng; return EQ; }
{ne}            { LastColumnCounter = ColumnCounter; if(print_tokens) printf("NE\n"); ColumnCounter += yyleng; return NE; }
{ge}            { LastColumnCounter = ColumnCounter; if(print_tokens) printf("GE\n"); ColumnCounter += yyleng; return GE; }
{le}            { LastColumnCounter = ColumnCounter; if(print_tokens) printf("LE\n"); ColumnCounter += yyleng; return LE; }
{lshift}        { LastColumnCounter = ColumnCounter; if(print_tokens) printf("LSHIFT\n"); ColumnCounter += yyleng; return LSHIFT; }
{rshift}        { LastColumnCounter = ColumnCounter; if(print_tokens) printf("RSHIFT\n"); ColumnCounter += yyleng; return RSHIFT; }
{bool}          { LastColumnCounter = ColumnCounter; if(print_tokens) printf("BOOL\n"); ColumnCounter += yyleng; return BOOL; }
{class}         { LastColumnCounter = ColumnCounter; if(print_tokens) printf("CLASS\n"); ColumnCounter += yyleng; return CLASS; }
{else}          { LastColumnCounter = ColumnCounter; if(print_tokens) printf("ELSE\n"); ColumnCounter += yyleng; return ELSE; }
{if}            { LastColumnCounter = ColumnCounter; if(print_tokens) printf("IF\n"); ColumnCounter += yyleng; return IF; }
{int}           { LastColumnCounter = ColumnCounter; if(print_tokens) printf("INT\n"); ColumnCounter += yyleng; return INT; }
{double}        { LastColumnCounter = ColumnCounter; if(print_tokens) printf("DOUBLE\n"); ColumnCounter += yyleng; return DOUBLE; }
{print}         { LastColumnCounter = ColumnCounter; if(print_tokens) printf("PRINT\n"); ColumnCounter += yyleng; return PRINT; }
{parseInt}      { LastColumnCounter = ColumnCounter; if(print_tokens) printf("PARSEINT\n"); ColumnCounter += yyleng; return PARSEINT; }
{public}        { LastColumnCounter = ColumnCounter; if(print_tokens) printf("PUBLIC\n"); ColumnCounter += yyleng; return PUBLIC; }
{return}        { LastColumnCounter = ColumnCounter; if(print_tokens) printf("RETURN\n"); ColumnCounter += yyleng; return RETURN; }
{static}        { LastColumnCounter = ColumnCounter; if(print_tokens) printf("STATIC\n"); ColumnCounter += yyleng; return STATIC; }
{string}        { LastColumnCounter = ColumnCounter; if(print_tokens) printf("STRING\n"); ColumnCounter += yyleng; return STRING; }
{void}          { LastColumnCounter = ColumnCounter; if(print_tokens) printf("VOID\n"); ColumnCounter += yyleng; return VOID; }
{while}         { LastColumnCounter = ColumnCounter; if(print_tokens) printf("WHILE\n"); ColumnCounter += yyleng; return WHILE; }
{dotlength}     { LastColumnCounter = ColumnCounter; if(print_tokens) printf("DOTLENGTH\n"); ColumnCounter += yyleng; return DOTLENGTH;}
{assign}        { LastColumnCounter = ColumnCounter; if(print_tokens) printf("ASSIGN\n"); ColumnCounter += yyleng; return ASSIGN; }
{star}          { LastColumnCounter = ColumnCounter; if(print_tokens) printf("STAR\n"); ColumnCounter += yyleng; return STAR; }
{comma}         { LastColumnCounter = ColumnCounter; if(print_tokens) printf("COMMA\n"); ColumnCounter += yyleng; return COMMA; }
{div}           { LastColumnCounter = ColumnCounter; if(print_tokens) printf("DIV\n"); ColumnCounter += yyleng; return DIV; }
{gt}            { LastColumnCounter = ColumnCounter; if(print_tokens) printf("GT\n"); ColumnCounter += yyleng; return GT; }
{lt}            { LastColumnCounter = ColumnCounter; if(print_tokens) printf("LT\n"); ColumnCounter += yyleng; return LT; }
{lbrace}        { LastColumnCounter = ColumnCounter; if(print_tokens) printf("LBRACE\n"); ColumnCounter += yyleng; return LBRACE; }
{rbrace}        { LastColumnCounter = ColumnCounter; if(print_tokens) printf("RBRACE\n"); ColumnCounter += yyleng; return RBRACE; }
{lpar}          { LastColumnCounter = ColumnCounter; if(print_tokens) printf("LPAR\n"); ColumnCounter += yyleng; return LPAR; }
{rpar}          { LastColumnCounter = ColumnCounter; if(print_tokens) printf("RPAR\n"); ColumnCounter += yyleng; return RPAR; }
{lsq}           { LastColumnCounter = ColumnCounter; if(print_tokens) printf("LSQ\n"); ColumnCounter += yyleng; return LSQ; }
{rsq}           { LastColumnCounter = ColumnCounter; if(print_tokens) printf("RSQ\n"); ColumnCounter += yyleng; return RSQ; }
{semicolon}     { LastColumnCounter = ColumnCounter; if(print_tokens) printf("SEMICOLON\n"); ColumnCounter += yyleng; return SEMICOLON; }
{minus}         { LastColumnCounter = ColumnCounter; if(print_tokens) printf("MINUS\n"); ColumnCounter += yyleng; return MINUS; }
{mod}           { LastColumnCounter = ColumnCounter; if(print_tokens) printf("MOD\n"); ColumnCounter += yyleng; return MOD; }
{plus}          { LastColumnCounter = ColumnCounter; if(print_tokens) printf("PLUS\n"); ColumnCounter += yyleng; return PLUS; }
{xor}           { LastColumnCounter = ColumnCounter; if(print_tokens) printf("XOR\n"); ColumnCounter += yyleng; return XOR; }
{not}           { LastColumnCounter = ColumnCounter; if(print_tokens) printf("NOT\n"); ColumnCounter += yyleng; return NOT; }

{decimal}       { LastColumnCounter = ColumnCounter; if(print_tokens) printf("DECIMAL(%s)\n",yytext); PASS_TOKEN(Decimal); ColumnCounter+=yyleng; return DECIMAL; }
{natural}       { LastColumnCounter = ColumnCounter; if(print_tokens) printf("NATURAL(%s)\n",yytext); PASS_TOKEN(Natural); ColumnCounter+=yyleng; return NATURAL; }
{identifier}    { LastColumnCounter = ColumnCounter; if(print_tokens) printf("IDENTIFIER(%s)\n",yytext); PASS_TOKEN(Identifier); ColumnCounter+=yyleng; return IDENTIFIER; }
{reserved}      { LastColumnCounter = ColumnCounter; 
                  if(print_tokens) printf("RESERVED(%s)\n", yytext); PASS_TOKEN(Reserved);ColumnCounter += yyleng; return RESERVED;
                   }
"."             { printf("Line %d, col %d: illegal character (.)\n", LineCounter, ColumnCounter); ColumnCounter++; }
.               { printf("Line %d, col %d: illegal character (%s)\n", LineCounter, ColumnCounter, yytext); ColumnCounter++; }
%%
extern int yylex();
int main(int argc, char *argv[]) {
    int flag_l = 0, flag_e1 = 0, flag_t = 0, flag_e2 = 0, flag_s = 0;

    if (argc > 1) {
        if (strcmp(argv[1], "-l") == 0) flag_l = 1;
        else if (strcmp(argv[1], "-e1") == 0) flag_e1 = 1;
        else if (strcmp(argv[1], "-t") == 0) flag_t = 1;
        else if (strcmp(argv[1], "-e2") == 0) flag_e2 = 1;
        else if (strcmp(argv[1], "-s") == 0) flag_s = 1;
    }

    if (flag_l) {
        print_tokens = 1;
        yylex();
        return 0;
    }
    
    if (flag_e1) {
        print_tokens = 0;
        yylex();
        return 0;
    }

    yyparse();

    if (flag_e2) {
        return 0; 
    }

    if (syntax_error == 0) {
        if (flag_t) {
            show(ast, 0);
        } else if (flag_s) {
            check_semantics(ast);
            
            SymTable *curr = global_table;
            while (curr) {
                print_sym_table(curr);
                curr = curr->next;
            }
            
            show_annotated(ast, 0);
        } else {
            check_semantics(ast);
        }
    }
    
    return 0;
}
//int yywrap() {  /* called on EOF, return 1 to terminate */
//    return 1;
//}

void yyerror(char *s) {
    syntax_error++;
    const char *tok;
    if (yytext && yytext[0] == '"' && yylval.lexeme != NULL)
        tok = yylval.lexeme;
    else
        tok = (yytext && yytext[0]) ? yytext : "";
        
    int col = LastColumnCounter;
    if (tok[0] == '\0') {
        col = ColumnCounter;
    }

    printf("Line %d, col %d: %s: %s\n", LineCounter, col, s, tok);
}