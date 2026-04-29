#ifndef _yy_defines_h_
#define _yy_defines_h_

#define CLASS 257
#define PUBLIC 258
#define STATIC 259
#define LBRACE 260
#define RBRACE 261
#define LPAR 262
#define RPAR 263
#define LSQ 264
#define RSQ 265
#define SEMICOLON 266
#define COMMA 267
#define BOOL 268
#define INT 269
#define DOUBLE 270
#define VOID 271
#define STRING 272
#define IF 273
#define WHILE 274
#define RETURN 275
#define PRINT 276
#define PARSEINT 277
#define ASSIGN 278
#define PLUS 279
#define MINUS 280
#define STAR 281
#define DIV 282
#define MOD 283
#define AND 284
#define OR 285
#define XOR 286
#define LSHIFT 287
#define RSHIFT 288
#define EQ 289
#define GE 290
#define GT 291
#define LE 292
#define LT 293
#define NE 294
#define NOT 295
#define DOTLENGTH 296
#define ARROW 297
#define IDENTIFIER 298
#define NATURAL 299
#define DECIMAL 300
#define BOOLLIT 301
#define STRLIT 302
#define RESERVED 303
#define UNARY 304
#define IF_PREC 305
#define ELSE 306
#ifdef YYSTYPE
#undef  YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
#endif
#ifndef YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
typedef union YYSTYPE {
    char *lexeme;
    struct node *node;
} YYSTYPE;
#endif /* !YYSTYPE_IS_DECLARED */
extern YYSTYPE yylval;

#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
/* Default: YYLTYPE is the text position type. */
typedef struct YYLTYPE
{
    int first_line;
    int first_column;
    int last_line;
    int last_column;
    unsigned source;
} YYLTYPE;
#define YYLTYPE_IS_DECLARED 1
#endif
#define YYRHSLOC(rhs, k) ((rhs)[k])
extern YYLTYPE yylloc;

#endif /* _yy_defines_h_ */
