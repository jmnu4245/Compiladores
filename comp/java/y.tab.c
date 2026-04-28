/* original parser id follows */
/* yysccsid[] = "@(#)yaccpar	1.9 (Berkeley) 02/21/93" */
/* (use YYMAJOR/YYMINOR for ifdefs dependent on parser version) */

#define YYBYACC 1
#define YYMAJOR 2
#define YYMINOR 0
#define YYPATCH 20240109

#define YYEMPTY        (-1)
#define yyclearin      (yychar = YYEMPTY)
#define yyerrok        (yyerrflag = 0)
#define YYRECOVERING() (yyerrflag != 0)
#define YYENOMEM       (-2)
#define YYEOF          0
#undef YYBTYACC
#define YYBTYACC 0
#define YYDEBUGSTR YYPREFIX "debug"
#define YYPREFIX "yy"

#define YYPURE 0

#line 5 "jucompiler.y"
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"


int yylex(void);
void yyerror(char *);
struct node *ast;

struct node *create_multiple_decls(enum category decl_type, struct node *type_node, struct node *first_id_node, struct node *extra_ids_container);
int count_children(struct node *n);
void unpack_nodes(struct node *parent, struct node *container);
#ifdef YYSTYPE
#undef  YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
#endif
#ifndef YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
#line 19 "jucompiler.y"
typedef union YYSTYPE {
    char *lexeme;
    struct node *node;
} YYSTYPE;
#endif /* !YYSTYPE_IS_DECLARED */
#line 49 "y.tab.c"

/* compatibility with bison */
#ifdef YYPARSE_PARAM
/* compatibility with FreeBSD */
# ifdef YYPARSE_PARAM_TYPE
#  define YYPARSE_DECL() yyparse(YYPARSE_PARAM_TYPE YYPARSE_PARAM)
# else
#  define YYPARSE_DECL() yyparse(void *YYPARSE_PARAM)
# endif
#else
# define YYPARSE_DECL() yyparse(void)
#endif

/* Parameters sent to lex. */
#ifdef YYLEX_PARAM
# define YYLEX_DECL() yylex(void *YYLEX_PARAM)
# define YYLEX yylex(YYLEX_PARAM)
#else
# define YYLEX_DECL() yylex(void)
# define YYLEX yylex()
#endif

#if !(defined(yylex) || defined(YYSTATE))
int YYLEX_DECL();
#endif

/* Parameters sent to yyerror. */
#ifndef YYERROR_DECL
#define YYERROR_DECL() yyerror(const char *s)
#endif
#ifndef YYERROR_CALL
#define YYERROR_CALL(msg) yyerror(msg)
#endif

