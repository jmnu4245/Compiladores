/* semantic.c — Semantic analysis for the Juc compiler */

#include "ast.h"
#include "sym_table.h"
#include "y.tab.h"
#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

SymTable *global_table  = NULL;
SymTable *current_table = NULL;
int print_errors = 0;

/* Forward declarations */
static void check_node(struct node *n, SymTable *global, SymTable *local);
void check_semantics_pass2(struct node *n);

/* ================================================================
 * ERROR REPORTING
 * One function per error type defined in the spec.
 * ================================================================ */
/* ================================================================
 * ERROR COLLECTOR (Strategy 3)
 * ================================================================ */

typedef struct ErrorNode {
    int line;
    int col;
    char msg[512];
    struct ErrorNode *next;
} ErrorNode;

static ErrorNode *error_list_head = NULL;

/* Inserta el error ordenado por línea y columna. Ignora duplicados exactos. */
static void add_error(int line, int col, const char *msg) {
    for (ErrorNode *cur = error_list_head; cur; cur = cur->next)
        if (cur->line == line && cur->col == col && strcmp(cur->msg, msg) == 0)
            return;

    ErrorNode *new_err = malloc(sizeof(ErrorNode));
    new_err->line = line;
    new_err->col  = col;
    strncpy(new_err->msg, msg, sizeof(new_err->msg) - 1);
    new_err->msg[sizeof(new_err->msg) - 1] = '\0';
    new_err->next = NULL;

    if (!error_list_head) { error_list_head = new_err; return; }
    ErrorNode *tail = error_list_head;
    while (tail->next) tail = tail->next;
    tail->next = new_err;
}



void print_semantic_errors() {
    ErrorNode *curr = error_list_head;
    while (curr != NULL) {
        printf("%s", curr->msg);
        ErrorNode *temp = curr;
        curr = curr->next;
        free(temp);
    }
    error_list_head = NULL;
}
static void err_already_defined(int line, int col, const char *name, const char *params) {
    char buf[512];
    if (params)
        snprintf(buf, sizeof(buf), "Line %d, col %d: Symbol %s%s already defined\n", line, col, name, params);
    else
        snprintf(buf, sizeof(buf), "Line %d, col %d: Symbol %s already defined\n", line, col, name);
    add_error(line, col, buf);
}

static void err_reserved(int line, int col) {
    char buf[512];
    snprintf(buf, sizeof(buf), "Line %d, col %d: Symbol _ is reserved\n", line, col);
    add_error(line, col, buf);
}

static void err_cannot_find(int line, int col, const char *name) {
    char buf[512];
    snprintf(buf, sizeof(buf), "Line %d, col %d: Cannot find symbol %s\n", line, col, name);
    add_error(line, col, buf);
}

static void err_op_unary(int line, int col, const char *op, BasicType t) {
    char buf[512];
    snprintf(buf, sizeof(buf), "Line %d, col %d: Operator %s cannot be applied to type %s\n",
             line, col, op, type_to_str(t));
    add_error(line, col, buf);
}

static void err_op_binary(int line, int col, const char *op, BasicType t1, BasicType t2) {
    char buf[512];
    snprintf(buf, sizeof(buf), "Line %d, col %d: Operator %s cannot be applied to types %s, %s\n",
             line, col, op, type_to_str(t1), type_to_str(t2));
    add_error(line, col, buf);
}

static void err_incompatible_type(int line, int col, BasicType t, const char *stmt) {
    char buf[512];
    snprintf(buf, sizeof(buf), "Line %d, col %d: Incompatible type %s in %s statement\n",
             line, col, type_to_str(t), stmt);
    add_error(line, col, buf);
}

static void err_incompatible_void(int line, int col, const char *stmt) {
    char buf[512];
    snprintf(buf, sizeof(buf), "Line %d, col %d: Incompatible type void in %s statement\n", line, col, stmt);
    add_error(line, col, buf);
}

