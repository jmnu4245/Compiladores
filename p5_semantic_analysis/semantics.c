#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "semantics.h"

int semantic_errors = 0;

struct symbol_list *symbol_table;
void check_parameters(struct node *parameters,struct symbol_list *scope) {
    int n = countchildren(parameters);
    for(int i=0;i<n;i++){
        struct node *parameter = getchild(parameters,i);
        struct node *identifier = getchild(parameter,1);
        struct node *type_node = getchild(parameter, 0);

        enum type param_type = no_type;
        if (type_node->category == Integer) {
            param_type = integer_type;
        } else if (type_node->category == Double) {
            param_type = double_type;
        }
        
        if(search_symbol(scope,identifier->token) == NULL) {
            insert_symbol(scope, identifier->token, param_type, parameter);
        } else {
            printf("Parameter %s already declared at line %d, column %d\n", identifier->token, identifier->line, identifier->column);
            semantic_errors++;
        }
    }    
}

void check_expression(struct node *expression, struct symbol_list *scope) {
    if (expression == NULL) return;

    int num_children = countchildren(expression);
    for (int i = 0; i < num_children; i++) {
        check_expression(getchild(expression, i),scope);
    }

    if (expression->category == Natural) {
        expression->type = integer_type;
    } 
    else if (expression->category == Decimal) {
        expression->type = double_type;
    } 
    else if (expression->category == Identifier) {
        struct symbol_list *sym = search_symbol(scope, expression->token);
        if (sym != NULL) {
            expression->type = sym->type;
        } else {
            printf("Identifier %s not declared at line %d, column %d\n", expression->token, expression->line, expression->column);
            semantic_errors++;
            expression->type = no_type;
        }
    }
    else if (expression->category == If) {
        struct node *then_branch = getchild(expression, 1);
        struct node *else_branch = getchild(expression, 2);
        
        if (then_branch->type == else_branch->type) {
            expression->type = then_branch->type;
        } 
        else if (then_branch->type == no_type) {
            expression->type = else_branch->type;
        } 
        else if (else_branch->type == no_type) {
            expression->type = then_branch->type;
        } 
        else {
            printf("Incompatible types in If statement at line %d, column %d\n", expression->line, expression->column);
            semantic_errors++;
            expression->type = no_type;
        }
    }
    else if (expression->category == Add || expression->category == Sub || 
             expression->category == Mul || expression->category == Div) {
        
        struct node *op1 = getchild(expression, 0);
        struct node *op2 = getchild(expression, 1);
        
        if (op1->type == op2->type) {
            expression->type = op1->type;
        } 
        else if (op1->type == no_type) {
            expression->type = op2->type;
        } 
        else if (op2->type == no_type) {
            expression->type = op1->type;
        } 
        else {
            printf("Incompatible types in operation at line %d, column %d\n", expression->line, expression->column);
            semantic_errors++;
            expression->type = no_type;
        }
    }
    else if (expression->category == Call) {
        struct node *id = getchild(expression, 0);
        struct symbol_list *funcdecllist = search_symbol(symbol_table, id->token);
        
        if (funcdecllist != NULL) {
            struct node *arguments = getchild(expression, 1);
            struct node *funcdecl = funcdecllist->node;
            struct node *parameters = getchild(funcdecl, 1);
            
            int narguments = countchildren(arguments);
            int nparameters = countchildren(parameters);
            
            if (narguments != nparameters) { 
                printf("Parameters of %s dont match at line %d, column %d\n", id->token, expression->line, expression->column);
                semantic_errors++;
            }
            expression->type = funcdecllist->type; 
        }
    }
}

void check_function(struct node *function) {
    struct node *id = getchild(function, 0);
    struct symbol_list *sym = search_symbol(symbol_table, id->token);
    struct symbol_list *scope = (struct symbol_list *) malloc(sizeof(struct symbol_list));;
    scope->next = NULL;
    if(sym == NULL) {
        sym = insert_symbol(symbol_table, id->token, no_type, function);
    } else {
        printf("Identifier %s already declared at line %d, column %d\n", id->token, id->line, id->column);
        semantic_errors++;
    }
    insert_symbol(scope,id->token,no_type,function);
    check_parameters(getchild(function, 1),scope);
    struct node *expr = getchild(function, 2);
    check_expression(expr,scope);

    if (sym != NULL) {
        sym->type = expr->type;
    }
}

int check_program(struct node *program) {
    symbol_table = (struct symbol_list *) malloc(sizeof(struct symbol_list));
    symbol_table->next = NULL;
    struct node_list *child = program->children;
    while((child = child->next) != NULL)
        check_function(child->node);
    return semantic_errors;
}

// insert a new symbol in the list, unless it is already there
struct symbol_list *insert_symbol(struct symbol_list *table, char *identifier, enum type type, struct node *node) {
    if(search_symbol(table, identifier) != NULL)
        return NULL;       /* return NULL if symbol is already inserted */
    struct symbol_list *new = (struct symbol_list *) malloc(sizeof(struct symbol_list));
    new->identifier = strdup(identifier);
    new->type = type;
    new->node = node;
    new->next = NULL;
    struct symbol_list *symbol = table;
    while(symbol->next != NULL)
        symbol = symbol->next;
    symbol->next = new;    /* insert new symbol at the tail of the list */
    return new;
}

// look up a symbol by its identifier
struct symbol_list *search_symbol(struct symbol_list *table, char *identifier) {
    struct symbol_list *symbol;
    for(symbol = table->next; symbol != NULL; symbol = symbol->next)
        if(strcmp(symbol->identifier, identifier) == 0)
            return symbol;
    return NULL;
}

void show_symbol_tables() {
    struct symbol_list *symbol;
    for(symbol = symbol_table->next; symbol != NULL; symbol = symbol->next)
        printf("Symbol %s : %s\n", symbol->identifier, type_name(symbol->type));
}
