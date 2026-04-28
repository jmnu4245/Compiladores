/* semantic.c — Análise semântica completa para o compilador Juc
 * Refactored: modular helpers + all missing checks implemented
 */

#include "ast.h"
#include "sym_table.h"
#include "y.tab.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

SymTable *global_table  = NULL;
SymTable *current_table = NULL;
void check_semantics_pass2(struct node *n);

/* ================================================================
 * UTILITIES
 * ================================================================ */

 struct node* get_identifier(struct node *parent) {
    if (!parent || !parent->children) return NULL;
    struct node_list *curr = parent->children;
    while (curr) {
        if (curr->node && curr->node->category == Identifier) {
            return curr->node;
        }
        curr = curr->next;
    }
    return NULL;
}

BasicType get_type_from_node(struct node *n) {
    if (n == NULL) return T_Undef;
    switch (n->category) {
        case Int:          return T_Int;
        case Double:       return T_Double;
        case Bool:         return T_Bool;
        case VoidNode:     return T_Void;
        case StringArray:  return T_StringArray;
        default:           return T_Undef;
    }
}

static int is_numeric(BasicType t) {
    return t == T_Int || t == T_Double;
}

/* formal <- actual: int→double promotion allowed */
static int types_compatible(BasicType formal, BasicType actual) {
    return formal == actual || (formal == T_Double && actual == T_Int);
}

/* Count comma-separated items in "(int,double)" → 2, "()" → 0 */
static int count_formal_params(const char *p) {
    if (!p || strcmp(p, "()") == 0) return 0;
    int n = 1;
    for (p++; *p && *p != ')'; p++)
        if (*p == ',') n++;
    return n;
}

/* Parse "(int,double,boolean)" into a heap-allocated BasicType[].
 * Caller must free(). Sets *count. */
static BasicType *parse_formal_types(const char *params_str, int *count) {
    *count = count_formal_params(params_str);
    if (*count == 0) return NULL;
    BasicType *types = malloc(*count * sizeof(BasicType));
    const char *p = params_str + 1;           /* skip '(' */
    for (int i = 0; i < *count; i++) {
        char buf[64]; int j = 0;
        while (*p && *p != ',' && *p != ')') buf[j++] = *p++;
        buf[j] = '\0';
        if (*p == ',') p++;
        if      (strcmp(buf, "int")      == 0) types[i] = T_Int;
        else if (strcmp(buf, "double")   == 0) types[i] = T_Double;
        else if (strcmp(buf, "boolean")  == 0) types[i] = T_Bool;
        else if (strcmp(buf, "String[]") == 0) types[i] = T_StringArray;
        else                                   types[i] = T_Undef;
    }
    return types;
}

/* Build "(int,double)" from a MethodParams AST node */
static void build_params_str(struct node *params_node, char *buf, int bufsz) {
    strncpy(buf, "(", bufsz);
    int first = 1;
    if (params_node && params_node->children) {
        struct node_list *cur = params_node->children;
        while (cur) {
            if (cur->node && cur->node->children) {
                if (!first) strncat(buf, ",", bufsz - strlen(buf) - 1);
                BasicType t = get_type_from_node(cur->node->children->node);
                strncat(buf, type_to_str(t), bufsz - strlen(buf) - 1);
                first = 0;
            }
            cur = cur->next;
        }
    }
    strncat(buf, ")", bufsz - strlen(buf) - 1);
}

/* ================================================================
 * INDIVIDUAL EXPRESSION CHECKS
 * ================================================================ */

/* ---- Literals ---- */
static void check_literal(struct node *n) {
    switch (n->category) {
        case Natural: {
            /* Spec: 2147483648 is ALWAYS out of bounds */
            long long val = atoll(n->token);
            if (val > 2147483647LL) {
                printf("Line %d, col %d: Number %s out of bounds\n",
                       n->line, n->col, n->token);
            }
            n->annot_type = T_Int;
            break;
        }
        case Decimal:  n->annot_type = T_Double; break;
        case BoolLit:  n->annot_type = T_Bool;   break;
        default:       n->annot_type = T_Undef;  break;
    }
}

/* ---- Identifier lookup ---- */
static void check_identifier(struct node *n, SymTable *global, SymTable *local) {
    Symbol *sym = lookup_symbol(global, local, n->token);
    if (!sym) {
        printf("Line %d, col %d: Cannot find symbol %s\n",
               n->line, n->col, n->token);
        n->annot_type = T_Undef;
    } else {
        n->annot_type = sym->type;
    }
}

