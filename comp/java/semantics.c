/* semantic.c — Semantic analysis for the Juc compiler */

#include "ast.h"
#include "y.tab.h"
#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "sym_table.h"

SymTable *global_table  = NULL;
SymTable *current_table = NULL;
int semantic_errors=0;

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
    char *msg;
    struct ErrorNode *next;
} ErrorNode;

static ErrorNode *error_list_head = NULL;

/* Inserta el error ordenado por línea y columna. Ignora duplicados exactos. */
static void add_error(int line, int col, const char *msg) {
    for (ErrorNode *cur = error_list_head; cur; cur = cur->next)
        if (cur->line == line && cur->col == col && strcmp(cur->msg, msg) == 0)
            return;
    semantic_errors++;
    ErrorNode *new_err = malloc(sizeof(ErrorNode));
    new_err->line = line;
    new_err->col  = col;
    new_err->msg = (char*)malloc(strlen(msg) + 1);
    strcpy(new_err->msg, msg);

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
        free(temp->msg);
        free(temp);
    }
    error_list_head = NULL;
}
//Strings dinamicos
static void report_error_fmt(int line, int col, const char *fmt,...) {
    va_list args;
    va_start(args, fmt);
    int len = vsnprintf(NULL, 0, fmt, args) + 1;
    va_end(args);

    char *buf = (char*)malloc(len);
    
    va_start(args, fmt);
    vsnprintf(buf, len, fmt, args);
    va_end(args);

    add_error(line, col, buf);
    free(buf);
}

static void err_already_defined(int line, int col, const char *name, const char *params) {
    if (params) report_error_fmt(line, col, "Line %d, col %d: Symbol %s%s already defined\n", line, col, name, params);
    else        report_error_fmt(line, col, "Line %d, col %d: Symbol %s already defined\n", line, col, name);
}

static void err_reserved(int line, int col) {
    report_error_fmt(line, col, "Line %d, col %d: Symbol _ is reserved\n", line, col);
}

static void err_cannot_find(int line, int col, const char *name) {
    report_error_fmt(line, col, "Line %d, col %d: Cannot find symbol %s\n", line, col, name);
}

static void err_op_unary(int line, int col, const char *op, BasicType t) {
    report_error_fmt(line, col, "Line %d, col %d: Operator %s cannot be applied to type %s\n", line, col, op, type_to_str(t));
}

static void err_op_binary(int line, int col, const char *op, BasicType t1, BasicType t2) {
    report_error_fmt(line, col, "Line %d, col %d: Operator %s cannot be applied to types %s, %s\n", line, col, op, type_to_str(t1), type_to_str(t2));
}

static void err_incompatible_type(int line, int col, BasicType t, const char *stmt) {
    report_error_fmt(line, col, "Line %d, col %d: Incompatible type %s in %s statement\n", line, col, type_to_str(t), stmt);
}

static void err_incompatible_void(int line, int col, const char *stmt) {
    report_error_fmt(line, col, "Line %d, col %d: Incompatible type void in %s statement\n", line, col, stmt);
}

static void err_number_out_of_bounds(int line, int col, const char *token) {
    report_error_fmt(line, col, "Line %d, col %d: Number %s out of bounds\n", line, col, token);
}