extern int YYPARSE_DECL();

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
#define RESERVED 297
#define ARROW 298
#define IDENTIFIER 299
#define NATURAL 300
#define DECIMAL 301
#define BOOLLIT 302
#define STRLIT 303
#define UNARY 304
#define IF_PREC 305
#define ELSE 306
#define YYERRCODE 256
typedef int YYINT;
static const YYINT yylhs[] = {                           -1,
    0,    1,    1,    2,    2,    2,    3,    3,    3,    3,
    4,    4,    5,    5,    6,    6,    6,    7,    8,    9,
    9,   10,   10,   12,   12,   11,   11,   13,   14,   14,
   15,   15,   16,   17,   17,   17,   17,   17,   17,   17,
   17,   17,   17,   17,   17,   17,   17,   18,   18,   19,
   19,   20,   20,   21,   21,   22,   23,   23,   24,   24,
   25,   25,   25,   25,   25,   25,   25,   25,   25,   25,
   25,   25,   25,   25,   25,   25,   25,   25,   25,   25,
   25,   25,   25,   25,   25,   25,   25,   25,
};
static const YYINT yylen[] = {                            2,
    5,    2,    0,    1,    1,    1,    4,    4,    4,    2,
    6,    2,    3,    0,    1,    1,    1,    3,    1,    5,
    5,    1,    0,    3,    1,    2,    2,    3,    2,    0,
    1,    1,    4,    3,    5,    7,    5,    2,    3,    2,
    2,    2,    1,    5,    5,    2,    2,    2,    0,    4,
    4,    2,    0,    3,    0,    3,    7,    4,    1,    1,
    3,    3,    3,    3,    3,    3,    3,    3,    3,    3,
    3,    3,    3,    3,    3,    3,    2,    2,    2,    3,
    3,    1,    1,    1,    2,    1,    1,    1,
};
static const YYINT yydefred[] = {                         0,
    0,    0,    0,    3,    0,    0,    0,    1,    6,    2,
    4,    5,   10,   12,    0,    0,   15,   16,   17,   19,
    0,    0,    0,    8,    9,    0,    0,   30,    7,    0,
    0,    0,    0,    0,    0,    0,    0,   25,    0,   11,
    0,    0,    0,   49,   28,   43,    0,    0,    0,    0,
    0,    0,    0,   29,   32,   31,    0,    0,    0,    0,
   26,   27,   20,    0,   13,   21,   47,   46,    0,    0,
    0,    0,   38,    0,    0,    0,    0,   86,   87,   88,
   82,   59,   83,    0,    0,    0,    0,    0,    0,   14,
   40,   41,   42,   18,   24,   34,   48,    0,    0,    0,
    0,    0,   78,   77,   79,   85,   39,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,   55,
   56,    0,    0,    0,   81,   80,    0,    0,   63,   64,
   65,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,   58,    0,   51,   50,    0,   33,
    0,   37,   45,   44,    0,    0,    0,    0,   54,   36,
   57,
};
#if defined(YYDESTRUCT_CALL) || defined(YYSTYPE_TOSTRING)
static const YYINT yystos[] = {                           0,
  257,  308,  299,  260,  309,  256,  258,  261,  266,  310,
  311,  312,  261,  266,  259,  256,  268,  269,  270,  271,
  314,  316,  317,  261,  266,  299,  299,  260,  321,  262,
  313,  262,  322,  272,  314,  315,  318,  319,  320,  266,
  267,  318,  256,  260,  261,  266,  273,  274,  275,  276,
  277,  299,  314,  323,  324,  325,  327,  330,  331,  264,
  299,  299,  263,  267,  299,  263,  261,  266,  326,  262,
  262,  262,  266,  279,  280,  295,  299,  300,  301,  302,
  327,  330,  331,  332,  333,  262,  262,  262,  278,  299,
  266,  266,  266,  265,  319,  261,  325,  332,  332,  256,
  332,  299,  333,  333,  333,  296,  266,  279,  280,  281,
  282,  283,  284,  285,  286,  287,  288,  289,  290,  291,
  292,  293,  294,  303,  332,  256,  299,  256,  328,  332,
  332,  313,  263,  263,  263,  263,  333,  333,  333,  333,
  333,  333,  333,  333,  333,  333,  333,  333,  333,  333,
  333,  333,  263,  263,  263,  264,  263,  263,  329,  266,
  325,  325,  266,  266,  332,  267,  306,  265,  332,  325,
  263,
};
#endif /* YYDESTRUCT_CALL || YYSTYPE_TOSTRING */
static const YYINT yydgoto[] = {                          2,
    5,   10,   11,   12,   31,   35,   36,   22,   23,   37,
   38,   39,   29,   33,   54,   55,   56,   69,   81,  129,
  159,   82,   83,   84,   85,
};
static const YYINT yysindex[] = {                      -244,
 -279,    0, -237,    0,  -86, -236, -232,    0,    0,    0,
    0,    0,    0,    0,  110, -183,    0,    0,    0,    0,
 -240, -238, -187,    0,    0, -186, -168,    0,    0,  -45,
 -250,  -45, -189, -148, -191, -182, -144,    0, -139,    0,
 -170, -132, -171,    0,    0,    0, -127, -125,  -81, -120,
 -119, -256, -154,    0,    0,    0, -109, -105, -104, -118,
    0,    0,    0,  -45,    0,    0,    0,    0, -242,  -51,
  -51, -188,    0,  -40,  -40,  -40, -257,    0,    0,    0,
    0,    0,    0,  -98,  211,  -93, -241, -147,  -51,    0,
    0,    0,    0,    0,    0,    0,    0,  -92,  -89,  -87,
  -85, -259,    0,    0,    0,    0,    0,  -40,  -40,  -40,
  -40,  -40,  -40,  -40,  -40,  -40,  -40,  -40,  -40,  -40,
  -40,  -40,  -40,  -84,  -75,  -66,  -82,  -63,  -50,    0,
    0, -197,  -83,  -83,    0,    0,  -14,  -14,    0,    0,
    0,  243,  227,  259, -156, -156,  275,   12,   12,   12,
   12,  275,  -71,  -54,    0,  -51,    0,    0,  -52,    0,
  -76,    0,    0,    0,  -48,  -51,  -83,  -27,    0,    0,
    0,
};
static const YYINT yyrindex[] = {                         0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, -169,    0,    0,    0,  -25,
    0,  -25,    0,    0,    0,    0,    0,    0,  -22,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   -9,    0,    0,    0,
    0,    0,    0,    0,  -62,    0,    0,  -21,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,   -9,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   23,   55,    0,    0,
    0,  -32, -203,  -20,   66,   98, -145,  109,  141,  152,
  184,  195,    0,    0,    0,    0,    0,    0,   24,    0,
 -110,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,
};
#if YYBTYACC
static const YYINT yycindex[] = {                         0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,
};
#endif
static const YYINT yygindex[] = {                         0,
    0,    0,    0,    0,  142,   60,    0,    0,    0,  231,
  232,    0,    0,    0,    0,    0,  -68,    0,  -33,    0,
    0,  -31,  -29,  -60,  -67,
};
#define YYTABLESIZE 568
static const YYINT yytable[] = {                         57,
   97,   58,   88,   59,   88,   88,  103,  104,  105,   98,
   99,  101,    1,   43,  126,   40,   41,   44,   96,    3,
   89,   89,    4,   46,   13,  125,   15,  130,  131,   14,
   47,   48,   49,   50,   51,   57,  106,   58,  106,   59,
  137,  138,  139,  140,  141,  142,  143,  144,  145,  146,
  147,  148,  149,  150,  151,  152,   52,  127,   26,   67,
   27,   67,   67,   67,  161,  162,   43,  100,  160,   41,
   44,   45,   28,   72,   21,   30,   46,   24,   17,   18,
   19,   67,   25,   47,   48,   49,   50,   51,   51,   67,
   74,   75,   53,   32,   68,  165,   14,   14,  170,   57,
   57,   58,   58,   59,   59,  169,   76,   61,  128,   52,
   77,   78,   79,   80,   72,   60,   62,   71,   63,   71,
   71,   71,  108,  109,  110,  111,  112,   64,   65,   51,
   66,   74,   75,   57,   70,   58,   71,   59,   71,   71,
   71,   86,   87,   71,   90,   35,   94,   76,   71,   35,
   35,   77,   78,   79,   80,   35,   91,   35,   35,   35,
   92,   93,   35,   35,   35,   35,   35,  107,   72,    6,
  133,    7,   43,  134,    8,  135,   44,  136,  153,    9,
   72,  156,   46,   51,   73,   74,   75,  154,   35,   47,
   48,   49,   50,   51,  163,   51,  155,   74,   75,  157,
   60,   76,   60,   60,   60,   77,   78,   79,   80,  124,
   72,  164,  158,   76,  166,   52,  168,   77,   78,   79,
   80,   72,   17,   18,   19,   51,   34,   74,   75,  167,
   66,  132,   66,   66,   66,  171,   51,   23,   74,   75,
   22,   53,   68,   76,   68,   68,   68,   77,   78,   79,
   80,   66,   66,   84,   76,   84,   84,   84,  102,   78,
   79,   80,   42,   68,   68,   68,  110,  111,  112,   84,
   84,   84,   84,   84,   84,   84,   84,   84,   84,   84,
   84,   84,   84,   84,   84,   61,   52,   61,   61,   61,
  108,  109,  110,  111,  112,   95,    0,    0,  116,  117,
    0,   61,   61,    0,    0,    0,   61,   61,   61,   61,
   61,   61,   61,   61,   61,   61,   61,   62,    0,   62,
   62,   62,    0,    0,    0,    0,    0,    0,   69,    0,
   69,   69,   69,   62,   62,    0,    0,    0,   62,   62,
   62,   62,   62,   62,   62,   62,   62,   62,   62,   69,
   69,   69,   69,   69,   69,   69,   69,   69,   69,   69,
   70,    0,   70,   70,   70,   16,    0,    0,    0,    0,
    0,   74,    0,   74,   74,   74,    0,   17,   18,   19,
   20,   70,   70,   70,   70,   70,   70,   70,   70,   70,
   70,   70,   74,   74,   74,    0,    0,   74,   74,   74,
   74,   74,   74,   73,    0,   73,   73,   73,    0,    0,
    0,    0,    0,    0,   76,    0,   76,   76,   76,    0,
    0,    0,    0,    0,   73,   73,   73,    0,    0,   73,
   73,   73,   73,   73,   73,   76,   76,   76,    0,    0,
   76,   76,   76,   76,   76,   76,   75,    0,   75,   75,
   75,    0,    0,    0,    0,    0,    0,   72,    0,   72,
   72,   72,    0,    0,    0,    0,    0,   75,   75,   75,
    0,    0,   75,   75,   75,   75,   75,   75,   72,   72,
   72,    0,    0,   72,    0,    0,    0,    0,   72,  108,
  109,  110,  111,  112,  113,  114,  115,  116,  117,  118,
  119,  120,  121,  122,  123,  108,  109,  110,  111,  112,
  113,    0,  115,  116,  117,  118,  119,  120,  121,  122,
  123,  108,  109,  110,  111,  112,    0,    0,  115,  116,
  117,  118,  119,  120,  121,  122,  123,  108,  109,  110,
  111,  112,    0,    0,    0,  116,  117,  118,  119,  120,
  121,  122,  123,  108,  109,  110,  111,  112,    0,    0,
    0,  116,  117,    0,  119,  120,  121,  122,
};
static const YYINT yycheck[] = {                         33,
   69,   33,  262,   33,  262,  262,   74,   75,   76,   70,
   71,   72,  257,  256,  256,  266,  267,  260,  261,  299,
  278,  278,  260,  266,  261,   86,  259,   88,   89,  266,
  273,  274,  275,  276,  277,   69,  296,   69,  296,   69,
  108,  109,  110,  111,  112,  113,  114,  115,  116,  117,
  118,  119,  120,  121,  122,  123,  299,  299,  299,  263,
  299,  265,  266,  267,  133,  134,  256,  256,  266,  267,
  260,  261,  260,  262,   15,  262,  266,  261,  268,  269,
  270,  285,  266,  273,  274,  275,  276,  277,  277,  261,
  279,  280,   33,  262,  266,  156,  266,  267,  167,  133,
  134,  133,  134,  133,  134,  166,  295,  299,  256,  299,
  299,  300,  301,  302,  262,  264,  299,  263,  263,  265,
  266,  267,  279,  280,  281,  282,  283,  267,  299,  277,
  263,  279,  280,  167,  262,  167,  262,  167,  284,  285,
  286,  262,  262,  289,  299,  256,  265,  295,  294,  260,
  261,  299,  300,  301,  302,  266,  266,  268,  269,  270,
  266,  266,  273,  274,  275,  276,  277,  266,  262,  256,
  263,  258,  256,  263,  261,  263,  260,  263,  263,  266,
  262,  264,  266,  277,  266,  279,  280,  263,  299,  273,
  274,  275,  276,  277,  266,  277,  263,  279,  280,  263,
  263,  295,  265,  266,  267,  299,  300,  301,  302,  303,
  262,  266,  263,  295,  267,  299,  265,  299,  300,  301,
  302,  262,  268,  269,  270,  277,  272,  279,  280,  306,
  263,   90,  265,  266,  267,  263,  277,  263,  279,  280,
  263,  263,  263,  295,  265,  266,  267,  299,  300,  301,
  302,  284,  285,  263,  295,  265,  266,  267,  299,  300,
  301,  302,   32,  284,  285,  286,  281,  282,  283,  279,
  280,  281,  282,  283,  284,  285,  286,  287,  288,  289,
  290,  291,  292,  293,  294,  263,  263,  265,  266,  267,
  279,  280,  281,  282,  283,   64,   -1,   -1,  287,  288,
   -1,  279,  280,   -1,   -1,   -1,  284,  285,  286,  287,
  288,  289,  290,  291,  292,  293,  294,  263,   -1,  265,
  266,  267,   -1,   -1,   -1,   -1,   -1,   -1,  263,   -1,
  265,  266,  267,  279,  280,   -1,   -1,   -1,  284,  285,
  286,  287,  288,  289,  290,  291,  292,  293,  294,  284,
  285,  286,  287,  288,  289,  290,  291,  292,  293,  294,
  263,   -1,  265,  266,  267,  256,   -1,   -1,   -1,   -1,
   -1,  263,   -1,  265,  266,  267,   -1,  268,  269,  270,
  271,  284,  285,  286,  287,  288,  289,  290,  291,  292,
  293,  294,  284,  285,  286,   -1,   -1,  289,  290,  291,
  292,  293,  294,  263,   -1,  265,  266,  267,   -1,   -1,
   -1,   -1,   -1,   -1,  263,   -1,  265,  266,  267,   -1,
   -1,   -1,   -1,   -1,  284,  285,  286,   -1,   -1,  289,
  290,  291,  292,  293,  294,  284,  285,  286,   -1,   -1,
  289,  290,  291,  292,  293,  294,  263,   -1,  265,  266,
  267,   -1,   -1,   -1,   -1,   -1,   -1,  263,   -1,  265,
  266,  267,   -1,   -1,   -1,   -1,   -1,  284,  285,  286,
   -1,   -1,  289,  290,  291,  292,  293,  294,  284,  285,
  286,   -1,   -1,  289,   -1,   -1,   -1,   -1,  294,  279,
  280,  281,  282,  283,  284,  285,  286,  287,  288,  289,
  290,  291,  292,  293,  294,  279,  280,  281,  282,  283,
  284,   -1,  286,  287,  288,  289,  290,  291,  292,  293,
  294,  279,  280,  281,  282,  283,   -1,   -1,  286,  287,
  288,  289,  290,  291,  292,  293,  294,  279,  280,  281,
  282,  283,   -1,   -1,   -1,  287,  288,  289,  290,  291,
  292,  293,  294,  279,  280,  281,  282,  283,   -1,   -1,
   -1,  287,  288,   -1,  290,  291,  292,  293,
};
#if YYBTYACC
static const YYINT yyctable[] = {                        -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,
};
#endif
#define YYFINAL 2
#ifndef YYDEBUG
#define YYDEBUG 1
#endif
#define YYMAXTOKEN 306
#define YYUNDFTOKEN 334
#define YYTRANSLATE(a) ((a) > YYMAXTOKEN ? YYUNDFTOKEN : (a))
#if YYDEBUG
static const char *const yyname[] = {

"$end",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"error","CLASS","PUBLIC","STATIC",
"LBRACE","RBRACE","LPAR","RPAR","LSQ","RSQ","SEMICOLON","COMMA","BOOL","INT",
"DOUBLE","VOID","STRING","IF","WHILE","RETURN","PRINT","PARSEINT","ASSIGN",
"PLUS","MINUS","STAR","DIV","MOD","AND","OR","XOR","LSHIFT","RSHIFT","EQ","GE",
"GT","LE","LT","NE","NOT","DOTLENGTH","RESERVED","ARROW","IDENTIFIER","NATURAL",
"DECIMAL","BOOLLIT","STRLIT","UNARY","IF_PREC","ELSE","$accept","Program",
"ProgramBody","Element","MethodDecl","FieldDecl","FieldList","Type","TypeArray",
"TypeVoid","MethodHeader","MethodParams","FormalParams","FormalParamsList",
"MethodBody","MethodBodyContent","BodyElement","VarDecl","Statement","StmtList",
"MethodInvocation","Args","ExprList","Assignment","ParseArgs","Expr",
"SimpleExpr","illegal-symbol",
};
static const char *const yyrule[] = {
"$accept : Program",
"Program : CLASS IDENTIFIER LBRACE ProgramBody RBRACE",
"ProgramBody : ProgramBody Element",
"ProgramBody :",
"Element : MethodDecl",
"Element : FieldDecl",
"Element : SEMICOLON",
"MethodDecl : PUBLIC STATIC MethodHeader MethodBody",
"MethodDecl : PUBLIC STATIC error RBRACE",
"MethodDecl : PUBLIC STATIC error SEMICOLON",
"MethodDecl : error RBRACE",
"FieldDecl : PUBLIC STATIC Type IDENTIFIER FieldList SEMICOLON",
"FieldDecl : error SEMICOLON",
"FieldList : FieldList COMMA IDENTIFIER",
"FieldList :",
"Type : BOOL",
"Type : INT",
"Type : DOUBLE",
"TypeArray : STRING LSQ RSQ",
"TypeVoid : VOID",
"MethodHeader : Type IDENTIFIER LPAR MethodParams RPAR",
"MethodHeader : TypeVoid IDENTIFIER LPAR MethodParams RPAR",
"MethodParams : FormalParamsList",
"MethodParams :",
"FormalParamsList : FormalParamsList COMMA FormalParams",
"FormalParamsList : FormalParams",
"FormalParams : Type IDENTIFIER",
"FormalParams : TypeArray IDENTIFIER",
"MethodBody : LBRACE MethodBodyContent RBRACE",
"MethodBodyContent : MethodBodyContent BodyElement",
"MethodBodyContent :",
"BodyElement : Statement",
"BodyElement : VarDecl",
"VarDecl : Type IDENTIFIER FieldList SEMICOLON",
"Statement : LBRACE StmtList RBRACE",
"Statement : IF LPAR Expr RPAR Statement",
"Statement : IF LPAR Expr RPAR Statement ELSE Statement",
"Statement : WHILE LPAR Expr RPAR Statement",
"Statement : RETURN SEMICOLON",
"Statement : RETURN Expr SEMICOLON",
"Statement : MethodInvocation SEMICOLON",
"Statement : Assignment SEMICOLON",
"Statement : ParseArgs SEMICOLON",
"Statement : SEMICOLON",
"Statement : PRINT LPAR Expr RPAR SEMICOLON",
"Statement : PRINT LPAR STRLIT RPAR SEMICOLON",
"Statement : error SEMICOLON",
"Statement : error RBRACE",
"StmtList : StmtList Statement",
"StmtList :",
"MethodInvocation : IDENTIFIER LPAR Args RPAR",
"MethodInvocation : IDENTIFIER LPAR error RPAR",
"Args : Expr ExprList",
"Args :",
"ExprList : ExprList COMMA Expr",
"ExprList :",
"Assignment : IDENTIFIER ASSIGN Expr",
"ParseArgs : PARSEINT LPAR IDENTIFIER LSQ Expr RSQ RPAR",
"ParseArgs : PARSEINT LPAR error RPAR",
"Expr : Assignment",
"Expr : SimpleExpr",
"SimpleExpr : SimpleExpr PLUS SimpleExpr",
"SimpleExpr : SimpleExpr MINUS SimpleExpr",
"SimpleExpr : SimpleExpr STAR SimpleExpr",
"SimpleExpr : SimpleExpr DIV SimpleExpr",
"SimpleExpr : SimpleExpr MOD SimpleExpr",
"SimpleExpr : SimpleExpr AND SimpleExpr",
"SimpleExpr : SimpleExpr OR SimpleExpr",
"SimpleExpr : SimpleExpr XOR SimpleExpr",
"SimpleExpr : SimpleExpr LSHIFT SimpleExpr",
"SimpleExpr : SimpleExpr RSHIFT SimpleExpr",
"SimpleExpr : SimpleExpr EQ SimpleExpr",
"SimpleExpr : SimpleExpr NE SimpleExpr",
"SimpleExpr : SimpleExpr GT SimpleExpr",
"SimpleExpr : SimpleExpr GE SimpleExpr",
"SimpleExpr : SimpleExpr LT SimpleExpr",
"SimpleExpr : SimpleExpr LE SimpleExpr",
"SimpleExpr : MINUS SimpleExpr",
"SimpleExpr : PLUS SimpleExpr",
"SimpleExpr : NOT SimpleExpr",
"SimpleExpr : LPAR Expr RPAR",
"SimpleExpr : LPAR error RPAR",
"SimpleExpr : MethodInvocation",
"SimpleExpr : ParseArgs",
"SimpleExpr : IDENTIFIER",
"SimpleExpr : IDENTIFIER DOTLENGTH",
"SimpleExpr : NATURAL",
"SimpleExpr : DECIMAL",
"SimpleExpr : BOOLLIT",

};
#endif

