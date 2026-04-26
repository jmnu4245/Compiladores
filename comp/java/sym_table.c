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
        case T_Void: return "void";
        default: return "undef";
    }
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

Symbol* lookup_symbol(SymTable *global, SymTable *local, const char *name) {
    Symbol *sym = NULL;
    if (local != NULL) sym = search_symbol(local, name);
    if (sym == NULL && global != NULL) sym = search_symbol(global, name);
    return sym;
}

void insert_symbol(SymTable *table, const char *name, BasicType type, int is_param, char *params_list, int line, int col) {

    if (!table || !name) return;

    if (search_symbol(table, name)) {
        printf("Line %d, col %d: Symbol %s already defined\n", line, col, name);
        return;
    }

    Symbol *new_sym = (Symbol*)malloc(sizeof(Symbol));
    new_sym->name = strdup(name);
    new_sym->type = type;
    new_sym->is_param = is_param;
    new_sym->params_list = params_list ? strdup(params_list) : NULL;
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
        

        if (curr->params_list != NULL) {
            printf("%s\t", curr->params_list);
        }
        
        printf("%s", type_to_str(curr->type));
        

        if (curr->is_param) {
            printf("\tparam");
        }
        
        printf("\n");
        curr = curr->next;
    }
    printf("\n");
}