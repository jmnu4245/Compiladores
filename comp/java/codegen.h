#ifndef CODEGEN_H
#define CODEGEN_H

#include "ast.h"
#include "sym_table.h"

void codegen_program(struct node *program, SymTable *global_table);

#endif