static void err_number_out_of_bounds(int line, int col, const char *token) {
    char buf[512];
    snprintf(buf, sizeof(buf), "Line %d, col %d: Number %s out of bounds\n", line, col, token);
    add_error(line, col, buf);
}

static void err_ambiguous(int line, int col, const char *name) {
    char buf[512];
    snprintf(buf, sizeof(buf), "Line %d, col %d: Reference to method %s is ambiguous\n", line, col, name);
    add_error(line, col, buf);
}

/* ================================================================
 * UTILITIES
 * ================================================================ */

static int is_numeric(BasicType t) {
    return t == T_Int || t == T_Double;
}

/* Checks whether actual type can be assigned/passed to formal type.
 * Only int -> double promotion is allowed. */
static int types_compatible(BasicType formal, BasicType actual) {
    return formal == actual || (formal == T_Double && actual == T_Int);
}

/* Maps a type AST node (Int, Double, Bool, VoidNode, StringArray) to BasicType. */
static BasicType type_from_node(struct node *n) {
    if (!n) return T_Undef;
    switch (n->category) {
        case Int:         return T_Int;
        case Double:      return T_Double;
        case Bool:        return T_Bool;
        case VoidNode:    return T_Void;
        case StringArray: return T_StringArray;
        default:          return T_Undef;
    }
}

static const char* get_op_str(int category) {
    switch(category) {
        case Add: return "+";    case Sub: return "-";    case Mul: return "*";
        case Div: return "/";    case Mod: return "%";    case Assign: return "=";
        case Eq:  return "==";   case Ne:  return "!=";   case Lt:  return "<";
        case Gt:  return ">";    case Le:  return "<=";   case Ge:  return ">=";
        case And: return "&&";   case Or:  return "||";   case Xor: return "^";
        case Not: return "!";    case Lshift: return "<<";case Rshift: return ">>";
        case Minus: return "-";  case Plus: return "+";
        default: return "";
    }
}

/* Finds the first Identifier child of a node (used for declarations). */
static struct node *get_identifier(struct node *parent) {
    if (!parent) return NULL;
    /* children->next skips the sentinel node at the head of the list */
    for (struct node_list *c = parent->children ? parent->children->next : NULL;
         c; c = c->next)
        if (c->node && c->node->category == Identifier) return c->node;
    return NULL;
}

/* Serialises a MethodParams node into a "(type1,type2)" string. */
static void build_params_str(struct node *params, char *buf, int bufsz) {
    strncpy(buf, "(", bufsz);
    int first = 1;
    if (params) {
        for (struct node_list *c = params->children ? params->children->next : NULL;
             c; c = c->next) {
            if (!c->node) continue;
            if (!first) strncat(buf, ",", bufsz - strlen(buf) - 1);
            strncat(buf, type_to_str(type_from_node(get_child(c->node, 0))),
                    bufsz - strlen(buf) - 1);
            first = 0;
        }
    }
    strncat(buf, ")", bufsz - strlen(buf) - 1);
}

/* Returns the number of comma-separated types inside "(int,double)".
 * An empty parameter list "()" returns 0. */
static int count_formal_params(const char *p) {
    if (!p || strcmp(p, "()") == 0) return 0;
    int n = 1;
    for (p++; *p && *p != ')'; p++)
        if (*p == ',') n++;
    return n;
}

/* Parses "(int,double)" into a heap-allocated BasicType[]. Caller must free(). */
static BasicType *parse_formal_types(const char *params_str, int *count) {
    *count = count_formal_params(params_str);
    if (*count == 0) return NULL;
    BasicType *types = malloc(*count * sizeof(BasicType));
    const char *p = params_str + 1;   /* skip '(' */
    for (int i = 0; i < *count; i++) {
        char buf[64]; int j = 0;
        while (*p && *p != ',' && *p != ')') buf[j++] = *p++;
        buf[j] = '\0';
        if (*p == ',') p++;
        if      (!strcmp(buf, "int"))      types[i] = T_Int;
        else if (!strcmp(buf, "double"))   types[i] = T_Double;
        else if (!strcmp(buf, "boolean"))  types[i] = T_Bool;
        else if (!strcmp(buf, "String[]")) types[i] = T_StringArray;
        else                               types[i] = T_Undef;
    }
    return types;
}