static void err_ambiguous(int line, int col, const char *name) {
    report_error_fmt(line, col, "Line %d, col %d: Reference to method %s is ambiguous\n", line, col, name);
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
BasicType type_from_node(struct node *n) {
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

ParamType *build_params_list(struct node *params_node) {
    ParamType *head = NULL, *tail = NULL;
    if (!params_node) return NULL;
    struct node *child;
    for (int i = 0; (child = get_child(params_node, i)) != NULL; i++) {
        ParamType *p = malloc(sizeof(ParamType));
        p->type = type_from_node(get_child(child, 0));
        p->next = NULL;
        if (!head) head = tail = p;
        else { tail->next = p; tail = p; }
    }
    return head;
}

/* ================================================================
 * INDIVIDUAL NODE CHECKS
 * Each function assumes its children have already been annotated.
 * ================================================================ */

static void check_literal(struct node *n) {

    char *stripped = (char*)malloc(strlen(n->token) + 1);
    int j = 0;
    
    // Limpiamos los '_' dinámicamente
    for (int k = 0; n->token[k]; k++) {
        if (n->token[k] != '_') stripped[j++] = n->token[k];
    }
    stripped[j] = '\0';

    switch (n->category) {
        case Natural: {
            errno = 0;
            char *endptr;
            long long val = strtoll(stripped, &endptr, 10);
            if (errno == ERANGE || val > 2147483647LL || val < 0)
                err_number_out_of_bounds(n->line, n->col, n->token);
            n->annot_type = T_Int;
            break;
        }
        case Decimal: {
            errno = 0;
            double val = strtod(stripped, NULL);
            if (isinf(val) || (val == 0.0 && errno == ERANGE)) 
                err_number_out_of_bounds(n->line, n->col, n->token);
            n->annot_type = T_Double;
            break;
        }
        case BoolLit: n->annot_type = T_Bool;   break;
        case StrLit:  n->annot_type = T_String; break;
        default:      n->annot_type = T_Undef;  break;
    }
    free(stripped);
}

static void check_identifier(struct node *n, SymTable *global, SymTable *local) {
    Symbol *sym = NULL;

    if (local) {
        for (Symbol *s = local->first; s; s = s->next) {
            // Ignoramos métodos activamente para encontrar la variable
            if (s->kind != SYM_METHOD && strcmp(s->name, n->token) == 0) { 
                sym = s; 
                break; 
            }
        }
    }

    if (!sym && global) {
        for (Symbol *s = global->first; s; s = s->next) {
            if (s->kind != SYM_METHOD && strcmp(s->name, n->token) == 0) { 
                sym = s; 
                break; 
            }
        }
    }

    if (!sym) { 
        err_cannot_find(n->line, n->col, n->token); 
        n->annot_type = T_Undef; 
    } else {        
        n->annot_type = sym->type;
    }
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
        if ((l->annot_type == r->annot_type && l->annot_type != T_StringArray && l->annot_type != T_Undef && l->annot_type != T_Void) ||
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
    if (l->annot_type != T_Bool || r->annot_type != T_Bool) {
        err_op_binary(n->line, n->col, get_op_str(n->category), l->annot_type, r->annot_type);
    }
    n->annot_type = T_Bool;
}
/* Lshift, Rshift, Xor */
static void check_bitwise_op(struct node *n) {
    struct node *l = get_child(n, 0), *r = get_child(n, 1);
    if (n->category == Xor){
        if(l->annot_type != T_Bool || r->annot_type != T_Bool){
                err_op_binary(n->line, n->col, get_op_str(n->category), l->annot_type, r->annot_type);
        }
        if(l->annot_type == T_Bool || r->annot_type == T_Bool){
            n->annot_type = T_Bool;
        }
        return;
    }
    if (l->annot_type != T_Int || r->annot_type != T_Int) {
        err_op_binary(n->line, n->col, get_op_str(n->category), l->annot_type, r->annot_type);
    }
    n->annot_type = T_Int; // Falla o no, asume T_Int para evitar cascadas
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
    if (!id_node || !id_node->token) return;
    const char *name = id_node->token;

    /* 1. Construir lista enlazada temporal de parámetros reales para poder usar las utilidades */
    ParamType *actual_list = NULL, *actual_tail = NULL;
    
    for (int i = 1; ; i++) {
        struct node *arg = get_child(n, i);
        if (!arg) break;
        
        ParamType *p = malloc(sizeof(ParamType));
        p->type = arg->annot_type;
        p->next = NULL;
        
        if (!actual_list) actual_list = actual_tail = p;
        else { actual_tail->next = p; actual_tail = p; }
    }

    /* 2. Resolución de sobrecarga: Se mantiene el bucle porque 'search_exact_method' no evalúa tipos compatibles (promoción implícita) */
    Symbol *exact   = NULL;
    Symbol *compat  = NULL;
    int     n_compat = 0;

    for (Symbol *sym = global->first; sym && !exact; sym = sym->next) {
        if (sym->kind != SYM_METHOD || strcmp(sym->name, name) != 0) continue;

        int is_exact = 1, is_compat = 1;
        ParamType *p_formal = sym->params;
        ParamType *p_actual = actual_list;
        
        while (p_formal && p_actual) {
            if (p_formal->type != p_actual->type)                  is_exact  = 0;
            if (!types_compatible(p_formal->type, p_actual->type)) is_compat = 0;
            p_formal = p_formal->next;
            p_actual = p_actual->next;
        }
        if (p_formal || p_actual) is_exact = is_compat = 0;

        if      (is_exact)  { exact = sym; }
        else if (is_compat) { n_compat++; if (n_compat == 1) compat = sym; }
    }

    Symbol *chosen = exact ? exact : (n_compat == 1 ? compat : NULL);

    /* 3. Utilizar la utilidad 'params_to_str' para generar la firma (elimina buffer overflows de strncat) */
    char *params_str = actual_list ? params_to_str(actual_list) : strdup("()");

    int name_len = snprintf(NULL, 0, "%s%s", name, params_str) + 1;
    char *full_name = (char*)malloc(name_len);
    snprintf(full_name, name_len, "%s%s", name, params_str);

    if (chosen) {
        n->annot_type         = chosen->type;
        id_node->annot_type   = T_None;
        
        id_node->annot_params = chosen->params ? params_to_str(chosen->params) : strdup("()");
    } else if (n_compat > 1) {
        err_ambiguous(id_node->line, id_node->col, full_name);
        n->annot_type = T_Undef;
        id_node->annot_type = T_Undef;
    } else {
        err_cannot_find(id_node->line, id_node->col, full_name);
        n->annot_type = T_Undef;
        id_node->annot_type = T_Undef;
    }
    free(params_str);
    free(full_name);

    /* 4. Liberar la lista temporal de parámetros reales */
    ParamType *curr = actual_list;
    while (curr) {
        ParamType *next = curr->next;
        free(curr);
        curr = next;
    }
}

/* If/While condition must be boolean. Called after children are processed. */
static void check_condition(struct node *n) {
    struct node *cond = get_child(n, 0);
    
    if (cond && cond->annot_type != T_Bool) {
        err_incompatible_type(cond->line, cond->col, cond->annot_type,
                              n->category == If ? "if" : "while");
    }
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
    struct node *child;
    for (int idx = 0; (child = get_child(n, idx)) != NULL; idx++) {
        if (n->category == Call && idx == 0) continue;
        check_node(child, global, local);
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
        case And:  case Or:   
            check_logical_binary_op(n);            break;
        case Not:
            check_not_op(n);                       break;
        case Lshift: case Rshift: case Xor:
            check_bitwise_op(n);                   break;
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
                                   SymTable *table, SymbolKind kind, ParamType *params) {
    if (!strcmp(name, "_")) { 
        err_reserved(line, col);
        return 1;
    }
    if (kind != SYM_METHOD) {
        for (Symbol *s = table->first; s; s = s->next) {
            if (s->kind != SYM_METHOD && strcmp(s->name, name) == 0) {
                err_already_defined(line, col, name, NULL);
                return 1;
            }
        }
        return 0;
    } else {
        for (Symbol *s = table->first; s; s = s->next) {
            if (s->kind == SYM_METHOD && strcmp(s->name, name) == 0) {
                int is_match = 1;
                ParamType *p1 = s->params;
                ParamType *p2 = params;
                
                while (p1 && p2) {
                    if (p1->type != p2->type) { is_match = 0; break; }
                    p1 = p1->next;
                    p2 = p2->next;
                }
                if (p1 || p2) is_match = 0;

                if (is_match) {
                    char *pbuf = params_to_str(params);
                    err_already_defined(line, col, name, pbuf);
                    free(pbuf);
                    return 1;
                }
            }
        }
        return 0;
    }
    
}

static void register_global_field(struct node *FieldDecl) {
    struct node *type_node = get_child(FieldDecl, 0);
    struct node *id_node   = get_child(FieldDecl, 1);  

    if (!check_declaration_valid(id_node->token, id_node->line, id_node->col, global_table,SYM_FIELD, NULL)) {
        insert_symbol(global_table, id_node->token, type_from_node(type_node), SYM_FIELD, NULL);
    }
}

static void register_method_header(struct node *method) {
    struct node *header    = get_child(method, 0);
    struct node *ret_type  = get_child(header, 0);
    struct node *method_id = get_child(header, 1);
    struct node *params    = get_child(header, 2);

    ParamType *params_list = build_params_list(params);
    //char params_str[512];
    //params_to_str(params_list, params_str, sizeof(params_str));
    
    SymTable *tmp_params = create_table("param_check");
    if (params) {
        struct node *param_child;
        for (int i = 0; (param_child = get_child(params, i)) != NULL; i++) {
            struct node *typ = get_child(param_child, 0);   
            struct node *id  = get_child(param_child, 1);
            if (!id || !id->token) continue;
            
            if (!check_declaration_valid(id->token, id->line, id->col, tmp_params, SYM_PARAM, NULL)) {
                insert_symbol(tmp_params, id->token, type_from_node(typ), SYM_PARAM, NULL);
            }
        }
    }

    int is_method_valid = !check_declaration_valid(method_id->token, method_id->line, method_id->col, global_table, SYM_METHOD, params_list);

    if (is_method_valid) {
        insert_symbol(global_table, method_id->token, type_from_node(ret_type), SYM_METHOD, params_list);
        Symbol *method_sym = search_exact_method(global_table, method_id->token, params_list);
        
        if (method_sym != NULL) {
            char *pbuf = params_to_str(params_list);
            int title_len = snprintf(NULL, 0, "Method %s%s", method_id->token, pbuf) + 1;
            char *title = (char*)malloc(title_len);
            snprintf(title, title_len, "Method %s%s", method_id->token, pbuf);
            
            method_sym->nested_table = create_table(title);

            free(title);
            free(pbuf);
            
            insert_symbol(method_sym->nested_table, "return", type_from_node(ret_type), SYM_RETURN, NULL);
            // Copiar los parámetros válidos desde tmp_params a la tabla del método
            for (Symbol *s = tmp_params->first; s; s = s->next) {
                insert_symbol(method_sym->nested_table, s->name, s->type, SYM_PARAM, NULL);
            }
        }
    } else {
        method->annot_type = T_Undef;
    }

    Symbol *curr = tmp_params->first;
    while (curr) {
        Symbol *next = curr->next;
        free(curr);
        curr = next;
    }
    free(tmp_params->title);
    free(tmp_params);
}

void check_semantics_pass1(struct node *program) {
    struct node *class_id = get_child(program,0);


    const char *token_str = class_id->token;
    int len = snprintf(NULL, 0, "Class %s", token_str) + 1;
    
    char *class_name = (char*)malloc(len);
    snprintf(class_name, len, "Class %s", token_str);

    global_table  = create_table(class_name);
    current_table = global_table;

    free(class_name);

    struct node *child;
    for (int i = 1; (child = get_child(program, i)) != NULL; i++) {
        if      (child->category == FieldDecl)  register_global_field(child);
        else if (child->category == MethodDecl) register_method_header(child);
    }
}

/* ================================================================
 * PASS 2 — Build local symbol tables and type-check method bodies.
 * ================================================================ */

static void process_method_body(struct node *method) {
    struct node *header   = get_child(method, 0);
    struct node *body     = get_child(method, 1);

    struct node *method_id = get_child(header, 1);
    struct node *params    = get_child(header, 2);

    ParamType *params_list = build_params_list(params);

    Symbol *method_sym = search_exact_method(global_table, method_id->token, params_list);
    
    if (!method_sym || !method_sym->nested_table) return;

    SymTable *saved = current_table;

    current_table = method_sym->nested_table;

    check_semantics_pass2(body);

    current_table = saved;

    ParamType *curr = params_list;
    while (curr) {
        ParamType *next = curr->next;
        free(curr);
        curr = next;
    }
}

void check_semantics_pass2(struct node *n) {
    if (!n) return;

    switch (n->category) {

        case Program: {
            struct node *child;
            //Lanzamos para methods y declaraciones
            for (int i = 1; (child = get_child(n, i)) != NULL; i++) {
                check_semantics_pass2(child);
            }
            break;
        }
        case ParamDecl: {
            break;
        }

        case VarDecl: {
            struct node *type_node = get_child(n, 0);
            struct node *id_node   = get_child(n, 1);
            if (!check_declaration_valid(id_node->token, id_node->line, id_node->col, current_table, SYM_LOCAL,NULL))
                insert_symbol(current_table, id_node->token, type_from_node(type_node),
                              SYM_LOCAL, NULL);
            break;
        }
        case FieldDecl: {
            break;
        }
        case MethodDecl: {
            /* Si Pass 1 NO lo marcó como inválido, procesamos su interior */
            if (n->annot_type != T_Undef) {
                process_method_body(n);
            }
                n->annot_type = T_None;
            break;
        }

        case If:
        case While:{
            struct node *child;
            for (int i = 0; (child = get_child(n, i)) != NULL; i++) {
                check_semantics_pass2(child);
            }
            check_condition(n);
            break;
        }

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
        default:{
            struct node *child;
            for (int i = 0; (child = get_child(n, i)) != NULL; i++) {
                check_semantics_pass2(child);
            }
            break;
        }
    }
}

/* ================================================================
 * ENTRY POINT
 * ================================================================ */

void check_semantics(struct node *ast) {
    check_semantics_pass1(ast); 
    check_semantics_pass2(ast); 
    print_semantic_errors();
}