#if YYDEBUG
int      yydebug;
#endif

int      yyerrflag;
int      yychar;
YYSTYPE  yyval;
YYSTYPE  yylval;
int      yynerrs;

#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
YYLTYPE  yyloc; /* position returned by actions */
YYLTYPE  yylloc; /* position from the lexer */
#endif

#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
#ifndef YYLLOC_DEFAULT
#define YYLLOC_DEFAULT(loc, rhs, n) \
do \
{ \
    if (n == 0) \
    { \
        (loc).first_line   = YYRHSLOC(rhs, 0).last_line; \
        (loc).first_column = YYRHSLOC(rhs, 0).last_column; \
        (loc).last_line    = YYRHSLOC(rhs, 0).last_line; \
        (loc).last_column  = YYRHSLOC(rhs, 0).last_column; \
    } \
    else \
    { \
        (loc).first_line   = YYRHSLOC(rhs, 1).first_line; \
        (loc).first_column = YYRHSLOC(rhs, 1).first_column; \
        (loc).last_line    = YYRHSLOC(rhs, n).last_line; \
        (loc).last_column  = YYRHSLOC(rhs, n).last_column; \
    } \
} while (0)
#endif /* YYLLOC_DEFAULT */
#endif /* defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED) */
#if YYBTYACC

#ifndef YYLVQUEUEGROWTH
#define YYLVQUEUEGROWTH 32
#endif
#endif /* YYBTYACC */

/* define the initial stack-sizes */
#ifdef YYSTACKSIZE
#undef YYMAXDEPTH
#define YYMAXDEPTH  YYSTACKSIZE
#else
#ifdef YYMAXDEPTH
#define YYSTACKSIZE YYMAXDEPTH
#else
#define YYSTACKSIZE 10000
#define YYMAXDEPTH  10000
#endif
#endif

#ifndef YYINITSTACKSIZE
#define YYINITSTACKSIZE 200
#endif

typedef struct {
    unsigned stacksize;
    YYINT    *s_base;
    YYINT    *s_mark;
    YYINT    *s_last;
    YYSTYPE  *l_base;
    YYSTYPE  *l_mark;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    YYLTYPE  *p_base;
    YYLTYPE  *p_mark;
#endif
} YYSTACKDATA;
#if YYBTYACC

struct YYParseState_s
{
    struct YYParseState_s *save;    /* Previously saved parser state */
    YYSTACKDATA            yystack; /* saved parser stack */
    int                    state;   /* saved parser state */
    int                    errflag; /* saved error recovery status */
    int                    lexeme;  /* saved index of the conflict lexeme in the lexical queue */
    YYINT                  ctry;    /* saved index in yyctable[] for this conflict */
};
typedef struct YYParseState_s YYParseState;
#endif /* YYBTYACC */
/* variables for the parser stack */
static YYSTACKDATA yystack;
#if YYBTYACC

/* Current parser state */
static YYParseState *yyps = 0;

/* yypath != NULL: do the full parse, starting at *yypath parser state. */
static YYParseState *yypath = 0;

/* Base of the lexical value queue */
static YYSTYPE *yylvals = 0;

/* Current position at lexical value queue */
static YYSTYPE *yylvp = 0;

/* End position of lexical value queue */
static YYSTYPE *yylve = 0;

/* The last allocated position at the lexical value queue */
static YYSTYPE *yylvlim = 0;

#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
/* Base of the lexical position queue */
static YYLTYPE *yylpsns = 0;

/* Current position at lexical position queue */
static YYLTYPE *yylpp = 0;

/* End position of lexical position queue */
static YYLTYPE *yylpe = 0;

/* The last allocated position at the lexical position queue */
static YYLTYPE *yylplim = 0;
#endif

/* Current position at lexical token queue */
static YYINT  *yylexp = 0;

static YYINT  *yylexemes = 0;
#endif /* YYBTYACC */
#line 299 "jucompiler.y"


void unpack_nodes(struct node *parent, struct node *container) {
    if (container == NULL || container->children == NULL) return;
    struct node_list *curr = container->children;
    while (curr != NULL) {
        
            addchild(parent, curr->node);
        
        curr = curr->next;
    }
}

struct node *create_multiple_decls(enum category decl_type, struct node *type_node, struct node *id_node, struct node *extra_ids_container) {
    struct node *wrapper = newnode(MethodBody, NULL,0,0); 
    
    struct node *first = newnode(decl_type, NULL,0,0);
    addchild(first, type_node);
    addchild(first, id_node);
    addchild(wrapper, first);

    if (extra_ids_container != NULL) {
        struct node_list *curr = extra_ids_container->children;
        while (curr != NULL ) {
            if (curr->node != NULL) {
            struct node *extra = newnode(decl_type, NULL,0,0);
            addchild(extra, newnode(type_node->category, NULL,0,0)); 
            addchild(extra, curr->node);
            addchild(wrapper, extra);
        }
        curr = curr->next;
        }
    }
    return wrapper;
}
#line 739 "y.tab.c"

/* For use in generated program */
#define yydepth (int)(yystack.s_mark - yystack.s_base)
#if YYBTYACC
#define yytrial (yyps->save)
#endif /* YYBTYACC */

#if YYDEBUG
#include <stdio.h>	/* needed for printf */
#endif

#include <stdlib.h>	/* needed for malloc, etc */
#include <string.h>	/* needed for memset */

/* allocate initial stack or double stack size, up to YYMAXDEPTH */
static int yygrowstack(YYSTACKDATA *data)
{
    int i;
    unsigned newsize;
    YYINT *newss;
    YYSTYPE *newvs;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    YYLTYPE *newps;
#endif

    if ((newsize = data->stacksize) == 0)
        newsize = YYINITSTACKSIZE;
    else if (newsize >= YYMAXDEPTH)
        return YYENOMEM;
    else if ((newsize *= 2) > YYMAXDEPTH)
        newsize = YYMAXDEPTH;

    i = (int) (data->s_mark - data->s_base);
    newss = (YYINT *)realloc(data->s_base, newsize * sizeof(*newss));
    if (newss == 0)
        return YYENOMEM;

    data->s_base = newss;
    data->s_mark = newss + i;

    newvs = (YYSTYPE *)realloc(data->l_base, newsize * sizeof(*newvs));
    if (newvs == 0)
        return YYENOMEM;

    data->l_base = newvs;
    data->l_mark = newvs + i;

#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    newps = (YYLTYPE *)realloc(data->p_base, newsize * sizeof(*newps));
    if (newps == 0)
        return YYENOMEM;

    data->p_base = newps;
    data->p_mark = newps + i;
#endif

    data->stacksize = newsize;
    data->s_last = data->s_base + newsize - 1;

#if YYDEBUG
    if (yydebug)
        fprintf(stderr, "%sdebug: stack size increased to %d\n", YYPREFIX, newsize);
#endif
    return 0;
}