/* ================================================================
 * INDIVIDUAL NODE CHECKS
 * Each function assumes its children have already been annotated.
 * ================================================================ */

static void check_literal(struct node *n) {
    switch (n->category) {
        case Natural: {
            char stripped[256]; int j = 0;
            for (int k = 0; n->token[k] && j < 255; k++)
                if (n->token[k] != '_') stripped[j++] = n->token[k];
            stripped[j] = '\0';
            
            long long val = atoll(stripped);
            if (val > 2147483647LL) err_number_out_of_bounds(n->line, n->col, n->token);
            n->annot_type = T_Int;
            break;
        }
        case Decimal: {
            char stripped[1024]; int j = 0;
            for (int k = 0; n->token[k] && j < 1023; k++)
                if (n->token[k] != '_') stripped[j++] = n->token[k];
            stripped[j] = '\0';
            
            errno = 0;
            double val = strtod(stripped, NULL);
            // Atrapa Infinito (overflow) y 0.0 (underflow real)
            if (isinf(val) || (val == 0.0 && errno == ERANGE)) 
                err_number_out_of_bounds(n->line, n->col, n->token);
            n->annot_type = T_Double;
            break;
        }
        case BoolLit: n->annot_type = T_Bool;   break;
        case StrLit:  n->annot_type = T_String; break;
        default:      n->annot_type = T_Undef;  break;
    }
}

static void check_identifier(struct node *n, SymTable *global, SymTable *local) {
    Symbol *sym = NULL;

    for (SymTable *tbl = local; tbl && !sym; tbl = tbl->next) {
        for (Symbol *s = tbl->first; s; s = s->next)
            if (!s->params_list && strcmp(s->name, n->token) == 0) { sym = s; break; }
    }
    if (!sym) {
        for (Symbol *s = global->first; s; s = s->next)
            if (!s->params_list && strcmp(s->name, n->token) == 0) { sym = s; break; }
    }
    if (!sym) { err_cannot_find(n->line, n->col, n->token); n->annot_type = T_Undef; }
    else        n->annot_type = sym->type;
}

/* Add, Sub, Mul, Div, Mod */
static void check_math_op(struct node *n) {
    struct node *l = get_child(n, 0), *r = get_child(n, 1);
    
    // Eliminado el early return de T_Undef
    if (!is_numeric(l->annot_type) || !is_numeric(r->annot_type)) {
        err_op_binary(n->line, n->col, get_op_str(n->category), l->annot_type, r->annot_type);
        n->annot_type = T_Undef;
    } else {
        n->annot_type = (l->annot_type == T_Double || r->annot_type == T_Double)
                        ? T_Double : T_Int;
    }
}

/* Eq, Ne, Lt, Gt, Le, Ge */
static void check_relational_op(struct node *n) {
    struct node *l = get_child(n, 0), *r = get_child(n, 1);
    
    int ok = 0;
    if (n->category == Eq || n->category == Ne) {
        if ((l->annot_type == r->annot_type && l->annot_type != T_StringArray && l->annot_type != T_Undef) ||
            (is_numeric(l->annot_type) && is_numeric(r->annot_type))) {
            ok = 1;
        }
    } else {
        if (is_numeric(l->annot_type) && is_numeric(r->annot_type)) ok = 1;
    }
             
    if (!ok) err_op_binary(n->line, n->col, get_op_str(n->category), l->annot_type, r->annot_type);
    
    n->annot_type = T_Bool;
}

