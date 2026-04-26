#include <stdlib.h>
#include <stdio.h>
#include "ast.h"

//He puesto todas las que he encontrado, vamos adaptando
const char* category_names[] = {
    "Program", "FieldDecl", "VarDecl", "MethodDecl", "MethodHeader", "MethodParams",
    "ParamDecl", "MethodBody", "Block", "If", "While", "Return", "Call", "Print", "ParseArgs",
    "Assign", "Or", "And", "Eq", "Ne", "Lt", "Gt", "Le", "Ge", "Add", "Sub", "Mul", "Div", "Mod", "Lshift",
    "Rshift", "Xor", "Not", "Minus", "Plus", "Length", "Bool", "BoolLit", "Double", "Decimal",
    "Identifier", "Int", "Natural", "StrLit", "StringArray", "Reserved", "Void"
};

// create a node of a given category with a given lexical symbol
struct node *newnode(enum category category, char *token, int line, int col) {
    struct node *new = malloc(sizeof(struct node));
    new->category = category;
    new->token = token;
    new->line = line;
    new->col = col;
    new->annot_type = T_Undef
    new->children = malloc(sizeof(struct node_list));
    new->children->node = NULL;
    new->children->next = NULL;
    return new;
}

// append a node to the list of children of the parent node
void addchild(struct node *parent, struct node *child) {

    if (parent == NULL || child == NULL) return;

    struct node_list *new = malloc(sizeof(struct node_list));
    new->node = child;
    new->next = NULL;

    if (parent->children->node == NULL && parent->children->next == NULL) {
        parent->children->node = child;
        free(new);
    } else {
        struct node_list *current = parent->children;
        while(current->next != NULL)
            current = current->next;
        current->next = new;
    }
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
    
    // Solo nodos expresiones por ahora
    if (n->annot_type != T_Undef || n->category == Call || n->category == Assign || /* añadir más categorías de expresiones si necesario*/
        n->category == Identifier || n->category == Natural || n->category == Decimal || n->category == BoolLit) {
        
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
    struct node_list *curr = n->children;
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