#if YYPURE || defined(YY_NO_LEAKS)
static void yyfreestack(YYSTACKDATA *data)
{
    free(data->s_base);
    free(data->l_base);
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    free(data->p_base);
#endif
    memset(data, 0, sizeof(*data));
}
#else
#define yyfreestack(data) /* nothing */
#endif /* YYPURE || defined(YY_NO_LEAKS) */
#if YYBTYACC

static YYParseState *
yyNewState(unsigned size)
{
    YYParseState *p = (YYParseState *) malloc(sizeof(YYParseState));
    if (p == NULL) return NULL;

    p->yystack.stacksize = size;
    if (size == 0)
    {
        p->yystack.s_base = NULL;
        p->yystack.l_base = NULL;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
        p->yystack.p_base = NULL;
#endif
        return p;
    }
    p->yystack.s_base    = (YYINT *) malloc(size * sizeof(YYINT));
    if (p->yystack.s_base == NULL) return NULL;
    p->yystack.l_base    = (YYSTYPE *) malloc(size * sizeof(YYSTYPE));
    if (p->yystack.l_base == NULL) return NULL;
    memset(p->yystack.l_base, 0, size * sizeof(YYSTYPE));
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    p->yystack.p_base    = (YYLTYPE *) malloc(size * sizeof(YYLTYPE));
    if (p->yystack.p_base == NULL) return NULL;
    memset(p->yystack.p_base, 0, size * sizeof(YYLTYPE));
#endif

    return p;
}

static void
yyFreeState(YYParseState *p)
{
    yyfreestack(&p->yystack);
    free(p);
}
#endif /* YYBTYACC */

#define YYABORT  goto yyabort
#define YYREJECT goto yyabort
#define YYACCEPT goto yyaccept
#define YYERROR  goto yyerrlab
#if YYBTYACC
#define YYVALID        do { if (yyps->save)            goto yyvalid; } while(0)
#define YYVALID_NESTED do { if (yyps->save && \
                                yyps->save->save == 0) goto yyvalid; } while(0)
#endif /* YYBTYACC */

int
YYPARSE_DECL()
{
    int yym, yyn, yystate, yyresult;
#if YYBTYACC
    int yynewerrflag;
    YYParseState *yyerrctx = NULL;
#endif /* YYBTYACC */
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    YYLTYPE  yyerror_loc_range[3]; /* position of error start/end (0 unused) */
#endif
#if YYDEBUG
    const char *yys;

    if ((yys = getenv("YYDEBUG")) != 0)
    {
        yyn = *yys;
        if (yyn >= '0' && yyn <= '9')
            yydebug = yyn - '0';
    }
    if (yydebug)
        fprintf(stderr, "%sdebug[<# of symbols on state stack>]\n", YYPREFIX);
#endif
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    memset(yyerror_loc_range, 0, sizeof(yyerror_loc_range));
#endif

#if YYBTYACC
    yyps = yyNewState(0); if (yyps == 0) goto yyenomem;
    yyps->save = 0;
#endif /* YYBTYACC */
    yym = 0;
    /* yyn is set below */
    yynerrs = 0;
    yyerrflag = 0;
    yychar = YYEMPTY;
    yystate = 0;

#if YYPURE
    memset(&yystack, 0, sizeof(yystack));
#endif

    if (yystack.s_base == NULL && yygrowstack(&yystack) == YYENOMEM) goto yyoverflow;
    yystack.s_mark = yystack.s_base;
    yystack.l_mark = yystack.l_base;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    yystack.p_mark = yystack.p_base;
#endif
    yystate = 0;
    *yystack.s_mark = 0;

yyloop:
    if ((yyn = yydefred[yystate]) != 0) goto yyreduce;
    if (yychar < 0)
    {
#if YYBTYACC
        do {
        if (yylvp < yylve)
        {
            /* we're currently re-reading tokens */
            yylval = *yylvp++;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            yylloc = *yylpp++;
#endif
            yychar = *yylexp++;
            break;
        }
        if (yyps->save)
        {
            /* in trial mode; save scanner results for future parse attempts */
            if (yylvp == yylvlim)
            {   /* Enlarge lexical value queue */
                size_t p = (size_t) (yylvp - yylvals);
                size_t s = (size_t) (yylvlim - yylvals);

                s += YYLVQUEUEGROWTH;
                if ((yylexemes = (YYINT *)realloc(yylexemes, s * sizeof(YYINT))) == NULL) goto yyenomem;
                if ((yylvals   = (YYSTYPE *)realloc(yylvals, s * sizeof(YYSTYPE))) == NULL) goto yyenomem;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                if ((yylpsns   = (YYLTYPE *)realloc(yylpsns, s * sizeof(YYLTYPE))) == NULL) goto yyenomem;
#endif
                yylvp   = yylve = yylvals + p;
                yylvlim = yylvals + s;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                yylpp   = yylpe = yylpsns + p;
                yylplim = yylpsns + s;
#endif
                yylexp  = yylexemes + p;
            }
            *yylexp = (YYINT) YYLEX;
            *yylvp++ = yylval;
            yylve++;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            *yylpp++ = yylloc;
            yylpe++;
#endif
            yychar = *yylexp++;
            break;
        }
        /* normal operation, no conflict encountered */
#endif /* YYBTYACC */
        yychar = YYLEX;
#if YYBTYACC
        } while (0);
#endif /* YYBTYACC */
        if (yychar < 0) yychar = YYEOF;
#if YYDEBUG
        if (yydebug)
        {
            if ((yys = yyname[YYTRANSLATE(yychar)]) == NULL) yys = yyname[YYUNDFTOKEN];
            fprintf(stderr, "%s[%d]: state %d, reading token %d (%s)",
                            YYDEBUGSTR, yydepth, yystate, yychar, yys);
#ifdef YYSTYPE_TOSTRING
#if YYBTYACC
            if (!yytrial)
#endif /* YYBTYACC */
                fprintf(stderr, " <%s>", YYSTYPE_TOSTRING(yychar, yylval));
#endif
            fputc('\n', stderr);
        }
#endif
    }
#if YYBTYACC

    /* Do we have a conflict? */
    if (((yyn = yycindex[yystate]) != 0) && (yyn += yychar) >= 0 &&
        yyn <= YYTABLESIZE && yycheck[yyn] == (YYINT) yychar)
    {
        YYINT ctry;

        if (yypath)
        {
            YYParseState *save;
#if YYDEBUG
            if (yydebug)
                fprintf(stderr, "%s[%d]: CONFLICT in state %d: following successful trial parse\n",
                                YYDEBUGSTR, yydepth, yystate);
#endif
            /* Switch to the next conflict context */
            save = yypath;
            yypath = save->save;
            save->save = NULL;
            ctry = save->ctry;
            if (save->state != yystate) YYABORT;
            yyFreeState(save);

        }
        else
        {

            /* Unresolved conflict - start/continue trial parse */
            YYParseState *save;
#if YYDEBUG
            if (yydebug)
            {
                fprintf(stderr, "%s[%d]: CONFLICT in state %d. ", YYDEBUGSTR, yydepth, yystate);
                if (yyps->save)
                    fputs("ALREADY in conflict, continuing trial parse.\n", stderr);
                else
                    fputs("Starting trial parse.\n", stderr);
            }
#endif
            save                  = yyNewState((unsigned)(yystack.s_mark - yystack.s_base + 1));
            if (save == NULL) goto yyenomem;
            save->save            = yyps->save;
            save->state           = yystate;
            save->errflag         = yyerrflag;
            save->yystack.s_mark  = save->yystack.s_base + (yystack.s_mark - yystack.s_base);
            memcpy (save->yystack.s_base, yystack.s_base, (size_t) (yystack.s_mark - yystack.s_base + 1) * sizeof(YYINT));
            save->yystack.l_mark  = save->yystack.l_base + (yystack.l_mark - yystack.l_base);
            memcpy (save->yystack.l_base, yystack.l_base, (size_t) (yystack.l_mark - yystack.l_base + 1) * sizeof(YYSTYPE));
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            save->yystack.p_mark  = save->yystack.p_base + (yystack.p_mark - yystack.p_base);
            memcpy (save->yystack.p_base, yystack.p_base, (size_t) (yystack.p_mark - yystack.p_base + 1) * sizeof(YYLTYPE));
#endif
            ctry                  = yytable[yyn];
            if (yyctable[ctry] == -1)
            {
#if YYDEBUG
                if (yydebug && yychar >= YYEOF)
                    fprintf(stderr, "%s[%d]: backtracking 1 token\n", YYDEBUGSTR, yydepth);
#endif
                ctry++;
            }
            save->ctry = ctry;
            if (yyps->save == NULL)
            {
                /* If this is a first conflict in the stack, start saving lexemes */
                if (!yylexemes)
                {
                    yylexemes = (YYINT *) malloc((YYLVQUEUEGROWTH) * sizeof(YYINT));
                    if (yylexemes == NULL) goto yyenomem;
                    yylvals   = (YYSTYPE *) malloc((YYLVQUEUEGROWTH) * sizeof(YYSTYPE));
                    if (yylvals == NULL) goto yyenomem;
                    yylvlim   = yylvals + YYLVQUEUEGROWTH;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                    yylpsns   = (YYLTYPE *) malloc((YYLVQUEUEGROWTH) * sizeof(YYLTYPE));
                    if (yylpsns == NULL) goto yyenomem;
                    yylplim   = yylpsns + YYLVQUEUEGROWTH;
#endif
                }
                if (yylvp == yylve)
                {
                    yylvp  = yylve = yylvals;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                    yylpp  = yylpe = yylpsns;
#endif
                    yylexp = yylexemes;
                    if (yychar >= YYEOF)
                    {
                        *yylve++ = yylval;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                        *yylpe++ = yylloc;
#endif
                        *yylexp  = (YYINT) yychar;
                        yychar   = YYEMPTY;
                    }
                }
            }
            if (yychar >= YYEOF)
            {
                yylvp--;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                yylpp--;
#endif
                yylexp--;
                yychar = YYEMPTY;
            }
            save->lexeme = (int) (yylvp - yylvals);
            yyps->save   = save;
        }
        if (yytable[yyn] == ctry)
        {
#if YYDEBUG
            if (yydebug)
                fprintf(stderr, "%s[%d]: state %d, shifting to state %d\n",
                                YYDEBUGSTR, yydepth, yystate, yyctable[ctry]);
#endif
            if (yychar < 0)
            {
                yylvp++;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                yylpp++;
#endif
                yylexp++;
            }
            if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack) == YYENOMEM)
                goto yyoverflow;
            yystate = yyctable[ctry];
            *++yystack.s_mark = (YYINT) yystate;
            *++yystack.l_mark = yylval;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            *++yystack.p_mark = yylloc;
#endif
            yychar  = YYEMPTY;
            if (yyerrflag > 0) --yyerrflag;
            goto yyloop;
        }
        else
        {
            yyn = yyctable[ctry];
            goto yyreduce;
        }
    } /* End of code dealing with conflicts */