/* And, Or, Xor */
static void check_logical_binary_op(struct node *n) {
    struct node *l = get_child(n, 0), *r = get_child(n, 1);
    
    if (n->category == Xor) {
        if (l->annot_type == T_Int  && r->annot_type == T_Int) {
            n->annot_type = l->annot_type;
        } else {
            err_op_binary(n->line, n->col, get_op_str(n->category), l->annot_type, r->annot_type);
            n->annot_type = T_Int;
        }
    } else {   /* And, Or */
        if (l->annot_type != T_Bool || r->annot_type != T_Bool)
            err_op_binary(n->line, n->col, get_op_str(n->category), l->annot_type, r->annot_type);
        n->annot_type = T_Bool;
    }
}

/* Lshift, Rshift */
static void check_shift_op(struct node *n) {
    struct node *l = get_child(n, 0), *r = get_child(n, 1);
    
    // Eliminado el early return de T_Undef
    if (l->annot_type != T_Int || r->annot_type != T_Int) {
        err_op_binary(n->line, n->col, get_op_str(n->category), l->annot_type, r->annot_type);}
    n->annot_type = T_Int;
}

/* Unary Minus / Plus */
static void check_unary_numeric_op(struct node *n) {
    struct node *child = get_child(n, 0);
    
    // Eliminado el early return de T_Undef
    if (!is_numeric(child->annot_type)) {
        err_op_unary(n->line, n->col, get_op_str(n->category), child->annot_type);
        n->annot_type = T_Undef;
    } else {
        n->annot_type = child->annot_type;
    }
}

/* Not */
static void check_not_op(struct node *n) {
    struct node *child = get_child(n, 0);
    if (child->annot_type != T_Bool) // Quitamos el "&& != T_Undef" para que falle con undef
        err_op_unary(n->line, n->col, get_op_str(n->category), child->annot_type);
    n->annot_type = T_Bool;
}

/* IDENTIFIER = Expr */
static void check_assignment(struct node *n) {
    struct node *l = get_child(n, 0), *r = get_child(n, 1);
    
    // Arrays no se pueden asignar en este lenguaje
    if (l->annot_type == T_StringArray || r->annot_type == T_StringArray) {
        err_op_binary(n->line, n->col, "=", l->annot_type, r->annot_type);
    } 
    else if (!types_compatible(l->annot_type, r->annot_type)) {
        err_op_binary(n->line, n->col, "=", l->annot_type, r->annot_type);
    }
    
    n->annot_type = l->annot_type;
}

/* return [Expr] — validates against the method's declared return type. */
static void check_return(struct node *n, SymTable *local) {
    struct node *expr    = get_child(n, 0);
    Symbol      *ret_sym = search_symbol(local, "return");
    BasicType    expected = ret_sym ? ret_sym->type        : T_Void;
    BasicType    actual   = expr    ? expr->annot_type     : T_Void;

    if (expr && expected == T_Void)
        err_incompatible_type(expr->line, expr->col, actual, "return");
    else if (!expr && expected != T_Void)
        err_incompatible_void(n->line, n->col, "return");
    else if (expr && !types_compatible(expected, actual))
        err_incompatible_type(expr->line, expr->col, actual, "return");

    n->annot_type = T_None;
}

static void check_print(struct node *n) {
    struct node *child = get_child(n, 0);
    if (child) {
        if (child->annot_type == T_Void) {
            err_incompatible_void(child->line, child->col, "System.out.print");
        } else if (child->annot_type == T_StringArray || child->annot_type == T_Undef) {
            // Imprime error para String[] y undef
            err_incompatible_type(child->line, child->col, child->annot_type, "System.out.print");
        }
    }
    n->annot_type = T_None;
}

/* Integer.parseInt(id[expr]) — id must be String[], expr must be int. */
static void check_parse_args(struct node *n) {
    struct node *id_node   = get_child(n, 0);
    struct node *expr_node = get_child(n, 1);
    BasicType id_t   = id_node->annot_type;
    BasicType expr_t = expr_node->annot_type;

    if (id_t != T_StringArray || expr_t != T_Int)
        err_op_binary(n->line, n->col, "Integer.parseInt", id_t, expr_t);
    n->annot_type = T_Int;
}

