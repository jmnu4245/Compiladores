#include "ast.h"
#include "sym_table.h"
#include "y.tab.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

SymTable *global_table = NULL;
SymTable *current_table = NULL;


BasicType get_type_from_node(struct node *n) {
    if (n == NULL) return T_Undef;
    if (n->category == Int) return T_Int;
    if (n->category == Double) return T_Double;
    if (n->category == Bool) return T_Bool;
    if (n->category == VoidNode) return T_Void;
    if (n->category == StringArray) return T_StringArray;
    return T_Undef;
}


void check_semantics(struct node *n) {
    if (n == NULL) return;

    switch (n->category) {
        case Program: {

            if (n->children && n->children->node) {
                struct node *class_id = n->children->node;
                char title[256];

                if (class_id->token) {
                    sprintf(title, "Class %s", class_id->token);
                } else {
                    sprintf(title, "Class Unknown");
                }

                global_table = create_table(title);
                current_table = global_table;

                struct node_list *curr = n->children;
                while (curr) {
                    check_semantics(curr->node);
                    curr = curr->next;
                }
            }
            break;
        }

        case FieldDecl: {

            struct node *type_node = n->children->node;
            struct node *id_node = n->children->next->node;
            insert_symbol(global_table, id_node->token, get_type_from_node(type_node), 0, NULL, id_node->line, id_node->col);
            break;
        }

        case MethodDecl: {

            if (!n->children || !n->children->next) return;

            struct node *header = n->children->node;
            struct node *body = n->children->next->node;

            if (!header || !header->children || !header->children->next || !header->children->next->next) return;
            
            struct node *ret_type = header->children->node;
            struct node *method_id = header->children->next->node;
            struct node *params_node = header->children->next->next->node;

            char params_str[256] = "(";

            int first = 1;

            if (params_node && params_node->children) {
                struct node_list *curr_param_list = params_node->children;
                while (curr_param_list != NULL) {
                    if (curr_param_list->node != NULL && curr_param_list->node->children != NULL) {
                        if (!first) strcat(params_str, ",");

                        BasicType p_type = get_type_from_node(curr_param_list->node->children->node);
                        const char *p_type_name = type_to_str(p_type);
                        if (p_type_name) strcat(params_str, p_type_name);

                        first = 0;
                    }
                    curr_param_list = curr_param_list->next;
                }
            }
            strcat(params_str, ")");

            insert_symbol(global_table, method_id->token, get_type_from_node(ret_type), 0, params_str, method_id->line, method_id->col);

            char method_title[512];
            sprintf(method_title, "Method %s %s", method_id->token, params_str);
            SymTable *method_table = create_table(method_title);

            SymTable *tmp = global_table;
            while(tmp && tmp->next) tmp = tmp->next;
            if(tmp) tmp->next = method_table;

            insert_symbol(method_table, "return", get_type_from_node(ret_type), 0, NULL, 0, 0);

            SymTable *old_table = current_table;
            current_table = method_table;
            check_semantics(params_node);
            check_semantics(body);
            current_table = old_table;
            break;
        }

        case VarDecl: {

            struct node *type_node = n->children->node;
            struct node *id_node = n->children->next->node;
            insert_symbol(current_table, id_node->token, get_type_from_node(type_node), 0, NULL, id_node->line, id_node->col);
            break;
        }

        case ParamDecl: {

            struct node *type_node = n->children->node;
            struct node *id_node = n->children->next->node;
            insert_symbol(current_table, id_node->token, get_type_from_node(type_node), 1, NULL, id_node->line, id_node->col);
            break;
        }

        default: {

            if (n && n->children) {
                struct node_list *curr = n->children;
                while (curr) {
                    check_semantics(curr->node);
                    curr = curr->next;
                }
            }
        }
    }
}