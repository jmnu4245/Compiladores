%{
        /* place here any C variables and definitions */
    #include <stdio.h>
    int LineCounter;
    int ColumnCounter;
    int ColumnCommentCounter;
    int LineCommentCounter;

    char *str=NULL;
%}
%X COMMENT STRING

digit   [0-9]
idigit [1-9]
letter [a-z]|"$"|"_"|[A-Z]
identifier {letter}({letter}|{digit})*  
natural ({idigit}(({digit}|"_")*{digit})*)|0
intpart  {natural}|"0"
decimal  ({intpart}"."({natural})?|"."{natural})(("e"|"E")("+"|"-")?{natural})?|{intpart}("e"|"E")("+"|"-")?{natural}
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
rbrace "}"
rpar ")"
rsq "]"
semicolon ";"
arrow "->"
lshift "<<"
rshift ">>"
xor "^"
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
whitespace " "
tab "\t"

%%
"/*"            {BEGIN(COMMENT);ColumnCommentCounter=2;LineCommentCounter=1;} 
<COMMENT>\r           {ColumnCommentCounter+=1;}
<COMMENT>\n      {LineCommentCounter++; ColumnCommentCounter=1; }
<COMMENT>.       {ColumnCommentCounter+=yyleng; }
<COMMENT>"*/"      {BEGIN(0);
                    if (LineCommentCounter==1) {
                       ColumnCounter += ColumnCommentCounter+2;
                    } else {
                       ColumnCounter = ColumnCommentCounter+2;
                       LineCounter += LineCommentCounter;
                    }
}
<COMMENT><<EOF>>  { printf("Line %d, col %d: unterminated comment\n", LineCounter, ColumnCounter); return 0; }
"\"" { if(str) free(str); str = strdup(""); BEGIN(STRING); ColumnCounter++; }
<STRING>"\""   {printf("STRLIT(\"%s\")\n",str); free(str); str=strdup(""); ColumnCounter++; BEGIN(0);}
<STRING>"\n"              {printf("\n");LineCounter++;ColumnCounter=1;}
<STRING>"\\"[ntrfr\\"]  { char *tmp=malloc(strlen(str)+3); strcpy(tmp,str); strcat(tmp,yytext); str=tmp; ColumnCounter+=2; }
<STRING>"\\"[^\n]       { printf("Line %d, col %d: invalid escape sequence (%s)\n", LineCounter, ColumnCounter, yytext); ColumnCounter+=2; }
<STRING>"\\"             { printf("Line %d, col %d: invalid escape sequence (\\)\n",
                           LineCounter, ColumnCounter);
                           ColumnCounter++; }
<STRING>. { 
    char *tmp = malloc(strlen(str) + yyleng + 1);
    strcpy(tmp, str);
    strcat(tmp, yytext);
    free(str);
    str = tmp;
    ColumnCounter += yyleng;
}
"//"[^\r\n]*    {LineCounter++; ColumnCounter = 1;}
{whitespace}    { ColumnCounter++; }
{tab}           { ColumnCounter += 4; }
\n              { LineCounter++; ColumnCounter=1; }
\r           {}
{reserved}      { printf("RESERVED(%s)\n", yytext);
                  ColumnCounter += yyleng; }
{boollit}  { printf("BOOLLIT(%s)\n", yytext); ColumnCounter += yyleng; }
{bool}          { printf("BOOL\n"   ); ColumnCounter += yyleng; }
{class}         { printf("CLASS\n"  ); ColumnCounter += yyleng; }
{else}          { printf("ELSE\n"   ); ColumnCounter += yyleng; }
{if}            { printf("IF\n"     ); ColumnCounter += yyleng; }
{int}           { printf("INT\n"    ); ColumnCounter += yyleng; }
{print}         { printf("PRINT\n"  ); ColumnCounter += yyleng; }
{parseInt}         { printf("PARSEINT\n"  ); ColumnCounter += yyleng; }
{double}        {printf("DOUBLE\n"  ); ColumnCounter += yyleng; }
{public}        { printf("PUBLIC\n" ); ColumnCounter += yyleng; }
{return}        { printf("RETURN\n" ); ColumnCounter += yyleng; }
{static}        { printf("STATIC\n" ); ColumnCounter += yyleng; }
{string}        { printf("STRING\n"); ColumnCounter += yyleng; }
{void}          { printf("VOID\n"   ); ColumnCounter += yyleng; }
{while}         { printf("WHILE\n"  ); ColumnCounter += yyleng; }
{and}           { printf("AND\n"      ); ColumnCounter += yyleng; }
{or}            { printf("OR\n"       ); ColumnCounter += yyleng; }
{eq}            { printf("EQ\n"       ); ColumnCounter += yyleng; }
{ne}            { printf("NE\n"       ); ColumnCounter += yyleng; }
{ge}            { printf("GE\n"       ); ColumnCounter += yyleng; }
{le}            { printf("LE\n"       ); ColumnCounter += yyleng; }
{gt}            { printf("GT\n"       ); ColumnCounter += yyleng; }
{lt}            { printf("LT\n"       ); ColumnCounter += yyleng; }
{lshift}        { printf("LSHIFT\n"   ); ColumnCounter += yyleng; }
{rshift}        { printf("RSHIFT\n"   ); ColumnCounter += yyleng; }
{arrow}         { printf("ARROW\n"    ); ColumnCounter += yyleng; }
{assign}        { printf("ASSIGN\n"   ); ColumnCounter += yyleng; }
{plus}          { printf("PLUS\n"     ); ColumnCounter += yyleng; }
{minus}         { printf("MINUS\n"    ); ColumnCounter += yyleng; }
{star}          { printf("STAR\n"     ); ColumnCounter += yyleng; }
{mod}           { printf("MOD\n"      ); ColumnCounter += yyleng; }
{xor}           { printf("XOR\n"      ); ColumnCounter += yyleng; }
{lbrace}        { printf("LBRACE\n"   ); ColumnCounter += yyleng; }
{rbrace}        { printf("RBRACE\n"   ); ColumnCounter += yyleng; }
{lpar}          { printf("LPAR\n"     ); ColumnCounter += yyleng; }
{rpar}          { printf("RPAR\n"     ); ColumnCounter += yyleng; }
{lsq}           { printf("LSQ\n"      ); ColumnCounter += yyleng; }
{rsq}           { printf("RSQ\n"      ); ColumnCounter += yyleng; }
{semicolon}     { printf("SEMICOLON\n"); ColumnCounter += yyleng; }
{comma}         { printf("COMMA\n"    ); ColumnCounter += yyleng; }

{natural}   {printf("NATURAL(%s)\n",yytext);ColumnCounter+=yyleng;}
{decimal}   {printf("DECIMAL(%s)\n",yytext);ColumnCounter+=yyleng;}
{identifier}  {printf("IDENTIFIER(%s)\n",yytext);ColumnCounter+=yyleng;}

. { printf("Line %d, col %d: illegal character:(%s)\n", LineCounter, ColumnCounter, yytext ); ColumnCounter++; }
%%
extern int yylex();
int main() {
    LineCounter=1;
    ColumnCounter=1;
    yylex();    /* run the lexical analysis automaton */
    return 0;
}
int yywrap() {  /* called on EOF, return 1 to terminate */
    return 1;
}