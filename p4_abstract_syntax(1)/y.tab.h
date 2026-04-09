#ifndef _yy_defines_h_
#define _yy_defines_h_

#define INTEGER 257
#define DOUBLE 258
#define IF 259
#define THEN 260
#define ELSE 261
#define IDENTIFIER 262
#define NATURAL 263
#define DECIMAL 264
#define LOW 265
#ifdef YYSTYPE
#undef  YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
#endif
#ifndef YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
typedef union YYSTYPE{
    char *lexeme;
    struct node *node;
} YYSTYPE;
#endif /* !YYSTYPE_IS_DECLARED */
extern YYSTYPE yylval;

#endif /* _yy_defines_h_ */
