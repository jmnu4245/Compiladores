#!/bin/sh
rm -f jucompiler lex.yy.c y.tab.c y.tab.h
yacc -d -v -t -g jucompiler.y
lex jucompiler.l
gcc -g lex.yy.c y.tab.c ast.c sym_table.c semantics.c  -o jucompiler -Wall -Wno-unused-function