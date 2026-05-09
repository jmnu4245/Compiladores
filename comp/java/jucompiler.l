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
    #include "codegen.h"

    void check_semantics(struct node *n);
    void print_sym_table(SymTable *table);

#define YY_USER_ACTION { \
    yylloc.first_line = yylloc.last_line = LineCounter; \
    yylloc.first_column = ColumnCounter; \
    yylloc.last_column = ColumnCounter + yyleng - 1; \
    LastColumnCounter = ColumnCounter; \
    ColumnCounter += yyleng; \
}

    #define PASS_TOKEN() yylval.lexeme = strdup(yytext);
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
    str_start_col = LastColumnCounter; 
}
<COMMENT>"*/" { 
    BEGIN(0); 
}
<COMMENT>\n|\r\n|\r { LineCounter++; ColumnCounter = 1; }

<COMMENT>[^*\n\r]+ { }
<COMMENT>"*"       { }

<COMMENT><<EOF>> { 
    printf("Line %d, col %d: unterminated comment\n", str_start_line, str_start_col); 
    return 0; 
}

"\"" { 
    BEGIN(STR_STATE); 
    str_start_line = LineCounter; 
    str_start_col = LastColumnCounter; /* MODIFICADO */
    string_erro = 0; 
    if(str) free(str); 
    str = strdup(""); 
}
<STR_STATE>\" {
    BEGIN(0);
    if (!string_erro) {
        char *with_quotes = malloc(strlen(str) + 3);
        sprintf(with_quotes, "\"%s\"", str);
        if(print_tokens) printf("STRLIT(\"%s\")\n", str);
        yylval.lexeme = with_quotes;
        if(str){ free(str); str = NULL; }
        LastColumnCounter = str_start_col; 
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
}
<STR_STATE>\\(.|[\n]|[\r]) {  
    string_erro = 1;
    if(yytext[1] == '\n' || yytext[1] == '\r') {
        printf("Line %d, col %d: invalid escape sequence (\\)\n", LineCounter, LastColumnCounter);
        yyless(1);
        ColumnCounter--; /* Compensar el yyless */
    } else {
        printf("Line %d, col %d: invalid escape sequence (%s)\n", LineCounter, LastColumnCounter, yytext);
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
}

"//"[^\r\n]* {}

{whitespace}    { }
{tab}           { }
\n|\r\n|\r      { LineCounter++; ColumnCounter = 1; }

{reserved}      { if(print_tokens) printf("RESERVED(%s)\n", yytext); PASS_TOKEN(); return RESERVED; }
{boollit}       { if(print_tokens) printf("BOOLLIT(%s)\n", yytext); PASS_TOKEN(); return BOOLLIT; }
{and}           { if(print_tokens) printf("AND\n"); return AND; }
{or}            { if(print_tokens) printf("OR\n"); return OR; }
{eq}            { if(print_tokens) printf("EQ\n"); return EQ; }
{ne}            { if(print_tokens) printf("NE\n"); return NE; }
{ge}            { if(print_tokens) printf("GE\n"); return GE; }
{le}            { if(print_tokens) printf("LE\n"); return LE; }
{lshift}        { if(print_tokens) printf("LSHIFT\n"); return LSHIFT; }
{rshift}        { if(print_tokens) printf("RSHIFT\n"); return RSHIFT; }
{arrow}         { if(print_tokens) printf("ARROW\n"); return ARROW; }
{bool}          { if(print_tokens) printf("BOOL\n"); return BOOL; }
{class}         { if(print_tokens) printf("CLASS\n"); return CLASS; }
{else}          { if(print_tokens) printf("ELSE\n"); return ELSE; }
{if}            { if(print_tokens) printf("IF\n"); return IF; }
{int}           { if(print_tokens) printf("INT\n"); return INT; }
{double}        { if(print_tokens) printf("DOUBLE\n"); return DOUBLE; }
{print}         { if(print_tokens) printf("PRINT\n"); return PRINT; }
{parseInt}      { if(print_tokens) printf("PARSEINT\n"); return PARSEINT; }
{public}        { if(print_tokens) printf("PUBLIC\n"); return PUBLIC; }
{return}        { if(print_tokens) printf("RETURN\n"); return RETURN; }
{static}        { if(print_tokens) printf("STATIC\n"); return STATIC; }
{string}        { if(print_tokens) printf("STRING\n"); return STRING; }
{void}          { if(print_tokens) printf("VOID\n"); return VOID; }
{while}         { if(print_tokens) printf("WHILE\n"); return WHILE; }
{dotlength}     { if(print_tokens) printf("DOTLENGTH\n"); return DOTLENGTH;}
{assign}        { if(print_tokens) printf("ASSIGN\n"); return ASSIGN; }
{star}          { if(print_tokens) printf("STAR\n"); return STAR; }
{comma}         { if(print_tokens) printf("COMMA\n"); return COMMA; }
{div}           { if(print_tokens) printf("DIV\n"); return DIV; }
{gt}            { if(print_tokens) printf("GT\n"); return GT; }
{lt}            { if(print_tokens) printf("LT\n"); return LT; }
{lbrace}        { if(print_tokens) printf("LBRACE\n"); return LBRACE; }
{rbrace}        { if(print_tokens) printf("RBRACE\n"); return RBRACE; }
{lpar}          { if(print_tokens) printf("LPAR\n"); return LPAR; }
{rpar}          { if(print_tokens) printf("RPAR\n"); return RPAR; }
{lsq}           { if(print_tokens) printf("LSQ\n"); return LSQ; }
{rsq}           { if(print_tokens) printf("RSQ\n"); return RSQ; }
{semicolon}     { if(print_tokens) printf("SEMICOLON\n"); return SEMICOLON; }
{minus}         { if(print_tokens) printf("MINUS\n"); return MINUS; }
{mod}           { if(print_tokens) printf("MOD\n"); return MOD; }
{plus}          { if(print_tokens) printf("PLUS\n"); return PLUS; }
{xor}           { if(print_tokens) printf("XOR\n"); return XOR; }
{not}           { if(print_tokens) printf("NOT\n"); return NOT; }

{decimal}       { if(print_tokens) printf("DECIMAL(%s)\n",yytext); PASS_TOKEN(); return DECIMAL; }
{natural}       { if(print_tokens) printf("NATURAL(%s)\n",yytext); PASS_TOKEN(); return NATURAL; }
{identifier}    { if(print_tokens) printf("IDENTIFIER(%s)\n",yytext); PASS_TOKEN(); return IDENTIFIER; }

"."             { printf("Line %d, col %d: illegal character (.)\n", LineCounter, LastColumnCounter);}
.               { printf("Line %d, col %d: illegal character (%s)\n", LineCounter, LastColumnCounter, yytext);}
%%
extern int yylex();
extern int semantic_errors;

int main(int argc, char *argv[]) {
    LineCounter = 1;
    ColumnCounter = 1;
    
    int flag_t = 0;
    int flag_e2 = 0;
    int flag_s = 0;
    int flag_e3 = 0;

    if (argc > 1) {
        if (strcmp(argv[1], "-l") == 0) {
            print_tokens = 1;
            while(yylex());
            return 0;
        }
        if (strcmp(argv[1], "-e1") == 0) {
            while(yylex());
            return 0;
        }
        if (strcmp(argv[1], "-t") == 0) flag_t = 1;
        if (strcmp(argv[1], "-e2") == 0) flag_e2 = 1;
        if (strcmp(argv[1], "-s") == 0) flag_s = 1;
        if (strcmp(argv[1], "-e3") == 0) flag_e3 = 1;
    }

    yyparse();

    if (syntax_error == 0) {
        if (flag_t) {
            show(ast, 0); 
        } else if (flag_e2) {
        } else {
            check_semantics(ast); 
            extern SymTable *global_table;
            if (flag_s) {
                print_all_tables(global_table);
                show_annotated(ast, 0);
            } else if (flag_e3) {
                
            } else if (semantic_errors == 0) {
                codegen_program(ast, global_table);
            }
        }
    }
    if (str) {
        free(str);
        str = NULL;
    }
    return 0;
}

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