#endif /* YYBTYACC */
    if (((yyn = yysindex[yystate]) != 0) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == (YYINT) yychar)
    {
#if YYDEBUG
        if (yydebug)
            fprintf(stderr, "%s[%d]: state %d, shifting to state %d\n",
                            YYDEBUGSTR, yydepth, yystate, yytable[yyn]);
#endif
        if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack) == YYENOMEM) goto yyoverflow;
        yystate = yytable[yyn];
        *++yystack.s_mark = yytable[yyn];
        *++yystack.l_mark = yylval;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
        *++yystack.p_mark = yylloc;
#endif
        yychar = YYEMPTY;
        if (yyerrflag > 0)  --yyerrflag;
        goto yyloop;
    }
    if (((yyn = yyrindex[yystate]) != 0) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == (YYINT) yychar)
    {
        yyn = yytable[yyn];
        goto yyreduce;
    }
    if (yyerrflag != 0) goto yyinrecovery;
#if YYBTYACC

    yynewerrflag = 1;
    goto yyerrhandler;
    goto yyerrlab; /* redundant goto avoids 'unused label' warning */

yyerrlab:
    /* explicit YYERROR from an action -- pop the rhs of the rule reduced
     * before looking for error recovery */
    yystack.s_mark -= yym;
    yystate = *yystack.s_mark;
    yystack.l_mark -= yym;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    yystack.p_mark -= yym;
#endif

    yynewerrflag = 0;
yyerrhandler:
    while (yyps->save)
    {
        int ctry;
        YYParseState *save = yyps->save;
#if YYDEBUG
        if (yydebug)
            fprintf(stderr, "%s[%d]: ERROR in state %d, CONFLICT BACKTRACKING to state %d, %d tokens\n",
                            YYDEBUGSTR, yydepth, yystate, yyps->save->state,
                    (int)(yylvp - yylvals - yyps->save->lexeme));
#endif
        /* Memorize most forward-looking error state in case it's really an error. */
        if (yyerrctx == NULL || yyerrctx->lexeme < yylvp - yylvals)
        {
            /* Free old saved error context state */
            if (yyerrctx) yyFreeState(yyerrctx);
            /* Create and fill out new saved error context state */
            yyerrctx                 = yyNewState((unsigned)(yystack.s_mark - yystack.s_base + 1));
            if (yyerrctx == NULL) goto yyenomem;
            yyerrctx->save           = yyps->save;
            yyerrctx->state          = yystate;
            yyerrctx->errflag        = yyerrflag;
            yyerrctx->yystack.s_mark = yyerrctx->yystack.s_base + (yystack.s_mark - yystack.s_base);
            memcpy (yyerrctx->yystack.s_base, yystack.s_base, (size_t) (yystack.s_mark - yystack.s_base + 1) * sizeof(YYINT));
            yyerrctx->yystack.l_mark = yyerrctx->yystack.l_base + (yystack.l_mark - yystack.l_base);
            memcpy (yyerrctx->yystack.l_base, yystack.l_base, (size_t) (yystack.l_mark - yystack.l_base + 1) * sizeof(YYSTYPE));
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            yyerrctx->yystack.p_mark = yyerrctx->yystack.p_base + (yystack.p_mark - yystack.p_base);
            memcpy (yyerrctx->yystack.p_base, yystack.p_base, (size_t) (yystack.p_mark - yystack.p_base + 1) * sizeof(YYLTYPE));
#endif
            yyerrctx->lexeme         = (int) (yylvp - yylvals);
        }
        yylvp          = yylvals   + save->lexeme;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
        yylpp          = yylpsns   + save->lexeme;
#endif
        yylexp         = yylexemes + save->lexeme;
        yychar         = YYEMPTY;
        yystack.s_mark = yystack.s_base + (save->yystack.s_mark - save->yystack.s_base);
        memcpy (yystack.s_base, save->yystack.s_base, (size_t) (yystack.s_mark - yystack.s_base + 1) * sizeof(YYINT));
        yystack.l_mark = yystack.l_base + (save->yystack.l_mark - save->yystack.l_base);
        memcpy (yystack.l_base, save->yystack.l_base, (size_t) (yystack.l_mark - yystack.l_base + 1) * sizeof(YYSTYPE));
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
        yystack.p_mark = yystack.p_base + (save->yystack.p_mark - save->yystack.p_base);
        memcpy (yystack.p_base, save->yystack.p_base, (size_t) (yystack.p_mark - yystack.p_base + 1) * sizeof(YYLTYPE));
#endif
        ctry           = ++save->ctry;
        yystate        = save->state;
        /* We tried shift, try reduce now */
        if ((yyn = yyctable[ctry]) >= 0) goto yyreduce;
        yyps->save     = save->save;
        save->save     = NULL;
        yyFreeState(save);

        /* Nothing left on the stack -- error */
        if (!yyps->save)
        {
#if YYDEBUG
            if (yydebug)
                fprintf(stderr, "%sdebug[%d,trial]: trial parse FAILED, entering ERROR mode\n",
                                YYPREFIX, yydepth);
#endif
            /* Restore state as it was in the most forward-advanced error */
            yylvp          = yylvals   + yyerrctx->lexeme;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            yylpp          = yylpsns   + yyerrctx->lexeme;
#endif
            yylexp         = yylexemes + yyerrctx->lexeme;
            yychar         = yylexp[-1];
            yylval         = yylvp[-1];
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            yylloc         = yylpp[-1];
#endif
            yystack.s_mark = yystack.s_base + (yyerrctx->yystack.s_mark - yyerrctx->yystack.s_base);
            memcpy (yystack.s_base, yyerrctx->yystack.s_base, (size_t) (yystack.s_mark - yystack.s_base + 1) * sizeof(YYINT));
            yystack.l_mark = yystack.l_base + (yyerrctx->yystack.l_mark - yyerrctx->yystack.l_base);
            memcpy (yystack.l_base, yyerrctx->yystack.l_base, (size_t) (yystack.l_mark - yystack.l_base + 1) * sizeof(YYSTYPE));
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            yystack.p_mark = yystack.p_base + (yyerrctx->yystack.p_mark - yyerrctx->yystack.p_base);
            memcpy (yystack.p_base, yyerrctx->yystack.p_base, (size_t) (yystack.p_mark - yystack.p_base + 1) * sizeof(YYLTYPE));
#endif
            yystate        = yyerrctx->state;
            yyFreeState(yyerrctx);
            yyerrctx       = NULL;
        }
        yynewerrflag = 1;
    }
    if (yynewerrflag == 0) goto yyinrecovery;
#endif /* YYBTYACC */

    YYERROR_CALL("syntax error");
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    yyerror_loc_range[1] = yylloc; /* lookahead position is error start position */
#endif

#if !YYBTYACC
    goto yyerrlab; /* redundant goto avoids 'unused label' warning */
yyerrlab:
#endif
    ++yynerrs;

yyinrecovery:
    if (yyerrflag < 3)
    {
        yyerrflag = 3;
        for (;;)
        {
            if (((yyn = yysindex[*yystack.s_mark]) != 0) && (yyn += YYERRCODE) >= 0 &&
                    yyn <= YYTABLESIZE && yycheck[yyn] == (YYINT) YYERRCODE)
            {
#if YYDEBUG
                if (yydebug)
                    fprintf(stderr, "%s[%d]: state %d, error recovery shifting to state %d\n",
                                    YYDEBUGSTR, yydepth, *yystack.s_mark, yytable[yyn]);
#endif
                if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack) == YYENOMEM) goto yyoverflow;
                yystate = yytable[yyn];
                *++yystack.s_mark = yytable[yyn];
                *++yystack.l_mark = yylval;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                /* lookahead position is error end position */
                yyerror_loc_range[2] = yylloc;
                YYLLOC_DEFAULT(yyloc, yyerror_loc_range, 2); /* position of error span */
                *++yystack.p_mark = yyloc;
#endif
                goto yyloop;
            }
            else
            {
#if YYDEBUG
                if (yydebug)
                    fprintf(stderr, "%s[%d]: error recovery discarding state %d\n",
                                    YYDEBUGSTR, yydepth, *yystack.s_mark);
#endif
                if (yystack.s_mark <= yystack.s_base) goto yyabort;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                /* the current TOS position is the error start position */
                yyerror_loc_range[1] = *yystack.p_mark;
#endif
#if defined(YYDESTRUCT_CALL)
#if YYBTYACC
                if (!yytrial)
#endif /* YYBTYACC */
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                    YYDESTRUCT_CALL("error: discarding state",
                                    yystos[*yystack.s_mark], yystack.l_mark, yystack.p_mark);
#else
                    YYDESTRUCT_CALL("error: discarding state",
                                    yystos[*yystack.s_mark], yystack.l_mark);
#endif /* defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED) */
#endif /* defined(YYDESTRUCT_CALL) */
                --yystack.s_mark;
                --yystack.l_mark;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                --yystack.p_mark;
#endif
            }
        }
    }
    else
    {
        if (yychar == YYEOF) goto yyabort;
#if YYDEBUG
        if (yydebug)
        {
            if ((yys = yyname[YYTRANSLATE(yychar)]) == NULL) yys = yyname[YYUNDFTOKEN];
            fprintf(stderr, "%s[%d]: state %d, error recovery discarding token %d (%s)\n",
                            YYDEBUGSTR, yydepth, yystate, yychar, yys);
        }
#endif
#if defined(YYDESTRUCT_CALL)
#if YYBTYACC
        if (!yytrial)
#endif /* YYBTYACC */
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            YYDESTRUCT_CALL("error: discarding token", yychar, &yylval, &yylloc);
#else
            YYDESTRUCT_CALL("error: discarding token", yychar, &yylval);
#endif /* defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED) */
#endif /* defined(YYDESTRUCT_CALL) */
        yychar = YYEMPTY;
        goto yyloop;
    }

