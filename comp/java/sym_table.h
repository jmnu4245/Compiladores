#ifndef SYM_TABLE_H
#define SYM_TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { 
    T_Int, T_Bool, T_Double, T_StringArray, T_Void, T_Undef, T_None 
} BasicType;

typedef struct symbol {
    char *name;
    BasicType type;
    int is_param;
    char *params_list;
    struct symbol *next;
} Symbol;

typedef struct sym_table {
    char *title;
    Symbol *first;
    struct sym_table *next;
} SymTable;


SymTable* create_table(const char *title);
void insert_symbol(SymTable *table, const char *name, BasicType type, int is_param, char *params_list, int line, int col);
Symbol* lookup_symbol(SymTable *global, SymTable *local, const char *name);
Symbol* search_symbol(SymTable *table, const char *name);
const char* type_to_str(BasicType type);
void print_sym_table(SymTable *table);

#endif