/* ---- Arithmetic: Add Sub Mul Div Mod (binary, both numeric) ---- */
static void check_math_operator(struct node *n) {
    struct node *left  = get_child(n, 0);
    struct node *right = get_child(n, 1);
    if (!left || !right) { n->annot_type = T_Undef; return; }

    if (!is_numeric(left->annot_type) || !is_numeric(right->annot_type)) {
        printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n",
               n->line, n->col, n->token,
               type_to_str(left->annot_type), type_to_str(right->annot_type));
        n->annot_type = T_Undef;
    } else {
        n->annot_type = (left->annot_type == T_Double ||
                         right->annot_type == T_Double) ? T_Double : T_Int;
    }
}

/* ---- Relational: Eq Ne Lt Gt Le Ge → always boolean ---- */
static void check_relational_operator(struct node *n) {
    struct node *left  = get_child(n, 0);
    struct node *right = get_child(n, 1);
    if (left && right) {
        int ok;
        if (n->category == Eq || n->category == Ne)
            /* == and != accept same type OR mixed numeric */
            ok = (left->annot_type == right->annot_type ||
                  (is_numeric(left->annot_type) && is_numeric(right->annot_type)));
        else
            /* < > <= >= require both numeric */
            ok = is_numeric(left->annot_type) && is_numeric(right->annot_type);

        if (!ok) {
            printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n",
                   n->line, n->col, n->token,
                   type_to_str(left->annot_type), type_to_str(right->annot_type));
        }
    }
    n->annot_type = T_Bool; /* always boolean, even on error */
}

/* ---- Logical: And Or (boolean), Not (boolean), Xor (boolean or int) ---- */
static void check_logical_operator(struct node *n) {
    if (n->category == Not) {
        struct node *child = get_child(n, 0);
        if (child && child->annot_type != T_Bool && child->annot_type != T_Undef) {
            printf("Line %d, col %d: Operator %s cannot be applied to type %s\n",
                   n->line, n->col, n->token, type_to_str(child->annot_type));
            n->annot_type = T_Undef;
        } else {
            n->annot_type = T_Bool;
        }
        return;
    }

    struct node *left  = get_child(n, 0);
    struct node *right = get_child(n, 1);
    if (!left || !right) { n->annot_type = T_Undef; return; }

    if (n->category == Xor) {
        /* ^ works on both booleans or both ints (Java semantics) */
        if (left->annot_type == T_Bool && right->annot_type == T_Bool) {
            n->annot_type = T_Bool;
        } else if (left->annot_type == T_Int && right->annot_type == T_Int) {
            n->annot_type = T_Int;
        } else {
            printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n",
                   n->line, n->col, n->token,
                   type_to_str(left->annot_type), type_to_str(right->annot_type));
            n->annot_type = T_Undef;
        }
    } else { /* And, Or: both must be boolean */
        if (left->annot_type != T_Bool || right->annot_type != T_Bool) {
            printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n",
                   n->line, n->col, n->token,
                   type_to_str(left->annot_type), type_to_str(right->annot_type));
            n->annot_type = T_Undef;
        } else {
            n->annot_type = T_Bool;
        }
    }
}

/* ---- Shift: Lshift Rshift (int, int → int) ---- */
static void check_shift_operator(struct node *n) {
    struct node *left  = get_child(n, 0);
    struct node *right = get_child(n, 1);
    if (!left || !right) { n->annot_type = T_Undef; return; }
    if (left->annot_type != T_Int || right->annot_type != T_Int) {
        printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n",
               n->line, n->col, n->token,
               type_to_str(left->annot_type), type_to_str(right->annot_type));
        n->annot_type = T_Undef;
    } else {
        n->annot_type = T_Int;
    }
}

/* ---- Unary: Minus(1) Plus(1) — single numeric child ---- */
static void check_unary_operator(struct node *n) {
    struct node *child = get_child(n, 0);
    if (!child) { n->annot_type = T_Undef; return; }
    if (!is_numeric(child->annot_type)) {
        printf("Line %d, col %d: Operator %s cannot be applied to type %s\n",
               n->line, n->col, n->token, type_to_str(child->annot_type));
        n->annot_type = T_Undef;
    } else {
        n->annot_type = child->annot_type;
    }
}