/* id.length — id must be String[]. */
static void check_length(struct node *n) {
    struct node *child = get_child(n, 0);
    if (child->annot_type != T_StringArray)
        err_op_unary(n->line, n->col, ".length", child->annot_type);
    n->annot_type = T_Int;
}

/* Method call with overload resolution (spec §3.3):
 *   1. Exact match on count + types          → use it.
 *   2. Unique compatible match               → use it.
 *   3. Multiple compatible matches           → ambiguous error.
 *   4. No compatible match                   → cannot-find error. */
static void check_call(struct node *n, SymTable *global) {
    struct node *id_node = get_child(n, 0);
    const char *name = id_node->token;

    /* Collect actual argument types from children[1..] */
    int n_actual = 0;
    BasicType actual[64];
    for (int i = 1; n_actual < 64; i++) {
        struct node *arg = get_child(n, i);
        if (!arg) break;
        actual[n_actual++] = arg->annot_type;
    }

    Symbol *exact   = NULL;
    Symbol *compat  = NULL;
    int     n_compat = 0;

    for (Symbol *sym = global->first; sym && !exact; sym = sym->next) {
        if (!sym->params_list || strcmp(sym->name, name) != 0) continue;

        int n_formal;
        BasicType *formal = parse_formal_types(sym->params_list, &n_formal);
        if (n_formal != n_actual) { free(formal); continue; }

        int is_exact = 1, is_compat = 1;
        for (int i = 0; i < n_actual; i++) {
            if (formal[i] != actual[i])                    is_exact  = 0;
            if (!types_compatible(formal[i], actual[i]))   is_compat = 0;
        }
        free(formal);

        if      (is_exact)  { exact = sym; }
        else if (is_compat) { n_compat++; if (n_compat == 1) compat = sym; }
    }

    Symbol *chosen = exact ? exact : (n_compat == 1 ? compat : NULL);

    /* Construir string de parámetros para el reporte de errores */
    char params_str[256] = "(";
    for (int i = 0; i < n_actual; i++) {
        if (i > 0) strncat(params_str, ",", sizeof(params_str) - strlen(params_str) - 1);
        strncat(params_str, type_to_str(actual[i]), sizeof(params_str) - strlen(params_str) - 1);
    }
    strncat(params_str, ")", sizeof(params_str) - strlen(params_str) - 1);

    char full_name[512];
    snprintf(full_name, sizeof(full_name), "%s%s", name, params_str);

    if (chosen) {
        n->annot_type         = chosen->type;
        id_node->annot_type   = T_None;
        id_node->annot_params = chosen->params_list;
    } else if (n_compat > 1) {
        err_ambiguous(id_node->line, id_node->col, full_name);
        n->annot_type = id_node->annot_type = T_Undef;
    } else {
        err_cannot_find(id_node->line, id_node->col, full_name);
        n->annot_type = id_node->annot_type = T_Undef;
    }
}

/* If/While condition must be boolean. Called after children are processed. */
static void check_condition(struct node *n) {
    struct node *cond = get_child(n, 0);
    if (cond && cond->annot_type != T_Bool)
        err_incompatible_type(cond->line, cond->col, cond->annot_type,
                              n->category == If ? "if" : "while");
    n->annot_type = T_None;
}

/* ================================================================
 * NODE DISPATCH — post-order type resolution
 * Recurses into children before checking the parent node.
 * ================================================================ */

