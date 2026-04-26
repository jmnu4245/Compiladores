#ifndef _SEMANTICS_H
#define _SEMANTICS_H

#include "ast.h"

int check_program(struct node *program);

struct symbol_list {
	char *identifier;
	enum type type;
	int is_param;
    char *param_types;
	int line,col;
	struct node *node;
	struct symbol_list *next;
};

struct table_list{
    char *identifier;
    struct symbol_list *node;
    struct table_list *next;
};

struct symbol_list *insert_symbol(struct symbol_list *symbol_table, char *identifier, enum type type,int is_param, int line, int col, struct node *node);
struct symbol_list *search_symbol(struct symbol_list *symbol_table, char *identifier);
void print_symbol_table();
void print_symbol_tables();

extern int semantic_errors;
extern struct symbol_list *global_table;

#endif
