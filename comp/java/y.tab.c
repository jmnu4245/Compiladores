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

#line 3 "jucompiler.y"
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"


int yylex(void);
void yyerror(char *);
struct node *ast;

struct node *create_multiple_decls(enum category decl_type, struct node *type_node, char *first_id_token, struct node *extra_ids_container);
int count_children(struct node *n);
void unpack_nodes(struct node *parent, struct node *container);
#ifdef YYSTYPE
#undef  YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
#endif
#ifndef YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
#line 17 "jucompiler.y"
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
#define IDENTIFIER 297
#define NATURAL 298
#define DECIMAL 299
#define BOOLLIT 300
#define STRLIT 301
#define UNARY 302
#define IF_PREC 303
#define ELSE 304
#define YYERRCODE 256
typedef int YYINT;
static const YYINT yylhs[] = {                           -1,
    0,    1,    1,    1,    2,    2,    2,    3,    4,    4,
    5,    5,    6,    6,    6,    7,    8,    9,    9,   10,
   10,   12,   12,   11,   11,   13,   14,   14,   14,   15,
   15,   16,   17,   17,   17,   17,   17,   17,   17,   17,
   17,   17,   17,   17,   17,   18,   18,   19,   19,   20,
   20,   21,   21,   22,   23,   23,   24,   24,   24,   24,
   24,   24,   24,   24,   24,   24,   24,   24,   24,   24,
   24,   24,   24,   24,   24,   24,   24,   24,   24,   24,
   24,   24,   24,   24,   24,   24,
};
static const YYINT yylen[] = {                            2,
    5,    2,    1,    0,    1,    1,    1,    4,    6,    2,
    3,    0,    1,    1,    1,    3,    1,    5,    5,    1,
    0,    3,    1,    2,    2,    3,    2,    1,    0,    1,
    1,    4,    3,    5,    7,    5,    2,    3,    2,    2,
    2,    1,    5,    5,    2,    2,    0,    4,    4,    2,
    0,    3,    0,    3,    7,    4,    3,    3,    3,    3,
    3,    3,    3,    3,    3,    3,    3,    3,    3,    3,
    3,    3,    2,    2,    2,    3,    3,    1,    1,    1,
    1,    2,    1,    1,    1,    1,
};
static const YYINT yydefred[] = {                         0,
    0,    0,    0,    0,    0,    0,    7,    0,    3,    5,
    6,   10,    0,    1,    2,   13,   14,   15,    0,    0,
    0,    0,    0,    0,    8,    0,    0,    0,    0,   47,
   42,    0,    0,    0,    0,    0,    0,    0,    0,   28,
   31,   30,    0,    0,    0,    0,    0,    0,    0,   23,
    0,    0,    9,    0,   45,    0,    0,    0,    0,   37,
    0,    0,    0,    0,   83,   84,   85,   86,   78,   79,
   80,    0,    0,    0,    0,    0,   12,   26,   27,   39,
   40,   41,    0,   24,   25,   19,    0,   18,   11,   33,
   46,    0,    0,    0,    0,   74,   73,   75,   82,   38,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,   16,   22,    0,    0,   77,
   76,    0,    0,   59,   60,   61,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,   56,
    0,   49,   48,    0,   32,    0,   36,   44,   43,    0,
    0,    0,    0,    0,   35,   55,
};
#if defined(YYDESTRUCT_CALL) || defined(YYSTYPE_TOSTRING)
static const YYINT yystos[] = {                           0,
  257,  306,  297,  260,  256,  258,  266,  307,  308,  309,
  310,  266,  259,  261,  308,  268,  269,  270,  271,  312,
  315,  297,  297,  260,  319,  262,  262,  311,  256,  260,
  266,  273,  274,  275,  276,  277,  297,  312,  320,  321,
  322,  323,  325,  328,  329,  272,  312,  313,  316,  317,
  318,  316,  266,  267,  266,  324,  262,  262,  262,  266,
  279,  280,  295,  297,  298,  299,  300,  301,  325,  328,
  329,  330,  262,  262,  262,  278,  297,  261,  321,  266,
  266,  266,  264,  297,  297,  263,  267,  263,  297,  261,
  323,  330,  330,  256,  330,  330,  330,  330,  296,  266,
  279,  280,  281,  282,  283,  284,  285,  286,  287,  288,
  289,  290,  291,  292,  293,  294,  301,  330,  256,  297,
  256,  326,  330,  330,  311,  265,  317,  263,  263,  263,
  263,  330,  330,  330,  330,  330,  330,  330,  330,  330,
  330,  330,  330,  330,  330,  330,  330,  263,  263,  263,
  264,  263,  263,  327,  266,  323,  323,  266,  266,  330,
  267,  304,  265,  330,  323,  263,
};
#endif /* YYDESTRUCT_CALL || YYSTYPE_TOSTRING */
static const YYINT yydgoto[] = {                          2,
    8,    9,   10,   11,   28,   47,   48,    0,   21,   49,
   50,   51,   25,   39,   40,   41,   42,   56,   69,  122,
  154,   70,   71,   72,
};
static const YYINT yysindex[] = {                      -246,
 -296,    0, -244, -238, -236, -214,    0, -165,    0,    0,
    0,    0, -134,    0,    0,    0,    0,    0, -205, -203,
 -209, -194, -163, -227,    0, -145, -145, -229, -166,    0,
    0, -159, -141,  -39, -132, -123, -255, -169,  -90,    0,
    0,    0, -121, -120, -111, -104, -125, -124,  -89,    0,
  -92,  -86,    0, -116,    0,  -40, -148, -148, -182,    0,
 -148, -148, -148, -256,    0,    0,    0,    0,    0,    0,
    0,  409,  -32, -253, -136, -148,    0,    0,    0,    0,
    0,    0,  -83,    0,    0,    0, -145,    0,    0,    0,
    0,   39,   71,  -75,  103,    0,    0,    0,    0,    0,
 -148, -148, -148, -148, -148, -148, -148, -148, -148, -148,
 -148, -148, -148, -148, -148, -148,  -74,  135,  -73,  -72,
  -70,  -69,  425,  425, -195,    0,    0, -187, -187,    0,
    0, -114, -114,    0,    0,    0,  489,  457,  473, -198,
 -198,   56,   26,   26,   26,   26,   56,  -68,  -67,    0,
 -148,    0,    0,  -61,    0, -107,    0,    0,    0,  380,
 -148, -187,  -63,  425,    0,    0,
};
static const YYINT yyrindex[] = {                         0,
    0,    0,    0,  -59,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, -191,  -48,    0,  -46,  -46,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  -45,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    7,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  -44,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  441,    0,    0,    0,
    0,    0, -242, -109,    0,    0,    0,    0,    0,    0,
    0,  167,  199,    0,    0,    0,  371,  -14,   18,  210,
  242,  339,  253,  285,  296,  328,  364,    0,    0,    0,
    0,    0,    0,  -41,    0,  -65,    0,    0,    0,    0,
    0,    0,    0, -232,    0,    0,
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
    0,    0,    0,    0,    0,    0,
};
#endif
static const YYINT yygindex[] = {                         0,
    0,  206,    0,    0,  138,  109,    0,    0,    0,  197,
  141,    0,    0,    0,  186,    0,  -51,    0,  -24,    0,
    0,  -22,  -20,  -49,
};
#define YYTABLESIZE 783
static const YYINT yytable[] = {                         43,
    3,   44,  119,   45,   91,   75,   75,   92,   93,   95,
    1,   96,   97,   98,   43,    4,   44,    5,   45,    6,
   53,   76,   76,  118,   53,  123,  124,    7,   29,   12,
   52,   43,   30,   44,   52,   45,   53,   54,   31,   99,
   16,   17,   18,  120,   13,   32,   33,   34,   35,   36,
   24,  132,  133,  134,  135,  136,  137,  138,  139,  140,
  141,  142,  143,  144,  145,  146,  147,   26,   29,   37,
  155,   54,   30,   94,   12,   12,  156,  157,   31,   59,
  101,  102,  103,  104,  105,   32,   33,   34,   35,   36,
    5,   22,    6,   23,   36,   14,   61,   62,   27,   55,
    7,  160,   57,   43,   43,   44,   44,   45,   45,   37,
  165,  164,   63,   59,   64,   65,   66,   67,   68,  121,
   58,   20,   16,   17,   18,   59,   46,   77,   36,   73,
   61,   62,   38,   16,   17,   18,   19,   43,   74,   44,
   36,   45,   61,   62,   80,   81,   63,   38,   64,   65,
   66,   67,   68,   54,   82,   54,   54,   54,   63,   83,
   64,   65,   66,   67,   68,   29,  103,  104,  105,   30,
   78,   84,   85,   86,   87,   31,   88,   16,   17,   18,
   89,  126,   32,   33,   34,   35,   36,  130,  148,  150,
   34,  151,  152,  153,   34,   34,  162,  158,  159,  166,
   34,    4,   34,   34,   34,  161,   37,   34,   34,   34,
   34,   34,   29,   15,  125,   29,   21,   20,   51,   30,
   90,   50,   59,   52,   79,   31,   60,  127,    0,   59,
    0,   34,   32,   33,   34,   35,   36,   36,    0,   61,
   62,    0,    0,    0,   36,    0,   61,   62,   63,    0,
   63,   63,   63,    0,    0,   63,   37,   64,   65,   66,
   67,   68,   63,    0,   64,   65,   66,   67,  117,   81,
   63,   81,   81,   81,    0,    0,    0,    0,    0,    0,
   64,    0,   64,   64,   64,   81,   81,   81,   81,   81,
   81,   81,   81,   81,   81,   81,   81,   81,   81,   81,
   81,  128,   64,   64,  101,  102,  103,  104,  105,    0,
    0,    0,  109,  110,    0,    0,    0,  101,  102,  103,
  104,  105,  106,  107,  108,  109,  110,  111,  112,  113,
  114,  115,  116,  129,  101,  102,  103,  104,  105,    0,
    0,    0,  109,  110,    0,  112,  113,  114,  115,  101,
  102,  103,  104,  105,  106,  107,  108,  109,  110,  111,
  112,  113,  114,  115,  116,  131,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  101,  102,  103,  104,  105,  106,  107,  108,  109,
  110,  111,  112,  113,  114,  115,  116,  149,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  101,  102,  103,  104,  105,  106,  107,
  108,  109,  110,  111,  112,  113,  114,  115,  116,   57,
    0,   57,   57,   57,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,   57,   57,    0,    0,    0,
   57,   57,   57,   57,   57,   57,   57,   57,   57,   57,
   57,   58,    0,   58,   58,   58,    0,    0,    0,    0,
    0,    0,   65,    0,   65,   65,   65,   58,   58,    0,
    0,    0,   58,   58,   58,   58,   58,   58,   58,   58,
   58,   58,   58,   65,   65,   65,   65,   65,   65,   65,
   65,   65,   65,   65,   66,    0,   66,   66,   66,    0,
    0,    0,    0,    0,    0,   70,    0,   70,   70,   70,
    0,    0,    0,    0,    0,   66,   66,   66,   66,   66,
   66,   66,   66,   66,   66,   66,   70,   70,   70,    0,
    0,   70,   70,   70,   70,   70,   70,   69,    0,   69,
   69,   69,    0,    0,    0,    0,    0,    0,   72,    0,
   72,   72,   72,    0,    0,    0,    0,    0,   69,   69,
   69,    0,    0,   69,   69,   69,   69,   69,   69,   72,
   72,   72,    0,    0,   72,   72,   72,   72,   72,   72,
   71,    0,   71,   71,   71,    0,    0,    0,    0,    0,
    0,   67,    0,   67,   67,   67,    0,    0,    0,    0,
    0,   71,   71,   71,    0,    0,   71,   71,   71,   71,
   71,   71,   67,   67,   67,    0,   68,   67,   68,   68,
   68,    0,   67,   62,    0,   62,   62,   62,    0,    0,
    0,    0,    0,    0,  163,    0,    0,   68,   68,   68,
    0,    0,   68,    0,   62,   62,   62,   68,  101,  102,
  103,  104,  105,  106,  107,  108,  109,  110,  111,  112,
  113,  114,  115,  116,  100,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  101,  102,  103,
  104,  105,  106,  107,  108,  109,  110,  111,  112,  113,
  114,  115,  116,  101,  102,  103,  104,  105,  106,  107,
  108,  109,  110,  111,  112,  113,  114,  115,  116,   86,
   86,   86,   86,   86,   86,   86,   86,   86,   86,   86,
   86,   86,   86,   86,   86,  101,  102,  103,  104,  105,
  106,    0,  108,  109,  110,  111,  112,  113,  114,  115,
  116,  101,  102,  103,  104,  105,  106,    0,    0,  109,
  110,  111,  112,  113,  114,  115,  116,  101,  102,  103,
  104,  105,    0,    0,    0,  109,  110,  111,  112,  113,
  114,  115,  116,
};
static const YYINT yycheck[] = {                         24,
  297,   24,  256,   24,   56,  262,  262,   57,   58,   59,
  257,   61,   62,   63,   39,  260,   39,  256,   39,  258,
  263,  278,  278,   73,  267,   75,   76,  266,  256,  266,
  263,   56,  260,   56,  267,   56,  266,  267,  266,  296,
  268,  269,  270,  297,  259,  273,  274,  275,  276,  277,
  260,  101,  102,  103,  104,  105,  106,  107,  108,  109,
  110,  111,  112,  113,  114,  115,  116,  262,  256,  297,
  266,  267,  260,  256,  266,  267,  128,  129,  266,  262,
  279,  280,  281,  282,  283,  273,  274,  275,  276,  277,
  256,  297,  258,  297,  277,  261,  279,  280,  262,  266,
  266,  151,  262,  128,  129,  128,  129,  128,  129,  297,
  162,  161,  295,  262,  297,  298,  299,  300,  301,  256,
  262,   13,  268,  269,  270,  262,  272,  297,  277,  262,
  279,  280,   24,  268,  269,  270,  271,  162,  262,  162,
  277,  162,  279,  280,  266,  266,  295,   39,  297,  298,
  299,  300,  301,  263,  266,  265,  266,  267,  295,  264,
  297,  298,  299,  300,  301,  256,  281,  282,  283,  260,
  261,  297,  297,  263,  267,  266,  263,  268,  269,  270,
  297,  265,  273,  274,  275,  276,  277,  263,  263,  263,
  256,  264,  263,  263,  260,  261,  304,  266,  266,  263,
  266,  261,  268,  269,  270,  267,  297,  273,  274,  275,
  276,  277,  261,    8,   77,  256,  263,  263,  263,  260,
  261,  263,  262,   27,   39,  266,  266,   87,   -1,  262,
   -1,  297,  273,  274,  275,  276,  277,  277,   -1,  279,
  280,   -1,   -1,   -1,  277,   -1,  279,  280,  263,   -1,
  265,  266,  267,   -1,   -1,  295,  297,  297,  298,  299,
  300,  301,  295,   -1,  297,  298,  299,  300,  301,  263,
  285,  265,  266,  267,   -1,   -1,   -1,   -1,   -1,   -1,
  263,   -1,  265,  266,  267,  279,  280,  281,  282,  283,
  284,  285,  286,  287,  288,  289,  290,  291,  292,  293,
  294,  263,  285,  286,  279,  280,  281,  282,  283,   -1,
   -1,   -1,  287,  288,   -1,   -1,   -1,  279,  280,  281,
  282,  283,  284,  285,  286,  287,  288,  289,  290,  291,
  292,  293,  294,  263,  279,  280,  281,  282,  283,   -1,
   -1,   -1,  287,  288,   -1,  290,  291,  292,  293,  279,
  280,  281,  282,  283,  284,  285,  286,  287,  288,  289,
  290,  291,  292,  293,  294,  263,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  279,  280,  281,  282,  283,  284,  285,  286,  287,
  288,  289,  290,  291,  292,  293,  294,  263,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  279,  280,  281,  282,  283,  284,  285,
  286,  287,  288,  289,  290,  291,  292,  293,  294,  263,
   -1,  265,  266,  267,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  279,  280,   -1,   -1,   -1,
  284,  285,  286,  287,  288,  289,  290,  291,  292,  293,
  294,  263,   -1,  265,  266,  267,   -1,   -1,   -1,   -1,
   -1,   -1,  263,   -1,  265,  266,  267,  279,  280,   -1,
   -1,   -1,  284,  285,  286,  287,  288,  289,  290,  291,
  292,  293,  294,  284,  285,  286,  287,  288,  289,  290,
  291,  292,  293,  294,  263,   -1,  265,  266,  267,   -1,
   -1,   -1,   -1,   -1,   -1,  263,   -1,  265,  266,  267,
   -1,   -1,   -1,   -1,   -1,  284,  285,  286,  287,  288,
  289,  290,  291,  292,  293,  294,  284,  285,  286,   -1,
   -1,  289,  290,  291,  292,  293,  294,  263,   -1,  265,
  266,  267,   -1,   -1,   -1,   -1,   -1,   -1,  263,   -1,
  265,  266,  267,   -1,   -1,   -1,   -1,   -1,  284,  285,
  286,   -1,   -1,  289,  290,  291,  292,  293,  294,  284,
  285,  286,   -1,   -1,  289,  290,  291,  292,  293,  294,
  263,   -1,  265,  266,  267,   -1,   -1,   -1,   -1,   -1,
   -1,  263,   -1,  265,  266,  267,   -1,   -1,   -1,   -1,
   -1,  284,  285,  286,   -1,   -1,  289,  290,  291,  292,
  293,  294,  284,  285,  286,   -1,  263,  289,  265,  266,
  267,   -1,  294,  263,   -1,  265,  266,  267,   -1,   -1,
   -1,   -1,   -1,   -1,  265,   -1,   -1,  284,  285,  286,
   -1,   -1,  289,   -1,  284,  285,  286,  294,  279,  280,
  281,  282,  283,  284,  285,  286,  287,  288,  289,  290,
  291,  292,  293,  294,  266,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  279,  280,  281,
  282,  283,  284,  285,  286,  287,  288,  289,  290,  291,
  292,  293,  294,  279,  280,  281,  282,  283,  284,  285,
  286,  287,  288,  289,  290,  291,  292,  293,  294,  279,
  280,  281,  282,  283,  284,  285,  286,  287,  288,  289,
  290,  291,  292,  293,  294,  279,  280,  281,  282,  283,
  284,   -1,  286,  287,  288,  289,  290,  291,  292,  293,
  294,  279,  280,  281,  282,  283,  284,   -1,   -1,  287,
  288,  289,  290,  291,  292,  293,  294,  279,  280,  281,
  282,  283,   -1,   -1,   -1,  287,  288,  289,  290,  291,
  292,  293,  294,
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
   -1,   -1,
};
#endif
#define YYFINAL 2
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 304
#define YYUNDFTOKEN 331
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
"GT","LE","LT","NE","NOT","DOTLENGTH","IDENTIFIER","NATURAL","DECIMAL",
"BOOLLIT","STRLIT","UNARY","IF_PREC","ELSE","$accept","Program","ProgramBody",
"Element","MethodDecl","FieldDecl","FieldList","Type","TypeArray","TypeVoid",
"MethodHeader","MethodParams","FormalParams","FormalParamsList","MethodBody",
"MethodBodyContent","BodyElement","VarDecl","Statement","StmtList",
"MethodInvocation","Args","ExprList","Assignment","ParseArgs","Expr",
"illegal-symbol",
};
static const char *const yyrule[] = {
"$accept : Program",
"Program : CLASS IDENTIFIER LBRACE ProgramBody RBRACE",
"ProgramBody : ProgramBody Element",
"ProgramBody : Element",
"ProgramBody :",
"Element : MethodDecl",
"Element : FieldDecl",
"Element : SEMICOLON",
"MethodDecl : PUBLIC STATIC MethodHeader MethodBody",
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
"MethodHeader : VOID IDENTIFIER LPAR MethodParams RPAR",
"MethodParams : FormalParamsList",
"MethodParams :",
"FormalParamsList : FormalParamsList COMMA FormalParams",
"FormalParamsList : FormalParams",
"FormalParams : Type IDENTIFIER",
"FormalParams : TypeArray IDENTIFIER",
"MethodBody : LBRACE MethodBodyContent RBRACE",
"MethodBodyContent : MethodBodyContent BodyElement",
"MethodBodyContent : BodyElement",
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
"Expr : Expr PLUS Expr",
"Expr : Expr MINUS Expr",
"Expr : Expr STAR Expr",
"Expr : Expr DIV Expr",
"Expr : Expr MOD Expr",
"Expr : Expr AND Expr",
"Expr : Expr OR Expr",
"Expr : Expr XOR Expr",
"Expr : Expr LSHIFT Expr",
"Expr : Expr RSHIFT Expr",
"Expr : Expr EQ Expr",
"Expr : Expr NE Expr",
"Expr : Expr GT Expr",
"Expr : Expr GE Expr",
"Expr : Expr LT Expr",
"Expr : Expr LE Expr",
"Expr : MINUS Expr",
"Expr : PLUS Expr",
"Expr : NOT Expr",
"Expr : LPAR Expr RPAR",
"Expr : LPAR error RPAR",
"Expr : MethodInvocation",
"Expr : Assignment",
"Expr : ParseArgs",
"Expr : IDENTIFIER",
"Expr : IDENTIFIER DOTLENGTH",
"Expr : NATURAL",
"Expr : DECIMAL",
"Expr : BOOLLIT",
"Expr : STRLIT",

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
#line 296 "jucompiler.y"
int count_children(struct node *n) {
    if (n == NULL || n->children == NULL) return 0;
    int count = 0;
    struct node_list *curr = n->children;
    while (curr != NULL) {
        if (curr->node != NULL) count++;
        curr = curr->next;
    }
    return count;
}

void unpack_nodes(struct node *parent, struct node *container) {
    if (container == NULL || container->children == NULL) return;
    struct node_list *curr = container->children;
    while (curr != NULL) {
        if (curr->node != NULL) {
            addchild(parent, curr->node);
        }
        curr = curr->next;
    }
}

struct node *create_multiple_decls(enum category decl_type, struct node *type_node, char *first_id_token, struct node *extra_ids_container) {
    struct node *wrapper = newnode(MethodBody, NULL); 
    
    struct node *first = newnode(decl_type, NULL);
    addchild(first, type_node);
    addchild(first, newnode(Identifier, first_id_token));
    addchild(wrapper, first);

    if (extra_ids_container != NULL) {
        struct node_list *curr = extra_ids_container->children;
        while (curr != NULL && curr->node != NULL) {
            struct node *extra = newnode(decl_type, NULL);
            addchild(extra, newnode(type_node->category, NULL)); 
            addchild(extra, curr->node);
            addchild(wrapper, extra);
            curr = curr->next;
        }
    }
    return wrapper;
}
#line 803 "y.tab.c"

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
#line 51 "jucompiler.y"
	{
yyval.node = ast = newnode(Program, NULL);
addchild(yyval.node, newnode(Identifier, yystack.l_mark[-3].lexeme));
unpack_nodes(ast, yystack.l_mark[-1].node);
}
#line 1480 "y.tab.c"
break;
case 2:
#line 57 "jucompiler.y"
	{  
        yyval.node = yystack.l_mark[-1].node;
        if (yystack.l_mark[0].node != NULL) {
            if (yystack.l_mark[0].node->category == MethodBody) { unpack_nodes(yyval.node, yystack.l_mark[0].node); }
            else { addchild(yyval.node, yystack.l_mark[0].node); }
        }
    }
#line 1491 "y.tab.c"
break;
case 3:
#line 64 "jucompiler.y"
	{ 
        yyval.node = newnode(MethodBody, NULL); 
        if (yystack.l_mark[0].node != NULL) {
            if (yystack.l_mark[0].node->category == MethodBody) { unpack_nodes(yyval.node, yystack.l_mark[0].node); }
            else { addchild(yyval.node, yystack.l_mark[0].node); }
        }
    }
#line 1502 "y.tab.c"
break;
case 4:
#line 71 "jucompiler.y"
	{ yyval.node = newnode(MethodBody, NULL); }
#line 1507 "y.tab.c"
break;
case 5:
#line 74 "jucompiler.y"
	{ yyval.node = yystack.l_mark[0].node; }
#line 1512 "y.tab.c"
break;
case 6:
#line 75 "jucompiler.y"
	{ yyval.node = yystack.l_mark[0].node; }
#line 1517 "y.tab.c"
break;
case 7:
#line 76 "jucompiler.y"
	{ yyval.node = NULL; }
#line 1522 "y.tab.c"
break;
case 8:
#line 78 "jucompiler.y"
	{
    yyval.node = newnode(MethodDecl, NULL);
    addchild(yyval.node, yystack.l_mark[-1].node);
    addchild(yyval.node, yystack.l_mark[0].node);
}
#line 1531 "y.tab.c"
break;
case 9:
#line 84 "jucompiler.y"
	{
    yyval.node = create_multiple_decls(FieldDecl, yystack.l_mark[-3].node, yystack.l_mark[-2].lexeme, yystack.l_mark[-1].node);
}
#line 1538 "y.tab.c"
break;
case 10:
#line 87 "jucompiler.y"
	{yyval.node=NULL;}
#line 1543 "y.tab.c"
break;
case 11:
#line 89 "jucompiler.y"
	{
    if (yystack.l_mark[-2].node == NULL) yyval.node = newnode(FieldDecl, NULL);
    else yyval.node = yystack.l_mark[-2].node;
    addchild(yyval.node, newnode(Identifier, yystack.l_mark[0].lexeme));
}
#line 1552 "y.tab.c"
break;
case 12:
#line 94 "jucompiler.y"
	{yyval.node = NULL;}
#line 1557 "y.tab.c"
break;
case 13:
#line 97 "jucompiler.y"
	{ yyval.node = newnode(Bool,NULL); }
#line 1562 "y.tab.c"
break;
case 14:
#line 98 "jucompiler.y"
	{ yyval.node = newnode(Int,NULL); }
#line 1567 "y.tab.c"
break;
case 15:
#line 99 "jucompiler.y"
	{ yyval.node = newnode(Double,NULL); }
#line 1572 "y.tab.c"
break;
case 16:
#line 101 "jucompiler.y"
	{ yyval.node = newnode(StringArray, NULL); }
#line 1577 "y.tab.c"
break;
case 17:
#line 103 "jucompiler.y"
	{ yyval.node = newnode(VoidNode, NULL); }
#line 1582 "y.tab.c"
break;
case 18:
#line 106 "jucompiler.y"
	{
    yyval.node = newnode(MethodHeader, NULL);
    addchild(yyval.node, yystack.l_mark[-4].node);
    addchild(yyval.node, newnode(Identifier, yystack.l_mark[-3].lexeme));
    addchild(yyval.node, yystack.l_mark[-1].node);
    }
#line 1592 "y.tab.c"
break;
case 19:
#line 112 "jucompiler.y"
	{
    yyval.node = newnode(MethodHeader, NULL);
    addchild(yyval.node, newnode(VoidNode, NULL));
    addchild(yyval.node, newnode(Identifier, yystack.l_mark[-3].lexeme));
    addchild(yyval.node, yystack.l_mark[-1].node);
    }
#line 1602 "y.tab.c"
break;
case 20:
#line 119 "jucompiler.y"
	{yyval.node=yystack.l_mark[0].node;}
#line 1607 "y.tab.c"
break;
case 21:
#line 120 "jucompiler.y"
	{yyval.node = newnode(MethodParams, NULL);}
#line 1612 "y.tab.c"
break;
case 22:
#line 122 "jucompiler.y"
	{ 
    yyval.node = yystack.l_mark[-2].node;
    addchild(yyval.node, yystack.l_mark[0].node);
}
#line 1620 "y.tab.c"
break;
case 23:
#line 126 "jucompiler.y"
	{
    yyval.node=newnode(MethodParams, NULL);
    addchild(yyval.node, yystack.l_mark[0].node);}
#line 1627 "y.tab.c"
break;
case 24:
#line 131 "jucompiler.y"
	{
    yyval.node = newnode(ParamDecl,NULL);
    addchild(yyval.node, yystack.l_mark[-1].node);
    addchild(yyval.node, newnode(Identifier, yystack.l_mark[0].lexeme));
}
#line 1636 "y.tab.c"
break;
case 25:
#line 136 "jucompiler.y"
	{
    yyval.node = newnode(ParamDecl,NULL);
    addchild(yyval.node, yystack.l_mark[-1].node);
    addchild(yyval.node, newnode(Identifier, yystack.l_mark[0].lexeme));
            }
#line 1645 "y.tab.c"
break;
case 26:
#line 143 "jucompiler.y"
	{
    yyval.node = yystack.l_mark[-1].node;
}
#line 1652 "y.tab.c"
break;
case 27:
#line 147 "jucompiler.y"
	{
    yyval.node = yystack.l_mark[-1].node;
    if (yystack.l_mark[0].node != NULL) {
        if (yystack.l_mark[0].node->category == MethodBody) { unpack_nodes(yyval.node, yystack.l_mark[0].node); } /* Si es un VarDecl múltiple */
        else { addchild(yyval.node, yystack.l_mark[0].node); }
    }
}
#line 1663 "y.tab.c"
break;
case 28:
#line 154 "jucompiler.y"
	{
    yyval.node = newnode(MethodBody, NULL); 
    if (yystack.l_mark[0].node != NULL) {
        if (yystack.l_mark[0].node->category == MethodBody) { unpack_nodes(yyval.node, yystack.l_mark[0].node); }
        else { addchild(yyval.node, yystack.l_mark[0].node); }
    }
}
#line 1674 "y.tab.c"
break;
case 29:
#line 161 "jucompiler.y"
	{ yyval.node = newnode(MethodBody, NULL); }
#line 1679 "y.tab.c"
break;
case 30:
#line 164 "jucompiler.y"
	{yyval.node = yystack.l_mark[0].node;}
#line 1684 "y.tab.c"
break;
case 31:
#line 165 "jucompiler.y"
	{yyval.node = yystack.l_mark[0].node;}
#line 1689 "y.tab.c"
break;
case 32:
#line 167 "jucompiler.y"
	{
    yyval.node = create_multiple_decls(VarDecl, yystack.l_mark[-3].node, yystack.l_mark[-2].lexeme, yystack.l_mark[-1].node);
}
#line 1696 "y.tab.c"
break;
case 33:
#line 171 "jucompiler.y"
	{
    int count = count_children(yystack.l_mark[-1].node);
        if (count == 1) {
            yyval.node = yystack.l_mark[-1].node->children->node; 
        } else {
            yyval.node = newnode(Block, NULL);
            unpack_nodes(yyval.node, yystack.l_mark[-1].node);
        }
}
#line 1709 "y.tab.c"
break;
case 34:
#line 180 "jucompiler.y"
	{
            yyval.node = newnode(If, NULL);
            addchild(yyval.node, yystack.l_mark[-2].node);
            addchild(yyval.node, yystack.l_mark[0].node);
            addchild(yyval.node, newnode(Block, NULL)); /* else branch vacio*/
         }
#line 1719 "y.tab.c"
break;
case 35:
#line 186 "jucompiler.y"
	{
            yyval.node = newnode(If, NULL);
            addchild(yyval.node, yystack.l_mark[-4].node);
            addchild(yyval.node, yystack.l_mark[-2].node);
            addchild(yyval.node, yystack.l_mark[0].node);
         }
#line 1729 "y.tab.c"
break;
case 36:
#line 192 "jucompiler.y"
	{
    yyval.node = newnode(While, NULL);
    addchild(yyval.node, yystack.l_mark[-2].node);
    addchild(yyval.node, yystack.l_mark[0].node);
         }
#line 1738 "y.tab.c"
break;
case 37:
#line 197 "jucompiler.y"
	{
    yyval.node = newnode(Return, NULL);
         }
#line 1745 "y.tab.c"
break;
case 38:
#line 200 "jucompiler.y"
	{
    yyval.node = newnode(Return, NULL);
    addchild(yyval.node, yystack.l_mark[-1].node);
         }
#line 1753 "y.tab.c"
break;
case 39:
#line 204 "jucompiler.y"
	{yyval.node = yystack.l_mark[-1].node;}
#line 1758 "y.tab.c"
break;
case 40:
#line 205 "jucompiler.y"
	{  yyval.node = yystack.l_mark[-1].node;    }
#line 1763 "y.tab.c"
break;
case 41:
#line 206 "jucompiler.y"
	{yyval.node = yystack.l_mark[-1].node; }
#line 1768 "y.tab.c"
break;
case 42:
#line 207 "jucompiler.y"
	{ yyval.node = NULL; }
#line 1773 "y.tab.c"
break;
case 43:
#line 208 "jucompiler.y"
	{
    yyval.node = newnode(Print, NULL);
    addchild(yyval.node, yystack.l_mark[-2].node);
         }
#line 1781 "y.tab.c"
break;
case 44:
#line 212 "jucompiler.y"
	{
    yyval.node = newnode(Print, NULL);
    addchild(yyval.node, newnode(StrLit, yystack.l_mark[-2].lexeme));
         }
#line 1789 "y.tab.c"
break;
case 45:
#line 216 "jucompiler.y"
	{yyval.node=NULL;}
#line 1794 "y.tab.c"
break;
case 46:
#line 218 "jucompiler.y"
	{
    yyval.node = yystack.l_mark[-1].node;
        if (yystack.l_mark[0].node != NULL) addchild(yyval.node, yystack.l_mark[0].node);
}
#line 1802 "y.tab.c"
break;
case 47:
#line 222 "jucompiler.y"
	{yyval.node = newnode(MethodBody, NULL); }
#line 1807 "y.tab.c"
break;
case 48:
#line 224 "jucompiler.y"
	{
        yyval.node = newnode(Call, NULL);
        addchild(yyval.node, newnode(Identifier, yystack.l_mark[-3].lexeme));
        unpack_nodes(yyval.node, yystack.l_mark[-1].node);
    }
#line 1816 "y.tab.c"
break;
case 49:
#line 229 "jucompiler.y"
	{ yyval.node = NULL; }
#line 1821 "y.tab.c"
break;
case 50:
#line 232 "jucompiler.y"
	{
        yyval.node = newnode(Program, NULL); /*Temporal*/
        addchild(yyval.node, yystack.l_mark[-1].node);
        unpack_nodes(yyval.node, yystack.l_mark[0].node);
    }
#line 1830 "y.tab.c"
break;
case 51:
#line 237 "jucompiler.y"
	{ yyval.node = NULL; }
#line 1835 "y.tab.c"
break;
case 52:
#line 240 "jucompiler.y"
	{
        if (yystack.l_mark[-2].node == NULL) yyval.node = newnode(Program, NULL);
        else yyval.node = yystack.l_mark[-2].node;
        addchild(yyval.node, yystack.l_mark[0].node);
    }
#line 1844 "y.tab.c"
break;
case 53:
#line 245 "jucompiler.y"
	{ yyval.node = NULL; }
#line 1849 "y.tab.c"
break;
case 54:
#line 247 "jucompiler.y"
	{
    yyval.node = newnode(Assign, NULL);
    addchild(yyval.node, newnode(Identifier, yystack.l_mark[-2].lexeme));
    addchild(yyval.node, yystack.l_mark[0].node);
}
#line 1858 "y.tab.c"
break;
case 55:
#line 253 "jucompiler.y"
	{
    yyval.node = newnode(ParseArgs, NULL);
    addchild(yyval.node, newnode(Identifier, yystack.l_mark[-4].lexeme));
     addchild(yyval.node, yystack.l_mark[-2].node);
}
#line 1867 "y.tab.c"
break;
case 56:
#line 258 "jucompiler.y"
	{yyval.node = NULL;}
#line 1872 "y.tab.c"
break;
case 57:
#line 262 "jucompiler.y"
	{ yyval.node = newnode(Add, NULL); addchild(yyval.node, yystack.l_mark[-2].node); addchild(yyval.node, yystack.l_mark[0].node); }
#line 1877 "y.tab.c"
break;
case 58:
#line 263 "jucompiler.y"
	{ yyval.node = newnode(Sub, NULL); addchild(yyval.node, yystack.l_mark[-2].node); addchild(yyval.node, yystack.l_mark[0].node); }
#line 1882 "y.tab.c"
break;
case 59:
#line 264 "jucompiler.y"
	{ yyval.node = newnode(Mul, NULL); addchild(yyval.node, yystack.l_mark[-2].node); addchild(yyval.node, yystack.l_mark[0].node); }
#line 1887 "y.tab.c"
break;
case 60:
#line 265 "jucompiler.y"
	{ yyval.node = newnode(Div, NULL); addchild(yyval.node, yystack.l_mark[-2].node); addchild(yyval.node, yystack.l_mark[0].node); }
#line 1892 "y.tab.c"
break;
case 61:
#line 266 "jucompiler.y"
	{ yyval.node = newnode(Mod, NULL); addchild(yyval.node, yystack.l_mark[-2].node); addchild(yyval.node, yystack.l_mark[0].node); }
#line 1897 "y.tab.c"
break;
case 62:
#line 267 "jucompiler.y"
	{ yyval.node = newnode(And, NULL); addchild(yyval.node, yystack.l_mark[-2].node); addchild(yyval.node, yystack.l_mark[0].node); }
#line 1902 "y.tab.c"
break;
case 63:
#line 268 "jucompiler.y"
	{ yyval.node = newnode(Or, NULL); addchild(yyval.node, yystack.l_mark[-2].node); addchild(yyval.node, yystack.l_mark[0].node); }
#line 1907 "y.tab.c"
break;
case 64:
#line 269 "jucompiler.y"
	{ yyval.node = newnode(Xor, NULL); addchild(yyval.node, yystack.l_mark[-2].node); addchild(yyval.node, yystack.l_mark[0].node); }
#line 1912 "y.tab.c"
break;
case 65:
#line 270 "jucompiler.y"
	{ yyval.node = newnode(Lshift, NULL); addchild(yyval.node, yystack.l_mark[-2].node); addchild(yyval.node, yystack.l_mark[0].node); }
#line 1917 "y.tab.c"
break;
case 66:
#line 271 "jucompiler.y"
	{ yyval.node = newnode(Rshift, NULL); addchild(yyval.node, yystack.l_mark[-2].node); addchild(yyval.node, yystack.l_mark[0].node); }
#line 1922 "y.tab.c"
break;
case 67:
#line 272 "jucompiler.y"
	{ yyval.node = newnode(Eq, NULL); addchild(yyval.node, yystack.l_mark[-2].node); addchild(yyval.node, yystack.l_mark[0].node); }
#line 1927 "y.tab.c"
break;
case 68:
#line 273 "jucompiler.y"
	{ yyval.node = newnode(Ne, NULL); addchild(yyval.node, yystack.l_mark[-2].node); addchild(yyval.node, yystack.l_mark[0].node); }
#line 1932 "y.tab.c"
break;
case 69:
#line 274 "jucompiler.y"
	{ yyval.node = newnode(Gt, NULL); addchild(yyval.node, yystack.l_mark[-2].node); addchild(yyval.node, yystack.l_mark[0].node); }
#line 1937 "y.tab.c"
break;
case 70:
#line 275 "jucompiler.y"
	{ yyval.node = newnode(Ge, NULL); addchild(yyval.node, yystack.l_mark[-2].node); addchild(yyval.node, yystack.l_mark[0].node); }
#line 1942 "y.tab.c"
break;
case 71:
#line 276 "jucompiler.y"
	{ yyval.node = newnode(Lt, NULL); addchild(yyval.node, yystack.l_mark[-2].node); addchild(yyval.node, yystack.l_mark[0].node); }
#line 1947 "y.tab.c"
break;
case 72:
#line 277 "jucompiler.y"
	{ yyval.node = newnode(Le, NULL); addchild(yyval.node, yystack.l_mark[-2].node); addchild(yyval.node, yystack.l_mark[0].node); }
#line 1952 "y.tab.c"
break;
case 73:
#line 278 "jucompiler.y"
	{ yyval.node = newnode(Minus, NULL); addchild(yyval.node, yystack.l_mark[0].node); }
#line 1957 "y.tab.c"
break;
case 74:
#line 279 "jucompiler.y"
	{ yyval.node = newnode(Plus, NULL); addchild(yyval.node, yystack.l_mark[0].node); }
#line 1962 "y.tab.c"
break;
case 75:
#line 280 "jucompiler.y"
	{ yyval.node = newnode(Not, NULL); addchild(yyval.node, yystack.l_mark[0].node); }
#line 1967 "y.tab.c"
break;
case 76:
#line 281 "jucompiler.y"
	{yyval.node = yystack.l_mark[-1].node;}
#line 1972 "y.tab.c"
break;
case 77:
#line 282 "jucompiler.y"
	{yyval.node = NULL;}
#line 1977 "y.tab.c"
break;
case 78:
#line 283 "jucompiler.y"
	{yyval.node=yystack.l_mark[0].node;}
#line 1982 "y.tab.c"
break;
case 79:
#line 284 "jucompiler.y"
	{yyval.node=yystack.l_mark[0].node;}
#line 1987 "y.tab.c"
break;
case 80:
#line 285 "jucompiler.y"
	{yyval.node=yystack.l_mark[0].node;}
#line 1992 "y.tab.c"
break;
case 81:
#line 286 "jucompiler.y"
	{yyval.node=newnode(Identifier,yystack.l_mark[0].lexeme);}
#line 1997 "y.tab.c"
break;
case 82:
#line 287 "jucompiler.y"
	{
        yyval.node = newnode(Length, NULL);
        addchild(yyval.node, newnode(Identifier, yystack.l_mark[-1].lexeme));}
#line 2004 "y.tab.c"
break;
case 83:
#line 290 "jucompiler.y"
	{yyval.node=newnode(Natural,yystack.l_mark[0].lexeme); }
#line 2009 "y.tab.c"
break;
case 84:
#line 291 "jucompiler.y"
	{yyval.node=newnode(Decimal,yystack.l_mark[0].lexeme);}
#line 2014 "y.tab.c"
break;
case 85:
#line 292 "jucompiler.y"
	{yyval.node=newnode(BoolLit,yystack.l_mark[0].lexeme);}
#line 2019 "y.tab.c"
break;
case 86:
#line 293 "jucompiler.y"
	{yyval.node=newnode(StrLit,yystack.l_mark[0].lexeme); }
#line 2024 "y.tab.c"
break;
#line 2026 "y.tab.c"
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
