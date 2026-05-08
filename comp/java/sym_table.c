#include "sym_table.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char* type_to_str(BasicType type) {
    switch(type) {
        case T_Int: return "int";
        case T_Bool: return "boolean";
        case T_Double: return "double";
        case T_StringArray: return "String[]";
        case T_String: return "String";
        case T_Void: return "void";
        default: return "undef";
    }
}

char* params_to_str(ParamType *p) {
    int len = 3;
    ParamType *curr = p;
    while (curr) {
        len += strlen(type_to_str(curr->type));
        if (curr->next) len += 1; // Para la coma
        curr = curr->next;
    }
    char *buf = (char*)malloc(len);
    if (!buf) return NULL;
    int offset = sprintf(buf, "(");
    curr = p;
    int first = 1;
    while (curr) {
        if (!first) offset += sprintf(buf + offset, ",");
        offset += sprintf(buf + offset, "%s", type_to_str(curr->type));
        first = 0;
        curr = curr->next;
    }
    sprintf(buf + offset, ")");
    
    return buf;
}

int params_equal(ParamType *a, ParamType *b) {
    for (; a && b; a = a->next, b = b->next) {
        if (a->type != b->type) return 0;
    }
    return a == NULL && b == NULL;
}

SymTable* create_table(const char *title) {
    SymTable *st = (SymTable*)malloc(sizeof(SymTable));
    if (st == NULL) return NULL;
    st->title = strdup(title);
    st->first = NULL;
    return st;
}

Symbol* search_symbol(SymTable *table, const char *name) {
    Symbol *curr = table->first;
    while (curr) {
        if (strcmp(curr->name, name) == 0) return curr;
        curr = curr->next;
    }
    return NULL;
}

Symbol* search_exact_method(SymTable *table, const char *name, ParamType *params) {
    Symbol *curr = table->first;
    while (curr) {
        if (strcmp(curr->name, name) == 0 && curr->kind == SYM_METHOD) {
            if (params_equal(curr->params, params)) {
                return curr;
            }
        }
        curr = curr->next;
    }
    return NULL;
}

Symbol* lookup_symbol(SymTable *global, SymTable *local, const char *name) {
    Symbol *sym = NULL;
    if (local != NULL) sym = search_symbol(local, name);
    if (sym == NULL && global != NULL) sym = search_symbol(global, name);
    return sym;
}

void insert_symbol(SymTable *table, char *name, BasicType type,SymbolKind kind, ParamType *params) {
    Symbol *new_sym = (Symbol*)malloc(sizeof(Symbol));
    new_sym->name = name;
    new_sym->type = type;
    new_sym->kind = kind;
    new_sym->params = params;
    new_sym->next = NULL;
    new_sym->nested_table = NULL;

    if (table->first == NULL) {
        table->first = new_sym;
    } else {
        Symbol *curr = table->first;
        while (curr->next) curr = curr->next;
        curr->next = new_sym;
    }
}


void print_sym_table(SymTable *table) {
    if (!table) return;

    printf("===== %s Symbol Table =====\n", table->title);
    Symbol *curr = table->first;
    while (curr) {
        printf("%s\t", curr->name);
        
        // Formateo especial dependiendo de si es un método o una variable
        if (curr->kind == SYM_METHOD) {
            if (curr->params != NULL) {
                char *parambuf = params_to_str(curr->params);
                printf("%s\t", parambuf);
            } else {
                // Si es un método pero no tiene parámetros, imprime ()
                printf("()\t");
            }
            printf("%s", type_to_str(curr->type));
        } else {
            // Si es una variable, solo ponemos tabulador para alinear el tipo
            printf("\t%s", type_to_str(curr->type));
        }
        
        if (curr->kind == SYM_PARAM) printf("\tparam");
        
        printf("\n");
        curr = curr->next;
    }
    printf("\n");
}

void print_all_tables(SymTable *global) {
    print_sym_table(global);
    Symbol *curr = global->first;
    while (curr) {
        if (curr->nested_table) {
            print_sym_table(curr->nested_table);
        }
        curr = curr->next;
    }
}