yyreduce:
    yym = yylen[yyn];
#if YYDEBUG
    if (yydebug)
    {
        fprintf(stderr, "%s[%d]: state %d, reducing by rule %d (%s)",
                        YYDEBUGSTR, yydepth, yystate, yyn, yyrule[yyn]);
#ifdef YYSTYPE_TOSTRING
#if YYBTYACC
        if (!yytrial)
#endif /* YYBTYACC */
            if (yym > 0)
            {
                int i;
                fputc('<', stderr);
                for (i = yym; i > 0; i--)
                {
                    if (i != yym) fputs(", ", stderr);
                    fputs(YYSTYPE_TOSTRING(yystos[yystack.s_mark[1-i]],
                                           yystack.l_mark[1-i]), stderr);
                }
                fputc('>', stderr);
            }
#endif
        fputc('\n', stderr);
    }
#endif
    if (yym > 0)
        yyval = yystack.l_mark[1-yym];
    else
        memset(&yyval, 0, sizeof yyval);
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)

    /* Perform position reduction */
    memset(&yyloc, 0, sizeof(yyloc));
#if YYBTYACC
    if (!yytrial)
#endif /* YYBTYACC */
    {
        YYLLOC_DEFAULT(yyloc, &yystack.p_mark[-yym], yym);
        /* just in case YYERROR is invoked within the action, save
           the start of the rhs as the error start position */
        yyerror_loc_range[1] = yystack.p_mark[1-yym];
    }