static void check_node(struct node *n, SymTable *global, SymTable *local) {
    if (!n) return;

    /* Post-order: process children first.
     * For Call, skip child[0] (the method-name Identifier) — it is
     * resolved by check_call after argument types are known. */
    struct node_list *c = n->children ? n->children->next : NULL;
    for (int idx = 0; c; c = c->next, idx++) {
        if (n->category == Call && idx == 0) continue;
        check_node(c->node, global, local);
    }

    switch (n->category) {
        case Natural: case Decimal: case BoolLit: case StrLit:
            check_literal(n);                      break;
        case Identifier:
            check_identifier(n, global, local);    break;
        case Add:  case Sub:  case Mul:  case Div:  case Mod:
            check_math_op(n);                      break;
        case Eq:   case Ne:   case Lt:   case Gt:   case Le:   case Ge:
            check_relational_op(n);                break;
        case And:  case Or:   case Xor:
            check_logical_binary_op(n);            break;
        case Not:
            check_not_op(n);                       break;
        case Lshift: case Rshift:
            check_shift_op(n);                     break;
        case Minus: case Plus:
            check_unary_numeric_op(n);             break;
        case Assign:
            check_assignment(n);                   break;
        case Return:
            check_return(n, local);                break;
        case Print:
            check_print(n);                        break;
        case ParseArgs:
            check_parse_args(n);                   break;
        case Length:
            check_length(n);                       break;
        case Call:
            check_call(n, global);                 break;
        default:
            n->annot_type = T_None;                break;
    }
}

/* ================================================================
 * PASS 1 — Register all global symbols (fields + method headers)
 *           before processing any method body, so forward calls work.
 * ================================================================ */

static int check_declaration_valid(const char *name, int line, int col,
                                   SymTable *table, const char *params) {
    if (!name) return 0;
    if (!strcmp(name, "_")) { 
        err_reserved(line, col); 
        return 1; 
    }
    for (Symbol *s = table->first; s; s = s->next) {
        if (strcmp(s->name, name) != 0) continue;
        if (!params && !s->params_list) {
            err_already_defined(line, col, name, NULL);  
            return 1;
        }
        if (params && s->params_list && !strcmp(s->params_list, params)) {
            err_already_defined(line, col, name, params); 
            return 1;
        }
    }
    return 0;
}

static void register_global_field(struct node *field) {
    struct node *type_node = get_child(field, 0);
    struct node *id_node   = get_identifier(field);
    if (!id_node) return;
    
    // Lo guardamos siempre y cuando no exista ya. Silencioso (print_errors = 0)
    if (!check_declaration_valid(id_node->token, id_node->line, id_node->col, global_table, NULL)) {
        insert_symbol(global_table, id_node->token, type_from_node(type_node), 0, NULL, id_node->line, id_node->col);
    }
}

static void register_method_header(struct node *method) {
    struct node *header    = get_child(method, 0);
    struct node *method_id = get_identifier(header);
    struct node *ret_type  = get_child(header, 0);
    struct node *params    = get_child(header, 2);
    if (!method_id || !method_id->token) return;

    char params_str[256];
    build_params_str(params, params_str, sizeof(params_str));

    SymTable *tmp_params = create_table("param_check");
    if (params) {
        for (struct node_list *c = params->children ? params->children->next : NULL;
             c; c = c->next) {
            if (!c->node) continue;
            struct node *id  = get_identifier(c->node);
            struct node *typ = get_child(c->node, 0);
            if (!id) continue;
            if (!check_declaration_valid(id->token, id->line, id->col, tmp_params, NULL))
                insert_symbol(tmp_params, id->token, type_from_node(typ),
                              1, NULL, id->line, id->col);
        }
    }

    // Silencioso (0)
    if (!check_declaration_valid(method_id->token, method_id->line, method_id->col, global_table, params_str)) {
        insert_symbol(global_table, method_id->token, type_from_node(ret_type), 0, params_str, method_id->line, method_id->col);
    } else {
        method_id->annot_type = T_Undef;
    }
}

void check_semantics_pass1(struct node *n) {
    struct node *class_id = get_identifier(n);
    char class_name[256];
    snprintf(class_name, sizeof(class_name), "Class %s",
             (class_id && class_id->token) ? class_id->token : "Unknown");

    global_table  = create_table(class_name);
    current_table = global_table;

    for (struct node_list *cur = n->children; cur; cur = cur->next) {
        if (!cur->node) continue;
        if      (cur->node->category == FieldDecl)  register_global_field(cur->node);
        else if (cur->node->category == MethodDecl) register_method_header(cur->node);
    }
}

