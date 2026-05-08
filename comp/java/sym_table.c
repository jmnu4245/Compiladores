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


void params_to_str(ParamType *p, char *buf, int bufsz) {
    strncpy(buf, "(", bufsz);
    int first = 1;
    for (; p; p = p->next) {
        if (!first) strncat(buf, ",", bufsz - strlen(buf) - 1);
        strncat(buf, type_to_str(p->type), bufsz - strlen(buf) - 1);
        first = 0;
    }
    strncat(buf, ")", bufsz - strlen(buf) - 1);
}

SymTable* create_table(const char *title) {
    SymTable *st = (SymTable*)malloc(sizeof(SymTable));
    if (st == NULL) return NULL;
    st->title = strdup(title);
    st->first = NULL;
    st->next = NULL;
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


SymTable* search_table(SymTable *global, const char *title) {
    SymTable *curr = global;
    while (curr != NULL) {
        if (curr->title != NULL && strcmp(curr->title, title) == 0) {
            return curr;
        }
        curr = curr->next;
        
    }
    return NULL;
}

SymTable* search_table_name(SymTable *global, const char *name) {
    char prefix[256];
    snprintf(prefix, sizeof(prefix), "Method %s(", name);
    size_t len = strlen(prefix);

    SymTable *curr = global;
    while (curr != NULL) {
        // Compara solo el inicio del título ("Method nombre(")
        if (curr->title != NULL && strncmp(curr->title, prefix, len) == 0) {
            return curr;
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

void insert_symbol(SymTable *table, const char *name, BasicType type,SymbolKind kind, ParamType *params, int line, int col) {

    if (!table || !name) return;

    Symbol *new_sym = (Symbol*)malloc(sizeof(Symbol));
    new_sym->name = strdup(name);
    new_sym->type = type;
    new_sym->kind = kind;
    new_sym->params = params;
    new_sym->next = NULL;


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
        

        if (curr->params != NULL) {
            char parambuf[256];
            params_to_str(curr->params, parambuf, sizeof(parambuf));
            printf("%s\t", parambuf);
        } else printf("\t");
        
        printf("%s", type_to_str(curr->type));
        

        if (curr->kind == SYM_PARAM) printf("\tparam");
        
        printf("\n");
        curr = curr->next;
    }
    printf("\n");
}