#endif

    switch (yyn)
    {
case 1:
#line 55 "jucompiler.y"
	{
yyval.node = ast = newnode(Program, NULL, 0, 0);
addchild(yyval.node, yystack.l_mark[-3].node);
unpack_nodes(ast, yystack.l_mark[-1].node);
}
#line 1416 "y.tab.c"
break;
case 2:
#line 61 "jucompiler.y"
	{  
        yyval.node = yystack.l_mark[-1].node;
        if (yystack.l_mark[0].node != NULL) {
            if (yystack.l_mark[0].node->category == MethodBody) { unpack_nodes(yyval.node, yystack.l_mark[0].node); }
            else { addchild(yyval.node, yystack.l_mark[0].node); }
        }
    }
#line 1427 "y.tab.c"
break;
case 3:
#line 68 "jucompiler.y"
	{ yyval.node = newnode(MethodBody, NULL, 0, 0); }
#line 1432 "y.tab.c"
break;
case 4:
#line 71 "jucompiler.y"
	{ yyval.node = yystack.l_mark[0].node; }
#line 1437 "y.tab.c"
break;
case 5:
#line 72 "jucompiler.y"
	{ yyval.node = yystack.l_mark[0].node; }
#line 1442 "y.tab.c"
break;
case 6:
#line 73 "jucompiler.y"
	{ yyval.node = NULL; }
#line 1447 "y.tab.c"
break;
case 7:
#line 75 "jucompiler.y"
	{
    yyval.node = newnode(MethodDecl, NULL, 0, 0);
    addchild(yyval.node, yystack.l_mark[-1].node);
    addchild(yyval.node, yystack.l_mark[0].node);
}
#line 1456 "y.tab.c"
break;
case 8:
#line 80 "jucompiler.y"
	{ yyval.node = NULL; }
#line 1461 "y.tab.c"
break;
case 9:
#line 81 "jucompiler.y"
	{ yyval.node = NULL; }
#line 1466 "y.tab.c"
break;
case 10:
#line 82 "jucompiler.y"
	{ yyval.node = NULL; }
#line 1471 "y.tab.c"
break;
case 11:
#line 84 "jucompiler.y"
	{
    yyval.node = create_multiple_decls(FieldDecl, yystack.l_mark[-3].node, yystack.l_mark[-2].node, yystack.l_mark[-1].node);
}
#line 1478 "y.tab.c"
break;
case 12:
#line 88 "jucompiler.y"
	{yyval.node=NULL;}
#line 1483 "y.tab.c"
break;
case 13:
#line 90 "jucompiler.y"
	{
    if (yystack.l_mark[-2].node == NULL) yyval.node = newnode(FieldDecl, NULL, 0, 0);
    else yyval.node = yystack.l_mark[-2].node;
    addchild(yyval.node, yystack.l_mark[0].node);
}
#line 1492 "y.tab.c"
break;
case 14:
#line 95 "jucompiler.y"
	{yyval.node = NULL;}
#line 1497 "y.tab.c"
break;
case 15:
#line 98 "jucompiler.y"
	{ yyval.node = newnode(Bool,NULL, 0, 0); }
#line 1502 "y.tab.c"
break;
case 16:
#line 99 "jucompiler.y"
	{ yyval.node = newnode(Int,NULL, 0, 0); }
#line 1507 "y.tab.c"
break;
case 17:
#line 100 "jucompiler.y"
	{ yyval.node = newnode(Double,NULL, 0, 0); }
#line 1512 "y.tab.c"
break;
case 18:
#line 102 "jucompiler.y"
	{ yyval.node = newnode(StringArray, NULL, 0, 0); }
#line 1517 "y.tab.c"
break;
case 19:
#line 104 "jucompiler.y"
	{ yyval.node = newnode(VoidNode, NULL, 0, 0); }
#line 1522 "y.tab.c"
break;
case 20:
#line 107 "jucompiler.y"
	{
    yyval.node = newnode(MethodHeader, NULL, 0, 0);
    addchild(yyval.node, yystack.l_mark[-4].node);
    addchild(yyval.node, yystack.l_mark[-3].node);
    addchild(yyval.node, yystack.l_mark[-1].node);
    }
#line 1532 "y.tab.c"
break;
case 21:
#line 113 "jucompiler.y"
	{
    yyval.node = newnode(MethodHeader, NULL, 0, 0);
    addchild(yyval.node, yystack.l_mark[-4].node);
    addchild(yyval.node, yystack.l_mark[-3].node);
    addchild(yyval.node, yystack.l_mark[-1].node);
    }
#line 1542 "y.tab.c"
break;
case 22:
#line 120 "jucompiler.y"
	{yyval.node=yystack.l_mark[0].node;}
#line 1547 "y.tab.c"
break;
case 23:
#line 121 "jucompiler.y"
	{yyval.node = newnode(MethodParams, NULL, 0, 0);}
#line 1552 "y.tab.c"
break;
case 24:
#line 123 "jucompiler.y"
	{ 
    yyval.node = yystack.l_mark[-2].node;
    addchild(yyval.node, yystack.l_mark[0].node);
}
#line 1560 "y.tab.c"
break;
case 25:
#line 127 "jucompiler.y"
	{
    yyval.node=newnode(MethodParams, NULL, 0, 0);
    addchild(yyval.node, yystack.l_mark[0].node);}
#line 1567 "y.tab.c"
break;
case 26:
#line 132 "jucompiler.y"
	{
    yyval.node = newnode(ParamDecl,NULL, 0, 0);
    addchild(yyval.node, yystack.l_mark[-1].node);
    addchild(yyval.node, yystack.l_mark[0].node);
}
#line 1576 "y.tab.c"
break;
case 27:
#line 137 "jucompiler.y"
	{
    yyval.node = newnode(ParamDecl,NULL, 0, 0);
    addchild(yyval.node, yystack.l_mark[-1].node);
    addchild(yyval.node, yystack.l_mark[0].node);
            }
#line 1585 "y.tab.c"
break;
case 28:
#line 144 "jucompiler.y"
	{
    yyval.node = yystack.l_mark[-1].node;
}
#line 1592 "y.tab.c"
break;
case 29:
#line 148 "jucompiler.y"
	{
    yyval.node = yystack.l_mark[-1].node;
    if (yystack.l_mark[0].node != NULL) {
        if (yystack.l_mark[0].node->category == MethodBody) { unpack_nodes(yyval.node, yystack.l_mark[0].node); } /* Si es un VarDecl múltiple */
        else { addchild(yyval.node, yystack.l_mark[0].node); }
    }
}
#line 1603 "y.tab.c"
break;
case 30:
#line 155 "jucompiler.y"
	{ yyval.node = newnode(MethodBody, NULL, 0, 0); }
#line 1608 "y.tab.c"
break;
case 31:
#line 158 "jucompiler.y"
	{yyval.node = yystack.l_mark[0].node;}
#line 1613 "y.tab.c"
break;
case 32:
#line 159 "jucompiler.y"
	{yyval.node = yystack.l_mark[0].node;}
#line 1618 "y.tab.c"
break;
case 33:
#line 161 "jucompiler.y"
	{
    yyval.node = create_multiple_decls(VarDecl, yystack.l_mark[-3].node, yystack.l_mark[-2].node, yystack.l_mark[-1].node);
}
#line 1625 "y.tab.c"
break;
case 34:
#line 165 "jucompiler.y"
	{
                                            int count = count_children(yystack.l_mark[-1].node);
                                            if (count == 0) {
                                                yyval.node = NULL;  /* bloque vacío explícito o null??*/
                                            } else if (count == 1) {
                                                struct node_list *curr = yystack.l_mark[-1].node->children;
                                                while (curr != NULL && curr->node == NULL) curr = curr->next;
                                                yyval.node = (curr != NULL) ? curr->node : newnode(Block, NULL, 0, 0);
                                            } else {
                                                yyval.node = newnode(Block, NULL, 0, 0);
                                                unpack_nodes(yyval.node, yystack.l_mark[-1].node);
                                            }
                                        }
#line 1642 "y.tab.c"
break;
case 35:
#line 178 "jucompiler.y"
	{
                                                yyval.node = newnode(If, NULL, 0, 0);
                                                addchild(yyval.node, yystack.l_mark[-2].node);
                                                    addchild(yyval.node, yystack.l_mark[0].node ? yystack.l_mark[0].node : newnode(Block, NULL, 0, 0));
                                                addchild(yyval.node, newnode(Block, NULL, 0, 0)); /* else branch vacio*/
                                            }
#line 1652 "y.tab.c"
break;
case 36:
#line 184 "jucompiler.y"
	{
                                                yyval.node = newnode(If, NULL, 0, 0);
                                                addchild(yyval.node, yystack.l_mark[-4].node);
                                                addchild(yyval.node, yystack.l_mark[-2].node ? yystack.l_mark[-2].node : newnode(Block, NULL, 0, 0));
                                        addchild(yyval.node, yystack.l_mark[0].node ? yystack.l_mark[0].node : newnode(Block, NULL, 0, 0));
                                            }
#line 1662 "y.tab.c"
break;
case 37:
#line 190 "jucompiler.y"
	{
                                            yyval.node = newnode(While, NULL, 0, 0);
                                            addchild(yyval.node, yystack.l_mark[-2].node);
                                                addchild(yyval.node, yystack.l_mark[0].node ? yystack.l_mark[0].node : newnode(Block, NULL, 0, 0));
                                                }
#line 1671 "y.tab.c"
break;
case 38:
#line 195 "jucompiler.y"
	{
    yyval.node = newnode(Return, NULL, 0, 0);
         }
#line 1678 "y.tab.c"
break;
case 39:
#line 198 "jucompiler.y"
	{
    yyval.node = newnode(Return, NULL, 0, 0);
    addchild(yyval.node, yystack.l_mark[-1].node);
         }
#line 1686 "y.tab.c"
break;
case 40:
#line 202 "jucompiler.y"
	{yyval.node = yystack.l_mark[-1].node;}
#line 1691 "y.tab.c"
break;
case 41:
#line 203 "jucompiler.y"
	{  yyval.node = yystack.l_mark[-1].node;    }
#line 1696 "y.tab.c"
break;
case 42:
#line 204 "jucompiler.y"
	{yyval.node = yystack.l_mark[-1].node; }
#line 1701 "y.tab.c"
break;
case 43:
#line 205 "jucompiler.y"
	{ yyval.node = NULL; }
#line 1706 "y.tab.c"
break;
case 44:
#line 206 "jucompiler.y"
	{
    yyval.node = newnode(Print, NULL, 0, 0);
    addchild(yyval.node, yystack.l_mark[-2].node);
         }
#line 1714 "y.tab.c"
break;
case 45:
#line 210 "jucompiler.y"
	{
    yyval.node = newnode(Print, NULL, 0, 0);
    addchild(yyval.node, yystack.l_mark[-2].node);
         }
#line 1722 "y.tab.c"
break;
case 46:
#line 214 "jucompiler.y"
	{yyval.node=NULL;}
#line 1727 "y.tab.c"
break;
case 47:
#line 215 "jucompiler.y"
	{yyval.node=NULL;}
#line 1732 "y.tab.c"
break;
case 48:
#line 218 "jucompiler.y"
	{
    yyval.node = yystack.l_mark[-1].node;
        if (yystack.l_mark[0].node != NULL) addchild(yyval.node, yystack.l_mark[0].node);
}
#line 1740 "y.tab.c"
break;
case 49:
#line 222 "jucompiler.y"
	{yyval.node = newnode(Aux, NULL, 0, 0); }
#line 1745 "y.tab.c"
break;
case 50:
#line 224 "jucompiler.y"
	{
        yyval.node = newnode(Call, NULL, 0, 0);
        addchild(yyval.node, yystack.l_mark[-3].node);
        unpack_nodes(yyval.node, yystack.l_mark[-1].node);
    }
#line 1754 "y.tab.c"
break;
case 51:
#line 229 "jucompiler.y"
	{ yyval.node = NULL; }
#line 1759 "y.tab.c"
break;
case 52:
#line 232 "jucompiler.y"
	{
        yyval.node = newnode(Aux, NULL, 0, 0); 
        addchild(yyval.node, yystack.l_mark[-1].node);
        unpack_nodes(yyval.node, yystack.l_mark[0].node);
    }
#line 1768 "y.tab.c"
break;
case 53:
#line 237 "jucompiler.y"
	{ yyval.node = NULL; }
#line 1773 "y.tab.c"
break;
case 54:
#line 240 "jucompiler.y"
	{
        if (yystack.l_mark[-2].node == NULL) yyval.node = newnode(Aux, NULL, 0, 0);
        else yyval.node = yystack.l_mark[-2].node;
        addchild(yyval.node, yystack.l_mark[0].node);
    }
#line 1782 "y.tab.c"
break;
case 55:
#line 245 "jucompiler.y"
	{ yyval.node = NULL; }
#line 1787 "y.tab.c"
break;
case 56:
#line 247 "jucompiler.y"
	{
    yyval.node = newnode(Assign, NULL, 0, 0);
    addchild(yyval.node, yystack.l_mark[-2].node);
    addchild(yyval.node, yystack.l_mark[0].node);
}
#line 1796 "y.tab.c"
break;
case 57:
#line 254 "jucompiler.y"
	{
    yyval.node = newnode(ParseArgs, NULL, 0, 0);
    addchild(yyval.node, yystack.l_mark[-4].node);
     addchild(yyval.node, yystack.l_mark[-2].node);
}
#line 1805 "y.tab.c"
break;
case 58:
#line 259 "jucompiler.y"
	{yyval.node = NULL;}
#line 1810 "y.tab.c"
break;
case 59:
#line 262 "jucompiler.y"
	{yyval.node = yystack.l_mark[0].node;}
#line 1815 "y.tab.c"
break;
case 60:
#line 263 "jucompiler.y"
	{yyval.node = yystack.l_mark[0].node;}
#line 1820 "y.tab.c"
break;
case 61:
#line 266 "jucompiler.y"
	{ yyval.node = newnode(Add, NULL, 0, 0); addchild(yyval.node, yystack.l_mark[-2].node); addchild(yyval.node, yystack.l_mark[0].node); }
#line 1825 "y.tab.c"
break;
case 62:
#line 267 "jucompiler.y"
	{ yyval.node = newnode(Sub, NULL, 0, 0); addchild(yyval.node, yystack.l_mark[-2].node); addchild(yyval.node, yystack.l_mark[0].node); }
#line 1830 "y.tab.c"
break;
case 63:
#line 268 "jucompiler.y"
	{ yyval.node = newnode(Mul, NULL, 0, 0); addchild(yyval.node, yystack.l_mark[-2].node); addchild(yyval.node, yystack.l_mark[0].node); }
#line 1835 "y.tab.c"
break;
case 64:
#line 269 "jucompiler.y"
	{ yyval.node = newnode(Div, NULL, 0, 0); addchild(yyval.node, yystack.l_mark[-2].node); addchild(yyval.node, yystack.l_mark[0].node); }
#line 1840 "y.tab.c"
break;
case 65:
#line 270 "jucompiler.y"
	{ yyval.node = newnode(Mod, NULL, 0, 0); addchild(yyval.node, yystack.l_mark[-2].node); addchild(yyval.node, yystack.l_mark[0].node); }
#line 1845 "y.tab.c"
break;
case 66:
#line 271 "jucompiler.y"
	{ yyval.node = newnode(And, NULL, 0, 0); addchild(yyval.node, yystack.l_mark[-2].node); addchild(yyval.node, yystack.l_mark[0].node); }
#line 1850 "y.tab.c"
break;
case 67:
#line 272 "jucompiler.y"
	{ yyval.node = newnode(Or, NULL, 0, 0); addchild(yyval.node, yystack.l_mark[-2].node); addchild(yyval.node, yystack.l_mark[0].node); }
#line 1855 "y.tab.c"
break;
case 68:
#line 273 "jucompiler.y"
	{ yyval.node = newnode(Xor, NULL, 0, 0); addchild(yyval.node, yystack.l_mark[-2].node); addchild(yyval.node, yystack.l_mark[0].node); }
#line 1860 "y.tab.c"
break;
case 69:
#line 274 "jucompiler.y"
	{ yyval.node = newnode(Lshift, NULL, 0, 0); addchild(yyval.node, yystack.l_mark[-2].node); addchild(yyval.node, yystack.l_mark[0].node); }
#line 1865 "y.tab.c"
break;
case 70:
#line 275 "jucompiler.y"
	{ yyval.node = newnode(Rshift, NULL, 0, 0); addchild(yyval.node, yystack.l_mark[-2].node); addchild(yyval.node, yystack.l_mark[0].node); }
#line 1870 "y.tab.c"
break;
case 71:
#line 276 "jucompiler.y"
	{ yyval.node = newnode(Eq, NULL, 0, 0); addchild(yyval.node, yystack.l_mark[-2].node); addchild(yyval.node, yystack.l_mark[0].node); }
#line 1875 "y.tab.c"
break;
case 72:
#line 277 "jucompiler.y"
	{ yyval.node = newnode(Ne, NULL, 0, 0); addchild(yyval.node, yystack.l_mark[-2].node); addchild(yyval.node, yystack.l_mark[0].node); }
#line 1880 "y.tab.c"
break;
case 73:
#line 278 "jucompiler.y"
	{ yyval.node = newnode(Gt, NULL, 0, 0); addchild(yyval.node, yystack.l_mark[-2].node); addchild(yyval.node, yystack.l_mark[0].node); }
#line 1885 "y.tab.c"
break;
case 74:
#line 279 "jucompiler.y"
	{ yyval.node = newnode(Ge, NULL, 0, 0); addchild(yyval.node, yystack.l_mark[-2].node); addchild(yyval.node, yystack.l_mark[0].node); }
#line 1890 "y.tab.c"
break;
case 75:
#line 280 "jucompiler.y"
	{ yyval.node = newnode(Lt, NULL, 0, 0); addchild(yyval.node, yystack.l_mark[-2].node); addchild(yyval.node, yystack.l_mark[0].node); }
#line 1895 "y.tab.c"
break;
case 76:
#line 281 "jucompiler.y"
	{ yyval.node = newnode(Le, NULL, 0, 0); addchild(yyval.node, yystack.l_mark[-2].node); addchild(yyval.node, yystack.l_mark[0].node); }
#line 1900 "y.tab.c"
break;
case 77:
#line 282 "jucompiler.y"
	{ yyval.node = newnode(Minus, NULL, 0, 0); addchild(yyval.node, yystack.l_mark[0].node); }
#line 1905 "y.tab.c"
break;
case 78:
#line 283 "jucompiler.y"
	{ yyval.node = newnode(Plus, NULL, 0, 0); addchild(yyval.node, yystack.l_mark[0].node); }
#line 1910 "y.tab.c"
break;
case 79:
#line 284 "jucompiler.y"
	{ yyval.node = newnode(Not, NULL, 0, 0); addchild(yyval.node, yystack.l_mark[0].node); }
#line 1915 "y.tab.c"
break;
case 80:
#line 285 "jucompiler.y"
	{ yyval.node = yystack.l_mark[-1].node; }
#line 1920 "y.tab.c"
break;
case 81:
#line 286 "jucompiler.y"
	{ yyval.node = NULL; }
#line 1925 "y.tab.c"
break;
case 82:
#line 287 "jucompiler.y"
	{ yyval.node = yystack.l_mark[0].node; }
#line 1930 "y.tab.c"
break;
case 83:
#line 288 "jucompiler.y"
	{ yyval.node = yystack.l_mark[0].node; }
#line 1935 "y.tab.c"
break;
case 84:
#line 289 "jucompiler.y"
	{ yyval.node = yystack.l_mark[0].node; }
#line 1940 "y.tab.c"
break;
case 85:
#line 290 "jucompiler.y"
	{
              yyval.node = newnode(Length, NULL, 0, 0);
              addchild(yyval.node, yystack.l_mark[-1].node);
            }
#line 1948 "y.tab.c"
break;
case 86:
#line 294 "jucompiler.y"
	{ yyval.node = yystack.l_mark[0].node; }
#line 1953 "y.tab.c"
break;
case 87:
#line 295 "jucompiler.y"
	{ yyval.node = yystack.l_mark[0].node; }
#line 1958 "y.tab.c"
break;
case 88:
#line 296 "jucompiler.y"
	{ yyval.node = yystack.l_mark[0].node; }
#line 1963 "y.tab.c"
break;
#line 1965 "y.tab.c"
    default:
        break;
    }
    yystack.s_mark -= yym;
    yystate = *yystack.s_mark;
    yystack.l_mark -= yym;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    yystack.p_mark -= yym;
