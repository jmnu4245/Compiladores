#include <stdlib.h>
#include <stdio.h>
#include "ast.h"
#include "sym_table.h"

//He puesto todas las que he encontrado, vamos adaptando
const char* category_names[] = {
    "Program", "FieldDecl", "VarDecl", "MethodDecl", "MethodHeader", "MethodParams",
    "ParamDecl", "MethodBody", "Block", "If", "While", "Return", "Call", "Print", "ParseArgs",
    "Assign", "Or", "And", "Eq", "Ne", "Lt", "Gt", "Le", "Ge", "Add", "Sub", "Mul", "Div", "Mod", "Lshift",
    "Rshift", "Xor", "Not", "Minus", "Plus", "Length", "Bool", "BoolLit", "Double", "Decimal",
    "Identifier", "Int", "Natural", "StrLit", "StringArray", "Void", "Aux"
};
// create a node of a given category with a given lexical symbol
struct node *newnode(enum category category, char *token, YYLTYPE position) {
    struct node *new = malloc(sizeof(struct node));
    new->category = category;
    new->token = token;
    new->line = position.first_line;
    new->col = position.first_column;
    new->annot_type = T_None;
    new->annot_params = NULL;
    new->children = malloc(sizeof(struct node_list));
    new->children->node = NULL;
    new->children->next = NULL;
    return new;
}

int count_children(struct node *n) {
    if (n == NULL || n->children == NULL) return 0;
    int count = 0;
    struct node_list *curr = n->children;
    while (curr != NULL) {
        if (curr->node != NULL) count++;
        curr = curr->next;
    }
    return count;
}
// append a node to the list of children of the parent node
void addchild(struct node *parent, struct node *child) {
    //if (parent == NULL || child == NULL) return;
    struct node_list *new = malloc(sizeof(struct node_list));
    new->node = child;
    new->next = NULL;
    struct node_list *children = parent->children;
    while (children->next != NULL)
        children = children->next;
    children->next = new;
}
// show subtree given a node
void show(struct node *n, int depth) {
    if (n == NULL) return;
    for (int i = 0; i < depth * 2; i++) {
        printf("."); 
    }
    printf("%s", category_names[n->category]);
    if (n->token != NULL) {
        printf("(%s)", n->token);
    }
    printf("\n");
    struct node_list *current = n->children;
    while (current != NULL) {
        show(current->node, depth + 1);
        current = current->next;
    }
}

void show_annotated(struct node *n, int depth) {
    if (n == NULL) return;
    for (int i = 0; i < depth * 2; i++) printf("."); 
    
    printf("%s", category_names[n->category]);
    if (n->token != NULL) printf("(%s)", n->token);
    
    if (n->annot_params != NULL) {
        printf(" - %s", n->annot_params);
    } else if (n->annot_type != T_None) {
        printf(" - %s", type_to_str(n->annot_type));
    }
    printf("\n");
    
    struct node_list *current = n->children;
    while (current != NULL) {
        show_annotated(current->node, depth + 1);
        current = current->next;
    }
}

struct node *get_child(struct node *n, int index) {
    if (n == NULL || n->children == NULL) return NULL;
    struct node_list *curr = n->children->next;//Hay un nodo centinela
    int i = 0;
    while (curr != NULL && i < index) {
        curr = curr->next;
        i++;
    }
    return curr ? curr->node : NULL;
}
//Dealocate tree
void free_tree(struct node *n) {
    if (n == NULL) return;
    struct node_list *current = n->children;
    while (current != NULL) {
        struct node_list *next = current->next;
        free_tree(current->node);
        free(current);
        current = next;
    }
    if (n->token != NULL) free(n->token);
    free(n);
}

void unpack_nodes(struct node *parent, struct node *container) {
    if (container == NULL || container->children == NULL) return;
    struct node_list *curr = container->children;
    while (curr != NULL) {
        if (curr->node != NULL) {
            addchild(parent, curr->node);
        }
        curr = curr->next;
    }
}