/* ================================================================
 * PASS 2 — Build local symbol tables and type-check method bodies.
 * ================================================================ */

static void process_method_body(struct node *method) {
    struct node *header   = get_child(method, 0);
    struct node *body     = get_child(method, 1);

    struct node *ret_type  = get_child(header, 0);
    struct node *method_id = get_identifier(header);
    struct node *params    = get_child(header, 2);
    if (!method_id || !method_id->token) return;

    char params_str[256];
    build_params_str(params, params_str, sizeof(params_str));

    char title[512];
    snprintf(title, sizeof(title), "Method %s%s", method_id->token, params_str);

    SymTable *method_table = create_table(title);
    SymTable *tmp = global_table;
    while (tmp->next) tmp = tmp->next;
    tmp->next = method_table;

    /* "return" pseudo-symbol stores the expected return type for this method */
    insert_symbol(method_table, "return", type_from_node(ret_type), 0, NULL, 0, 0);

    SymTable *saved = current_table;
    current_table   = method_table;

    check_semantics_pass2(params);   /* register formal parameters */
    check_semantics_pass2(body);     /* type-check the body        */

    current_table = saved;
}

void check_semantics_pass2(struct node *n) {
    if (!n) return;

    switch (n->category) {

        case Program:
            for (struct node_list *cur = n->children; cur; cur = cur->next)
                if (cur->node && cur->node->category != Identifier)
                    check_semantics_pass2(cur->node);
            break;

        case ParamDecl: {
            struct node *type_node = get_child(n, 0);
            struct node *id_node   = get_identifier(n);
            if (!check_declaration_valid(id_node->token, id_node->line, id_node->col,
                                         current_table, NULL))
                insert_symbol(current_table, id_node->token, type_from_node(type_node),
                              1, NULL, id_node->line, id_node->col);
            break;
        }

        case VarDecl: {
            struct node *type_node = get_child(n, 0);
            struct node *id_node   = get_identifier(n);
            if (!type_node || !id_node || !id_node->token) break;
            if (!check_declaration_valid(id_node->token, id_node->line, id_node->col,
                                         current_table, NULL))
                insert_symbol(current_table, id_node->token, type_from_node(type_node),
                              0, NULL, id_node->line, id_node->col);
            break;
        }

        case FieldDecl: {
            break;
        }

        case MethodDecl: {
            struct node *header = get_child(n, 0);
            struct node *method_id = get_identifier(header);
            /* Si Pass 1 NO lo marcó como inválido, procesamos su interior */
            if (method_id && method_id->annot_type != T_Undef) {
                process_method_body(n);
            } else if (method_id && method_id->annot_type == T_Undef) {
                method_id->annot_type = T_None; 
            }
            break;
        }

        case If:
        case While:
            for (struct node_list *cur = n->children ? n->children->next : NULL;
                 cur; cur = cur->next)
                check_semantics_pass2(cur->node);
            check_condition(n);
            break;

        /* Expressions and expression-statements: delegate to check_node */
        case Assign:
        case Add:  case Sub:  case Mul:  case Div:  case Mod:
        case Eq:   case Ne:   case Lt:   case Gt:   case Le:  case Ge:
        case And:  case Or:   case Xor:  case Not:
        case Lshift: case Rshift:
        case Minus: case Plus:
        case Identifier: case Natural: case Decimal: case BoolLit:
        case Call: case Return: case Print: case Length: case ParseArgs:
            check_node(n, global_table, current_table);
            break;

        /* Structural nodes (Block, MethodBody, MethodParams, …): just recurse */
        default:
            for (struct node_list *cur = n->children; cur; cur = cur->next)
                check_semantics_pass2(cur->node);
            break;
    }
}

/* ================================================================
 * ENTRY POINT
 * ================================================================ */

void check_semantics(struct node *n) {
    check_semantics_pass1(n); 
    check_semantics_pass2(n); 
    print_semantic_errors();
}