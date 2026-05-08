#ifndef _SEMANTICS_H
#define _SEMANTICS_H

#include "ast.h"
#include "sym_table.h"

extern int semantic_errors;
extern SymTable *global_table;
extern SymTable *current_table;

void check_semantics(struct node *n);
void check_expression(struct node *n, SymTable *global, SymTable *local);
BasicType type_from_node(struct node *n);
void build_params_str(struct node *params, char *buf, int bufsz);
#endif