/* ---- Assignment: left = right ---- */
static void check_assignment(struct node *n) {
    struct node *left  = get_child(n, 0);
    struct node *right = get_child(n, 1);
    if (!left || !right) { n->annot_type = T_Undef; return; }

    if (types_compatible(left->annot_type, right->annot_type)) {
        n->annot_type = left->annot_type;
    } else {
        printf("Line %d, col %d: Incompatible type %s in assign statement\n",
               n->line, n->col, type_to_str(right->annot_type));
        n->annot_type = left->annot_type; /* keep LHS type to continue */
    }
}

/* ---- Return ---- */
static void check_return(struct node *n, SymTable *local) {
    struct node *expr  = get_child(n, 0);
    Symbol      *ret   = search_symbol(local, "return");
    BasicType expected = ret  ? ret->type         : T_Void;
    BasicType actual   = expr ? expr->annot_type  : T_Void;

    if (!types_compatible(expected, actual)) {
        printf("Line %d, col %d: Incompatible type %s in return statement\n",
               n->line, n->col, type_to_str(actual));
    }
    n->annot_type = T_None; /* return is a statement */
}

/* ---- Print: statement, no expression type ---- */
static void check_print(struct node *n) {
    /* System.out.print accepts int, double, boolean, String literals.
     * The argument type was already validated by the recursive descent.
     * The node itself is a statement: no annotation needed. */
    n->annot_type = T_None;
}

/* ---- ParseArgs: Integer.parseInt(id[expr]) → int ---- */
static void check_parse_args(struct node *n, SymTable *global, SymTable *local) {
    /* ParseArgs(2): children = (Identifier, Expr_index) */
    struct node *id_node  = get_child(n, 0);
    /* idx_node already validated as expression by the recursive descent */

    if (id_node) {
        Symbol *sym = lookup_symbol(global, local, id_node->token);
        if (!sym) {
            printf("Line %d, col %d: Cannot find symbol %s\n",
                   id_node->line, id_node->col, id_node->token);
            id_node->annot_type = T_Undef;
        } else {
            id_node->annot_type = sym->type;
            /* Note: the spec says Integer.parseInt is an operator that yields int;
             * type of the array operand is implicitly String[] — enforced at syntax level. */
        }
    }
    n->annot_type = T_Int; /* always int per spec */
}

/* ---- Length: id.length → int ---- */
static void check_length(struct node *n, SymTable *global, SymTable *local) {
    /* Length(1): single Identifier child */
    struct node *id_node = get_child(n, 0);
    if (id_node) {
        Symbol *sym = lookup_symbol(global, local, id_node->token);
        if (!sym) {
            printf("Line %d, col %d: Cannot find symbol %s\n",
                   id_node->line, id_node->col, id_node->token);
            id_node->annot_type = T_Undef;
        } else {
            id_node->annot_type = sym->type;
            if (sym->type != T_StringArray) {
                printf("Line %d, col %d: Operator .length cannot be applied to type %s\n",
                       id_node->line, id_node->col, type_to_str(sym->type));
            }
        }
    }
    n->annot_type = T_Int;
}

/* ---- Method call: full overload resolution per spec ----
 *
 * Algorithm (simplified Java rules from spec §3.3):
 *   1. Exact match on argument count AND types → use it.
 *   2. No exact match → collect all methods with same name and arg count
 *      where each actual type is compatible with the corresponding formal
 *      (int→double promotion counts as compatible).
 *      a. Exactly one such method → use it.
 *      b. More than one         → "Reference to method X is ambiguous".
 *      c. Zero                  → "Cannot find symbol X".
 *
 * NOTE: This function iterates global_table->symbols directly.
 *       Adjust the field name if your sym_table uses a different one
 *       (e.g., `head`, `first`, `entries`).
 */
