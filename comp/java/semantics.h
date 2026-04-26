#ifndef _SEMANTICS_H
#define _SEMANTICS_H

#include "ast.h"
#include "sym_table.h"

extern int semantic_errors;
extern SymTable *global_table;
extern SymTable *current_table;

void check_semantics(struct node *n);
void check_expression(struct node *n, SymTable *global, SymTable *local);

#endif