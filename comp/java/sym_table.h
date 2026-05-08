#ifndef SYM_TABLE_H
#define SYM_TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { T_Int, T_Bool, T_Double, T_StringArray, T_String, T_Void, T_Undef, T_None } BasicType;

typedef enum {
    SYM_FIELD,    // variable global
    SYM_METHOD,   // método
    SYM_PARAM,    // parámetro formal
    SYM_LOCAL,    // variable local
    SYM_RETURN    // pseudo-símbolo "return"
} SymbolKind;

typedef struct ParamType {
    BasicType        type;
    struct ParamType *next;
} ParamType;

typedef struct symbol {
    SymbolKind  kind;
    char *name;
    BasicType type;
    ParamType *params;
    struct symbol *next;
    struct sym_table *nested_table;
} Symbol;

typedef struct sym_table {
    char *title;
    Symbol *first;
} SymTable;

struct node;

SymTable* create_table(const char *title);
void insert_symbol(SymTable*, char*, BasicType, SymbolKind kind, ParamType*);
Symbol* lookup_symbol(SymTable *global, SymTable *local, const char *name);
Symbol* search_symbol(SymTable *table, const char *name);
SymTable* search_table(SymTable *global, const char *title);
SymTable* search_table_name(SymTable *global, const char *name);
Symbol* search_exact_method(SymTable *table, const char *name, ParamType *params);
const char* type_to_str(BasicType type);
void print_sym_table(SymTable *table);
void print_all_tables(SymTable *global);
int params_equal(ParamType *a, ParamType *b);
char* params_to_str(ParamType *p);
ParamType *build_params_list(struct node *params_node);

#endif