static void check_method_call(struct node *n, SymTable *global, SymTable *local) {
    struct node *id_node = get_child(n, 0);
    if (!id_node) { n->annot_type = T_Undef; return; }

    const char *method_name = id_node->token;

    /* Collect actual argument types (children after the Identifier node) */
    int n_actual = 0;
    BasicType actual_types[64];
    struct node_list *arg = n->children ? n->children->next : NULL;
    while (arg && n_actual < 64) {
        if (arg->node) actual_types[n_actual++] = arg->node->annot_type;
        arg = arg->next;
    }

    Symbol *exact_match  = NULL;
    Symbol *compat_match = NULL;
    int     ambiguous    = 0;

    /* Iterate all symbols in the global table looking for methods */
for (Symbol *sym = global->first; sym && !exact_match; sym = sym->next) {
        if (!sym->params_list || strcmp(sym->name, method_name) != 0) continue;

        int n_formal;
        BasicType *formal_types = parse_formal_types(sym->params_list, &n_formal);
        if (n_formal != n_actual) { free(formal_types); continue; }

        int exact = 1, compat = 1;
        for (int i = 0; i < n_actual; i++) {
            if (formal_types[i] != actual_types[i])
                exact = 0;
            if (!types_compatible(formal_types[i], actual_types[i]))
                compat = 0;
        }
        free(formal_types);

        if (exact) {
            exact_match = sym;       /* stop immediately — cannot do better */
        } else if (compat) {
            if (compat_match) ambiguous = 1;
            else              compat_match = sym;
        }
    }

    if (exact_match) {
         n->annot_type         = exact_match->type;
    id_node->annot_type   = T_None;
    //id_node->annot_params = exact_match->params_list;
    } else if (ambiguous) {
        printf("Line %d, col %d: Reference to method %s is ambiguous\n",
               id_node->line, id_node->col, method_name);
        n->annot_type = id_node->annot_type = T_Undef;
    } else if (compat_match) {
        n->annot_type         = compat_match->type;
    id_node->annot_type   = T_None;
    id_node->annot_params = compat_match->params_list;
    } else {
        printf("Line %d, col %d: Cannot find symbol %s\n",
               id_node->line, id_node->col, method_name);
        n->annot_type = id_node->annot_type = T_Undef;
        }
}

/* ================================================================
 * MAIN check_expression  (post-order dispatch)
 * ================================================================ */

void check_expression(struct node *n, SymTable *global, SymTable *local) {
    if (!n) return;

    /* 1. Post-order: resolve children before the parent */
    for (struct node_list *c = n->children; c; c = c->next)
        check_expression(c->node, global, local);

    /* 2. Delegate to the appropriate helper */
    switch (n->category) {
        case Natural: case Decimal: case BoolLit:
            check_literal(n);                           break;
        case Identifier:
            check_identifier(n, global, local);         break;
        case Add: case Sub: case Mul: case Div: case Mod:
            check_math_operator(n);                     break;
        case Eq: case Ne: case Lt: case Gt: case Le: case Ge:
            check_relational_operator(n);               break;
        case And: case Or: case Not: case Xor:
            check_logical_operator(n);                  break;
        case Lshift: case Rshift:
            check_shift_operator(n);                    break;
        case Minus: case Plus:          /* unary: Minus(1) / Plus(1) */
            check_unary_operator(n);                    break;
        case Assign:
            check_assignment(n);                        break;
        case Return:
            check_return(n, local);                     break;
        case Print:
            check_print(n);                             break;
        case ParseArgs:
            check_parse_args(n, global, local);         break;
        case Length:
            check_length(n, global, local);             break;
        case Call:
            check_method_call(n, global, local);        break;
        default:
            n->annot_type = T_None;                   break;
    }
}

/* ================================================================
 * PASS 1 — register global symbols (fields + method headers)
 *           so methods can be called before they are declared.
 * ================================================================ */

/* Returns 1 if the identifier should be rejected (reserved or duplicate) */
static int check_declaration_errors(const char *token, int line, int col,
                                    SymTable *table, const char *params) {

    if (!token) return 0;                                    
    if (strcmp(token, "_") == 0) {
        printf("Line %d, col %d: Symbol _ is reserved\n", line, col);
        return 1;
    }
    /* Duplicate: for methods, duplicate = same name AND same param list */
    Symbol *existing = search_symbol(table, token);
    if (existing) {
        /* For regular variables / fields: any match is a duplicate */
        if (!params && !existing->params_list) {
            printf("Line %d, col %d: Symbol %s already defined\n", line, col, token);
            return 1;
        }
        /* For methods: duplicate only if same signature */
        if (params && existing->params_list && strcmp(existing->params_list, params) == 0) {
            printf("Line %d, col %d: Symbol %s already defined\n", line, col, token);
            return 1;
        }
    }
    return 0;
}

static void register_global_field(struct node *field) {
    struct node *type_node = get_child(field,0);
    struct node *id_node   = get_identifier(field);

    if (!type_node || !id_node || !id_node->token) return;

    if (check_declaration_errors(id_node->token, id_node->line, id_node->col,
                                 global_table, NULL))
        return;

    insert_symbol(global_table, id_node->token,
                  get_type_from_node(type_node), 0, NULL,
                  id_node->line, id_node->col);
}

