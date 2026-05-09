/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    CLASS = 258,                   /* CLASS  */
    PUBLIC = 259,                  /* PUBLIC  */
    STATIC = 260,                  /* STATIC  */
    LBRACE = 261,                  /* LBRACE  */
    RBRACE = 262,                  /* RBRACE  */
    LPAR = 263,                    /* LPAR  */
    RPAR = 264,                    /* RPAR  */
    LSQ = 265,                     /* LSQ  */
    RSQ = 266,                     /* RSQ  */
    SEMICOLON = 267,               /* SEMICOLON  */
    COMMA = 268,                   /* COMMA  */
    BOOL = 269,                    /* BOOL  */
    INT = 270,                     /* INT  */
    DOUBLE = 271,                  /* DOUBLE  */
    VOID = 272,                    /* VOID  */
    STRING = 273,                  /* STRING  */
    IF = 274,                      /* IF  */
    WHILE = 275,                   /* WHILE  */
    RETURN = 276,                  /* RETURN  */
    PRINT = 277,                   /* PRINT  */
    PARSEINT = 278,                /* PARSEINT  */
    ASSIGN = 279,                  /* ASSIGN  */
    PLUS = 280,                    /* PLUS  */
    MINUS = 281,                   /* MINUS  */
    STAR = 282,                    /* STAR  */
    DIV = 283,                     /* DIV  */
    MOD = 284,                     /* MOD  */
    AND = 285,                     /* AND  */
    OR = 286,                      /* OR  */
    XOR = 287,                     /* XOR  */
    LSHIFT = 288,                  /* LSHIFT  */
    RSHIFT = 289,                  /* RSHIFT  */
    EQ = 290,                      /* EQ  */
    GE = 291,                      /* GE  */
    GT = 292,                      /* GT  */
    LE = 293,                      /* LE  */
    LT = 294,                      /* LT  */
    NE = 295,                      /* NE  */
    NOT = 296,                     /* NOT  */
    DOTLENGTH = 297,               /* DOTLENGTH  */
    ARROW = 298,                   /* ARROW  */
    IDENTIFIER = 299,              /* IDENTIFIER  */
    NATURAL = 300,                 /* NATURAL  */
    DECIMAL = 301,                 /* DECIMAL  */
    BOOLLIT = 302,                 /* BOOLLIT  */
    STRLIT = 303,                  /* STRLIT  */
    RESERVED = 304,                /* RESERVED  */
    UNARY = 305,                   /* UNARY  */
    IF_PREC = 306,                 /* IF_PREC  */
    ELSE = 307                     /* ELSE  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define CLASS 258
#define PUBLIC 259
#define STATIC 260
#define LBRACE 261
#define RBRACE 262
#define LPAR 263
#define RPAR 264
#define LSQ 265
#define RSQ 266
#define SEMICOLON 267
#define COMMA 268
#define BOOL 269
#define INT 270
#define DOUBLE 271
#define VOID 272
#define STRING 273
#define IF 274
#define WHILE 275
#define RETURN 276
#define PRINT 277
#define PARSEINT 278
#define ASSIGN 279
#define PLUS 280
#define MINUS 281
#define STAR 282
#define DIV 283
#define MOD 284
#define AND 285
#define OR 286
#define XOR 287
#define LSHIFT 288
#define RSHIFT 289
#define EQ 290
#define GE 291
#define GT 292
#define LE 293
#define LT 294
#define NE 295
#define NOT 296
#define DOTLENGTH 297
#define ARROW 298
#define IDENTIFIER 299
#define NATURAL 300
#define DECIMAL 301
#define BOOLLIT 302
#define STRLIT 303
#define RESERVED 304
#define UNARY 305
#define IF_PREC 306
#define ELSE 307

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 23 "jucompiler.y"

    char *lexeme;
    struct node *node;

#line 176 "y.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


extern YYSTYPE yylval;
extern YYLTYPE yylloc;

int yyparse (void);


#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