#endif
    yym = yylhs[yyn];
    if (yystate == 0 && yym == 0)
    {
#if YYDEBUG
        if (yydebug)
        {
            fprintf(stderr, "%s[%d]: after reduction, ", YYDEBUGSTR, yydepth);
#ifdef YYSTYPE_TOSTRING
#if YYBTYACC
            if (!yytrial)
#endif /* YYBTYACC */
                fprintf(stderr, "result is <%s>, ", YYSTYPE_TOSTRING(yystos[YYFINAL], yyval));
#endif
            fprintf(stderr, "shifting from state 0 to final state %d\n", YYFINAL);
        }
#endif
        yystate = YYFINAL;
        *++yystack.s_mark = YYFINAL;
        *++yystack.l_mark = yyval;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
        *++yystack.p_mark = yyloc;
#endif
        if (yychar < 0)
        {
#if YYBTYACC
            do {
            if (yylvp < yylve)
            {
                /* we're currently re-reading tokens */
                yylval = *yylvp++;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                yylloc = *yylpp++;
#endif
                yychar = *yylexp++;
                break;
            }
            if (yyps->save)
            {
                /* in trial mode; save scanner results for future parse attempts */
                if (yylvp == yylvlim)
                {   /* Enlarge lexical value queue */
                    size_t p = (size_t) (yylvp - yylvals);
                    size_t s = (size_t) (yylvlim - yylvals);

                    s += YYLVQUEUEGROWTH;
                    if ((yylexemes = (YYINT *)realloc(yylexemes, s * sizeof(YYINT))) == NULL)
                        goto yyenomem;
                    if ((yylvals   = (YYSTYPE *)realloc(yylvals, s * sizeof(YYSTYPE))) == NULL)
                        goto yyenomem;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                    if ((yylpsns   = (YYLTYPE *)realloc(yylpsns, s * sizeof(YYLTYPE))) == NULL)
                        goto yyenomem;
#endif
                    yylvp   = yylve = yylvals + p;
                    yylvlim = yylvals + s;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                    yylpp   = yylpe = yylpsns + p;
                    yylplim = yylpsns + s;
#endif
                    yylexp  = yylexemes + p;
                }
                *yylexp = (YYINT) YYLEX;
                *yylvp++ = yylval;
                yylve++;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                *yylpp++ = yylloc;
                yylpe++;
#endif
                yychar = *yylexp++;
                break;
            }
            /* normal operation, no conflict encountered */
#endif /* YYBTYACC */
            yychar = YYLEX;
#if YYBTYACC
            } while (0);
#endif /* YYBTYACC */
            if (yychar < 0) yychar = YYEOF;
#if YYDEBUG
            if (yydebug)
            {
                if ((yys = yyname[YYTRANSLATE(yychar)]) == NULL) yys = yyname[YYUNDFTOKEN];
                fprintf(stderr, "%s[%d]: state %d, reading token %d (%s)\n",
                                YYDEBUGSTR, yydepth, YYFINAL, yychar, yys);
            }
#endif
        }
        if (yychar == YYEOF) goto yyaccept;
        goto yyloop;
    }
    if (((yyn = yygindex[yym]) != 0) && (yyn += yystate) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == (YYINT) yystate)
        yystate = yytable[yyn];
    else
        yystate = yydgoto[yym];
#if YYDEBUG
    if (yydebug)
    {
        fprintf(stderr, "%s[%d]: after reduction, ", YYDEBUGSTR, yydepth);
#ifdef YYSTYPE_TOSTRING
#if YYBTYACC
        if (!yytrial)
#endif /* YYBTYACC */
            fprintf(stderr, "result is <%s>, ", YYSTYPE_TOSTRING(yystos[yystate], yyval));
#endif
        fprintf(stderr, "shifting from state %d to state %d\n", *yystack.s_mark, yystate);
    }
#endif
    if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack) == YYENOMEM) goto yyoverflow;
    *++yystack.s_mark = (YYINT) yystate;
    *++yystack.l_mark = yyval;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    *++yystack.p_mark = yyloc;
#endif
    goto yyloop;
#if YYBTYACC

    /* Reduction declares that this path is valid. Set yypath and do a full parse */
yyvalid:
    if (yypath) YYABORT;
    while (yyps->save)
    {
        YYParseState *save = yyps->save;
        yyps->save = save->save;
        save->save = yypath;
        yypath = save;
    }
#if YYDEBUG
    if (yydebug)
        fprintf(stderr, "%s[%d]: state %d, CONFLICT trial successful, backtracking to state %d, %d tokens\n",
                        YYDEBUGSTR, yydepth, yystate, yypath->state, (int)(yylvp - yylvals - yypath->lexeme));
#endif
    if (yyerrctx)
    {
        yyFreeState(yyerrctx);
        yyerrctx = NULL;
    }
    yylvp          = yylvals + yypath->lexeme;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    yylpp          = yylpsns + yypath->lexeme;
#endif
    yylexp         = yylexemes + yypath->lexeme;
    yychar         = YYEMPTY;
    yystack.s_mark = yystack.s_base + (yypath->yystack.s_mark - yypath->yystack.s_base);
    memcpy (yystack.s_base, yypath->yystack.s_base, (size_t) (yystack.s_mark - yystack.s_base + 1) * sizeof(YYINT));
    yystack.l_mark = yystack.l_base + (yypath->yystack.l_mark - yypath->yystack.l_base);
    memcpy (yystack.l_base, yypath->yystack.l_base, (size_t) (yystack.l_mark - yystack.l_base + 1) * sizeof(YYSTYPE));
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    yystack.p_mark = yystack.p_base + (yypath->yystack.p_mark - yypath->yystack.p_base);
    memcpy (yystack.p_base, yypath->yystack.p_base, (size_t) (yystack.p_mark - yystack.p_base + 1) * sizeof(YYLTYPE));
#endif
    yystate        = yypath->state;
    goto yyloop;
#endif /* YYBTYACC */

yyoverflow:
    YYERROR_CALL("yacc stack overflow");
#if YYBTYACC
    goto yyabort_nomem;
yyenomem:
    YYERROR_CALL("memory exhausted");
yyabort_nomem:
#endif /* YYBTYACC */
    yyresult = 2;
    goto yyreturn;

yyabort:
    yyresult = 1;
    goto yyreturn;

yyaccept:
#if YYBTYACC
    if (yyps->save) goto yyvalid;
#endif /* YYBTYACC */
    yyresult = 0;

yyreturn:
#if defined(YYDESTRUCT_CALL)
    if (yychar != YYEOF && yychar != YYEMPTY)
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
        YYDESTRUCT_CALL("cleanup: discarding token", yychar, &yylval, &yylloc);
#else
        YYDESTRUCT_CALL("cleanup: discarding token", yychar, &yylval);
#endif /* defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED) */

    {
        YYSTYPE *pv;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
        YYLTYPE *pp;

        for (pv = yystack.l_base, pp = yystack.p_base; pv <= yystack.l_mark; ++pv, ++pp)
             YYDESTRUCT_CALL("cleanup: discarding state",
                             yystos[*(yystack.s_base + (pv - yystack.l_base))], pv, pp);
#else
        for (pv = yystack.l_base; pv <= yystack.l_mark; ++pv)
             YYDESTRUCT_CALL("cleanup: discarding state",
                             yystos[*(yystack.s_base + (pv - yystack.l_base))], pv);
#endif /* defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED) */
    }
#endif /* defined(YYDESTRUCT_CALL) */

#if YYBTYACC
    if (yyerrctx)
    {
        yyFreeState(yyerrctx);
        yyerrctx = NULL;
    }
    while (yyps)
    {
        YYParseState *save = yyps;
        yyps = save->save;
        save->save = NULL;
        yyFreeState(save);
    }
    while (yypath)
    {
        YYParseState *save = yypath;
        yypath = save->save;
        save->save = NULL;
        yyFreeState(save);
    }
#endif /* YYBTYACC */
    yyfreestack(&yystack);
    return (yyresult);
}