static void register_method_header(struct node *method) {
    struct node *header = get_child(method, 0);
    if (!header) return;
    struct node *method_id = get_identifier(header);
    if (!method_id || !method_id->token) return;
    struct node *ret_type = get_child(header, 0);
    struct node *params_node = get_child(header, 2);

    char params_str[256];
    build_params_str(params_node, params_str, sizeof(params_str));

    if (check_declaration_errors(method_id->token, method_id->line, method_id->col,
                                 global_table, params_str))
        return;

    insert_symbol(global_table, method_id->token,
                  get_type_from_node(ret_type), 0, params_str,
                  method_id->line, method_id->col);
}

void check_semantics_pass1(struct node *n) {
    if (!n || n->category != Program) return;

    struct node *class_id = n->children ? n->children->node : NULL;
    char title[256];
    sprintf(title, "Class %s",
            (class_id && class_id->token) ? class_id->token : "Unknown");

    global_table  = create_table(title);
    current_table = global_table;

    for (struct node_list *cur = n->children; cur; cur = cur->next) {
        if (!cur->node) continue;
        if      (cur->node->category == FieldDecl)  register_global_field(cur->node);
        else if (cur->node->category == MethodDecl) register_method_header(cur->node);
    }
}

/* ================================================================
 * PASS 2 — process method bodies: declare local symbols + type-check
 * ================================================================ */


static void process_method_body(struct node *method) {
    struct node *header = get_child(method, 0);
    struct node *body   = get_child(method, 1);
    if (!header || !body) return;

    struct node *ret_type    = get_child(header, 0);
    struct node *method_id   = get_identifier(header);
    struct node *params_node = get_child(header, 2);
    if (!method_id || !method_id->token) return;

    char params_str[256];
    build_params_str(params_node, params_str, sizeof(params_str));

    char method_title[512];
    sprintf(method_title, "Method %s%s", method_id->token, params_str);

    SymTable *method_table = create_table(method_title);

    /* Append to the end of the table linked list */
    SymTable *tmp = global_table;
    while (tmp->next) tmp = tmp->next;
    tmp->next = method_table;

    /* "return" pseudo-symbol stores the expected return type */
    insert_symbol(method_table, "return",
                  get_type_from_node(ret_type), 0, NULL, 0, 0);

    SymTable *old_table = current_table;
    current_table = method_table;

    check_semantics_pass2(params_node);  /* register formal parameters */
    check_semantics_pass2(body);         /* process body statements    */

    current_table = old_table;
}



void check_semantics_pass2(struct node *n) {
    if (!n) return;

    switch (n->category) {

        case Program: {
            /* Skip the first child (class Identifier), process the rest */
            struct node_list *cur = n->children ? n->children->next : NULL;
            for (; cur; cur = cur->next)
                check_semantics_pass2(cur->node);
            break;
        }

        case MethodDecl:
            process_method_body(n);
            break;

        case ParamDecl: {
            struct node *type_node = get_child(n, 0);
            struct node *id_node   = get_identifier(n);
            if (!type_node || !id_node || !id_node->token) break;

            if (!check_declaration_errors(id_node->token, id_node->line, id_node->col, current_table, NULL))
                insert_symbol(current_table, id_node->token, get_type_from_node(type_node), 1, NULL, id_node->line, id_node->col);
            break;
        }

        case VarDecl: {
            struct node *type_node = get_child(n, 0);
            struct node *id_node   = get_identifier(n);
            if (!type_node || !id_node || !id_node->token) break;

            if (!check_declaration_errors(id_node->token, id_node->line, id_node->col, current_table, NULL))
                insert_symbol(current_table, id_node->token, get_type_from_node(type_node), 0, NULL, id_node->line, id_node->col);
            break;
        }

        /* All expression / statement nodes: delegate to check_expression */
        case Assign:
        case Add: case Sub: case Mul: case Div: case Mod:
        case Eq:  case Ne:  case Lt:  case Gt:  case Le:  case Ge:
        case And: case Or:  case Xor: case Not:
        case Lshift: case Rshift:
        case Minus: case Plus:          /* unary */
        case Identifier: case Natural: case Decimal: case BoolLit:
        case Call: case Return: case Print: case Length: case ParseArgs:
            check_expression(n, global_table, current_table);
            break;

        /* Structural nodes (If, While, Block, MethodBody, …): recurse */
        default: {
            for (struct node_list *cur = n->children; cur; cur = cur->next)
                check_semantics_pass2(cur->node);
            break;
        }
    }
}

/* ================================================================
 * ENTRY POINT
 * ================================================================ */

void check_semantics(struct node *n) {
    check_semantics_pass1(n);   /* global symbol table */
    check_semantics_pass2(n);   /* local tables + type checking */
}