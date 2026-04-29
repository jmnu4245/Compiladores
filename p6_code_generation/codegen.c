#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "semantics.h"
#include "codegen.h"

int temporary;   // sequence of temporary registers in a function

extern struct symbol_list *symbol_table;

int codegen_natural(struct node *natural) {
    printf("  %%%d = add i32 %s, 0\n", temporary, natural->token);
    return temporary++;
}

int codegen_identifer(struct node *identifier){
    printf("  %%%d = add i32 %%%s, 0\n", temporary, identifier->token);
    return temporary++;
}
int codegen_multiplication(struct node *multiplication){
    int t1 = codegen_expression(getchild(multiplication,0));
    int t2 = codegen_expression(getchild(multiplication,1));
    printf("  %%%d = mul i32 %%%d, %%%d\n", temporary, t1, t2);
    return temporary++;
}

int codegen_addition(struct node * addition){
    int t1 = codegen_expression(getchild(addition,0));
    int t2 = codegen_expression(getchild(addition,1));
    printf("  %%%d = add i32 %%%d, %%%d\n", temporary, t1, t2);
    return temporary++;
}

int codegen_substraction(struct node * substraction){
    int t1 = codegen_expression(getchild(substraction,0));
    int t2 = codegen_expression(getchild(substraction,1));
    printf("  %%%d = sub i32 %%%d, %%%d\n", temporary, t1, t2);
    return temporary++;
}
int codegen_division(struct node * division){
    int t1 = codegen_expression(getchild(division,0));
    int t2 = codegen_expression(getchild(division,1));
    printf("  %%%d = sdiv i32 %%%d, %%%d\n", temporary, t1, t2);
    return temporary++;
}

int codegen_call(struct node *call){
    struct node *identifier = getchild(call,0);
    struct node *arguments = getchild(call,1);
    int n = countchildren(arguments);
    int arg_temps[n];
    for (int i = 0; i < n; i++) {
        arg_temps[i] = codegen_expression(getchild(arguments, i));
    }
    printf("  %%%d = call i32 @_%s(", temporary, identifier->token);
    for(int i=0;i<n;i++){
        if (i>0) printf(", ");
        printf("i32 %%%d",arg_temps[i]);
    }
    printf(")\n");
    return temporary++;
}

int codegen_if(struct node *ifelse) {
    if (ifelse == NULL) {
        fprintf(stderr, "Error: Puntero ifelse es NULL\n");
        return -1;
    }

    struct node *cond_node = getchild(ifelse, 0);
    struct node *then_node = getchild(ifelse, 1);
    struct node *else_node = getchild(ifelse, 2);

    //if (cond_node == NULL || then_node == NULL || else_node == NULL) {
    //    fprintf(stderr, "Error (Segfault evitado): El nodo If no tiene 3 hijos en memoria.\n");
    //    return -1;
    //}

    static int if_counter = 1;
    int id = if_counter++;

    int result_ptr = temporary++;
    printf("  %%%d = alloca i32\n", result_ptr);
    
    int cond = codegen_expression(cond_node);

    int cmp = temporary++;
    printf("  %%%d = icmp ne i32 %%%d, 0\n", cmp, cond);
    printf("  br i1 %%%d, label %%L%dthen, label %%L%delse\n\n", cmp, id, id);

    printf("L%dthen:\n", id);
    int then_val = codegen_expression(then_node);
    printf("  store i32 %%%d, i32* %%%d\n", then_val, result_ptr);
    printf("  br label %%L%dend\n\n", id);

    printf("L%delse:\n", id);
    int else_val = codegen_expression(else_node);
    printf("  store i32 %%%d, i32* %%%d\n", else_val, result_ptr);
    printf("  br label %%L%dend\n\n", id);

    printf("L%dend:\n", id);
    int final_val = temporary++;
    printf("  %%%d = load i32, i32* %%%d\n", final_val, result_ptr);

    return final_val;
}
int codegen_expression(struct node *expression) {
    int tmp = -1;
    switch(expression->category) {
        case Natural:
            tmp = codegen_natural(expression);
            break;
        case Identifier:
            tmp = codegen_identifer(expression);
            break;
            
        case Mul:
            tmp = codegen_multiplication(expression);
            break;

        case Add:
        tmp = codegen_addition(expression);
            break;

        case Sub:
            tmp = codegen_substraction(expression);
            break;
        case Div:
            tmp = codegen_division(expression);
            break;
        case Call:
            tmp = codegen_call(expression);
            break;
        case If:
            tmp = codegen_if(expression);
            break;
        default:
            break;
    }
    return tmp;
}

void codegen_parameters(struct node *parameters) {
    struct node *parameter;
    int curr = 0;
    while((parameter = getchild(parameters, curr++)) != NULL) {
        if(curr > 1)
            printf(", ");
        printf("i32 %%%s", getchild(parameter, 1)->token);
    }
}

void codegen_function(struct node *function) {
    temporary = 1;
    printf("define i32 @_%s(", getchild(function, 0)->token);
    codegen_parameters(getchild(function, 1));
    printf(") {\n");
    int tmp = codegen_expression(getchild(function, 2));
    printf("  ret i32 %%%d\n", tmp);
    printf("}\n\n");
}

// code generation begins here, with the AST root node
void codegen_program(struct node *program) {
    // predeclared I/O functions
    printf("declare i32 @_read(i32)\n");
    printf("declare i32 @_write(i32)\n\n");

    // generate code for each function
    struct node_list *function = program->children;
    while((function = function->next) != NULL)
        codegen_function(function->node);

    // generate the entry point which calls main(integer) if it exists
    struct symbol_list *entry = search_symbol(symbol_table, "main");
    if(entry != NULL && entry->node->category == Function)
        printf("define i32 @main() {\n"
               "  %%1 = call i32 @_main(i32 0)\n"
               "  ret i32 %%1\n"
               "}\n");
}
