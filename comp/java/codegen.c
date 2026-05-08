#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "codegen.h"
#include "semantics.h"
#include "sym_table.h"
#include "ast.h"

/* =========================================================================
 * Globals
 * ========================================================================= */
static int tmp_counter = 1;   /* SSA register / label counter, reset per method */
static BasicType current_ret_type = T_Void;
static int main_emitted = 0;

/* =========================================================================
 * Type helpers
 * ========================================================================= */


static const char *type_to_llvm(BasicType t) {
    switch (t) {
        case T_Int:         return "i32";
        case T_Double:      return "double";
        case T_Bool:        return "i1";
        case T_StringArray: return "i8**";
        case T_Void:        return "void";
        default:            return "i32";
    }
}

static const char *default_val_llvm(BasicType t) {
    switch (t) {
        case T_Double: return "0.0";
        default:       return "0";
    }
}


static void build_mangled_suffix(BasicType *types, int n, char *out) {
    out[0] = '\0';
    for (int i = 0; i < n; i++) {
        switch (types[i]) {
            case T_Int:    strcat(out, "_i"); break;
            case T_Double: strcat(out, "_d"); break;
            case T_Bool:   strcat(out, "_b"); break;
            default:       strcat(out, "_x"); break;
        }
    }
}

/* =========================================================================
 * Numeric literal cleaning  (Juc allows 1_000_000 style underscores)
 * ========================================================================= */
static void strip_underscores(const char *in, char *out) {
    for (; *in; in++)
        if (*in != '_') *out++ = *in;
    *out = '\0';
}

/* =========================================================================
 * String literal registry
 *   Juc STRLIT tokens (with surrounding quotes) are collected in a pre-pass
 *   so that their global LLVM constants can be emitted before the functions.
 * ========================================================================= */
#define MAX_STRLITS 256

static char *strlit_tokens[MAX_STRLITS];
static char  strlit_llvm_content[MAX_STRLITS][1024];  /* LLVM c"..." body  */
static int   strlit_byte_count[MAX_STRLITS];           /* bytes incl. \00   */
static int   strlit_count = 0;


 static void free_strlits(void) {
    for (int i = 0; i < strlit_count; i++) {
        free(strlit_tokens[i]);
    }
}
/*
 * Convert a Juc STRLIT token (with surrounding quotes) to an LLVM constant
 * string body.  Juc escape sequences are translated to \XX hex escapes that
 * LLVM's assembler understands.  A null terminator \00 is appended.
 * Returns the total byte count (string length + 1 for '\0').
 */
static int juc_strlit_to_llvm(const char *token, char *out) {
    const char *p = token + 1;  /* skip opening '"' */
    char *q = out;
    int count = 0;

    while (*p && *p != '"') {
        unsigned char c;
        if (*p == '\\' && *(p + 1)) {
            p++;
            switch (*p) {
                case 'f':  c = '\f'; break;
                case 'n':  c = '\n'; break;
                case 'r':  c = '\r'; break;
                case 't':  c = '\t'; break;
                case '\\': c = '\\'; break;
                case '"':  c = '"';  break;
                default:   c = (unsigned char)*p; break;
            }
        } else {
            c = (unsigned char)*p;
        }

        /* Non-printable, backslash and quote → \XX */
        if (c < 32 || c == '"' || c == '\\' || c > 126) {
            sprintf(q, "\\%02X", c);
            q += 3;
        } else {
            *q++ = c;
        }
        count++;
        p++;
    }

    /* Null terminator */
    strcpy(q, "\\00");
    count++;   /* the \0 byte */
    return count;
}

/* Returns the index of this literal in the registry (adds if new). */
static int register_strlit(const char *token) {
    int i;
    for (i = 0; i < strlit_count; i++)
        if (strcmp(strlit_tokens[i], token) == 0) return i;
    if (strlit_count >= MAX_STRLITS) return 0;
    strlit_tokens[strlit_count] = strdup(token);
    strlit_byte_count[strlit_count] =
        juc_strlit_to_llvm(token, strlit_llvm_content[strlit_count]);
    return strlit_count++;
}

/* DFS pre-scan of the entire AST to collect every StrLit token. */
static void collect_strlits(struct node *n) {
    if (!n) return;
    if (n->category == StrLit && n->token)
        register_strlit(n->token);
    for (struct node_list *c = n->children; c; c = c->next)
        collect_strlits(c->node);
}

/* Emit global string-literal constants (call before method codegen). */
static void emit_strlits(void) {
    int i;
    for (i = 0; i < strlit_count; i++)
        printf("@.str.lit.%d = private unnamed_addr constant [%d x i8] c\"%s\"\n",
               i, strlit_byte_count[i], strlit_llvm_content[i]);
    if (strlit_count > 0) printf("\n");
}

/* =========================================================================
 * Variable access prefix  (local → %, global → @_)
 * ========================================================================= */
static const char *get_var_prefix(SymTable *global, SymTable *local,
                                   const char *name) {
    if (local) {
        Symbol *s;
        for (s = local->first; s; s = s->next)
            if (strcmp(s->name, name) == 0) return "%";
    }
    return "@_";
}

/* =========================================================================
 * Type coercion helper
 *   Emits a conversion instruction when the register type does not match the
 *   target type, and returns the (possibly new) register holding the result.
 * ========================================================================= */
static int coerce_to(int reg, BasicType from, BasicType to) {
    if (from == to || from == T_Undef || to == T_Undef) return reg;
    if (from == T_Int && to == T_Double) {
        int r = tmp_counter++;
        printf("  %%%d = sitofp i32 %%%d to double\n", r, reg);
        return r;
    }
    if (from == T_Double && to == T_Int) {
        int r = tmp_counter++;
        printf("  %%%d = fptosi double %%%d to i32\n", r, reg);
        return r;
    }
    if (from == T_Bool && to == T_Int) {
        int r = tmp_counter++;
        printf("  %%%d = zext i1 %%%d to i32\n", r, reg);
        return r;
    }
    return reg;
}

static int codegen_statement(struct node *stmt, SymTable *global,
                              SymTable *local);

/* =========================================================================
 * ExprGen
 * ========================================================================= */
static int codegen_expression(struct node *expr, SymTable *global,
                               SymTable *local) {
    if (!expr) return -1;

    switch (expr->category) {

        /* ---- Literals ---- */

        case Natural: {
            char clean[256];
            strip_underscores(expr->token, clean);
            int r = tmp_counter++;
            printf("  %%%d = add i32 0, %s\n", r, clean);
            return r;
        }

case Decimal: {
    char clean[64];
    strip_underscores(expr->token, clean);

    if (clean[0] == '.') {
        memmove(clean + 1, clean, strlen(clean) + 1);
        clean[0] = '0';
    }
    char *e_pos = strpbrk(clean, "eE");
    if (e_pos && !memchr(clean, '.', e_pos - clean)) {
        memmove(e_pos + 2, e_pos, strlen(e_pos) + 1);
        e_pos[0] = '.';
        e_pos[1] = '0';
    }

    int r = tmp_counter++;
    printf("  %%%d = fadd double 0.0, %s\n", r, clean);
    return r;
}

        case BoolLit: {
            int r = tmp_counter++;
            printf("  %%%d = add i1 0, %d\n", r,
                   strcmp(expr->token, "true") == 0 ? 1 : 0);
            return r;
        }

        case Identifier: {
            int r = tmp_counter++;
            const char *pfx = get_var_prefix(global, local, expr->token);
            const char *lt  = type_to_llvm(expr->annot_type);
            printf("  %%%d = load %s, %s* %s%s\n", r, lt, lt, pfx,
                   expr->token);
            return r;
        }

        /*
         * StrLit appearing as a standalone expression (e.g. inside Print).
         * Returns a pointer to the first byte of the global string constant.
         */
        case StrLit: {
            int idx = register_strlit(expr->token);
            int r   = tmp_counter++;
            printf("  %%%d = getelementptr inbounds [%d x i8], [%d x i8]* @.str.lit.%d, i32 0, i32 0\n",
                   r, strlit_byte_count[idx], strlit_byte_count[idx], idx);
            return r;
        }

        /* ---- Arithmetic binary operators ---- */

        case Add: case Sub: case Mul: case Div: case Mod: {
            struct node *c0 = get_child(expr, 0);
            struct node *c1 = get_child(expr, 1);
            BasicType rt = expr->annot_type;
            int t1 = coerce_to(codegen_expression(c0, global, local),
                                c0->annot_type, rt);
            int t2 = coerce_to(codegen_expression(c1, global, local),
                                c1->annot_type, rt);
            int r   = tmp_counter++;
            int flt = (rt == T_Double);
            const char *op =
                expr->category == Add ? (flt ? "fadd" : "add") :
                expr->category == Sub ? (flt ? "fsub" : "sub") :
                expr->category == Mul ? (flt ? "fmul" : "mul") :
                expr->category == Div ? (flt ? "fdiv" : "sdiv") :
                                        (flt ? "frem" : "srem");
            printf("  %%%d = %s %s %%%d, %%%d\n", r, op,
                   type_to_llvm(rt), t1, t2);
            return r;
        }

        /* ---- Bitwise / logical binary operators ---- */

        case And: case Or: case Xor: case Lshift: case Rshift: {
            struct node *c0 = get_child(expr, 0);
            struct node *c1 = get_child(expr, 1);
            int t1 = codegen_expression(c0, global, local);
            int t2 = codegen_expression(c1, global, local);
            int r   = tmp_counter++;
            const char *op =
                expr->category == And    ? "and"  :
                expr->category == Or     ? "or"   :
                expr->category == Xor    ? "xor"  :
                expr->category == Lshift ? "shl"  :
                                           "ashr";
            printf("  %%%d = %s %s %%%d, %%%d\n", r, op,
                   type_to_llvm(expr->annot_type), t1, t2);
            return r;
        }

        /* ---- Relational binary operators ---- */

        case Eq: case Ne: case Lt: case Gt: case Le: case Ge: {
            struct node *c0 = get_child(expr, 0);
            struct node *c1 = get_child(expr, 1);
            BasicType op_type = c0->annot_type;
            int t1 = codegen_expression(c0, global, local);
            int t2 = codegen_expression(c1, global, local);

            /* Promote int operand to double when mixed */
            if (c0->annot_type == T_Int && c1->annot_type == T_Double) {
                t1 = coerce_to(t1, T_Int, T_Double);
                op_type = T_Double;
            } else if (c0->annot_type == T_Double && c1->annot_type == T_Int) {
                t2 = coerce_to(t2, T_Int, T_Double);
                op_type = T_Double;
            }

            int flt = (op_type == T_Double);
            const char *pred;
            if (flt) {
                pred = expr->category == Eq ? "oeq" :
                       expr->category == Ne ? "one" :
                       expr->category == Lt ? "olt" :
                       expr->category == Gt ? "ogt" :
                       expr->category == Le ? "ole" : "oge";
            } else {
                pred = expr->category == Eq ? "eq"  :
                       expr->category == Ne ? "ne"  :
                       expr->category == Lt ? "slt" :
                       expr->category == Gt ? "sgt" :
                       expr->category == Le ? "sle" : "sge";
            }

            int r = tmp_counter++;
            printf("  %%%d = %s %s %s %%%d, %%%d\n",
                   r, flt ? "fcmp" : "icmp", pred,
                   type_to_llvm(op_type), t1, t2);
            return r;
        }

        /* ---- Unary operators ---- */

        case Not: {
            int t = codegen_expression(get_child(expr, 0), global, local);
            int r = tmp_counter++;
            printf("  %%%d = xor i1 %%%d, 1\n", r, t);
            return r;
        }

        case Minus: {
            struct node *c0 = get_child(expr, 0);
            int t = codegen_expression(c0, global, local);
            int r = tmp_counter++;
            if (expr->annot_type == T_Double)
                printf("  %%%d = fneg double %%%d\n", r, t);
            else
                printf("  %%%d = sub i32 0, %%%d\n", r, t);
            return r;
        }

        case Plus: {
            /* Unary plus is a no-op */
            return codegen_expression(get_child(expr, 0), global, local);
        }

        /* ---- Assignment ---- */

        case Assign: {
            struct node *id_node  = get_child(expr, 0);
            struct node *val_node = get_child(expr, 1);
            int val_r = coerce_to(codegen_expression(val_node, global, local),
                                   val_node->annot_type,
                                   id_node->annot_type);
            const char *pfx = get_var_prefix(global, local, id_node->token);
            const char *lt  = type_to_llvm(id_node->annot_type);
            printf("  store %s %%%d, %s* %s%s\n",
                   lt, val_r, lt, pfx, id_node->token);
            /* In Java/Juc, assignment is also an expression → return value */
            int r = tmp_counter++;
            if (id_node->annot_type == T_Double)
    printf("  %%%d = fadd double 0.0, %%%d\n", r, val_r);
else
    printf("  %%%d = add %s 0, %%%d\n", r, lt, val_r);
            return r;
        }

        /* ---- Method call ---- */
        case Call: {
            struct node *name_node = get_identifier(expr);
            if (!name_node) return -1;

            int arg_regs[64];
            BasicType arg_types[64];
            BasicType param_types[64];
            int nargs = 0;
            struct node *a;
            
            /* El índice 0 es el nombre de la función, los args empiezan en el 1 */
            while ((a = get_child(expr, nargs + 1)) != NULL && nargs < 64) {
                arg_regs[nargs]  = codegen_expression(a, global, local);
                arg_types[nargs] = a->annot_type;
                nargs++;
            }

            SymTable *method_table = search_table_name(global, name_node->token);
                int n_params = 0;
                for (Symbol *s = method_table->first; s != NULL; s = s->next) {
                    if (s->kind==SYM_PARAM) {
                        param_types[n_params++] = s->type;
                    }
                }
            BasicType ret = expr->annot_type;
            int r=-1;

            int final_arg_regs[64];
            
            for (int i = 0; i < nargs; i++) {
                final_arg_regs[i] = arg_regs[i]; 

                if (arg_types[i] == T_Int && param_types[i] == T_Double) {
                    final_arg_regs[i] = tmp_counter++;
                    printf("  %%%d = sitofp i32 %%%d to double\n", final_arg_regs[i], arg_regs[i]);
                }
            }

            char mangled[256];
            build_mangled_suffix(param_types, nargs, mangled);
            if (ret == T_Void) {
                printf("  call void @_%s%s(", name_node->token, mangled);
            } else {
                r = tmp_counter++;
                printf("  %%%d = call %s @_%s%s(", r, type_to_llvm(ret), name_node->token, mangled);
            }
            
            for (int i = 0; i < nargs; i++) {
                if (i > 0) printf(", ");
                printf("%s %%%d", type_to_llvm(param_types[i]), final_arg_regs[i]);
            }
            printf(")\n");
            return r;
        }

        /* ---- Integer.parseInt(args[expr]) ---- */

        case ParseArgs: {
            struct node *id_node  = get_child(expr, 0);
            struct node *idx_expr = get_child(expr, 1);

            int idx_r = codegen_expression(idx_expr, global, local);
            int real_idx_r = tmp_counter++;
            printf("  %%%d = add i32 %%%d, 1\n", real_idx_r, idx_r);

            const char *pfx = get_var_prefix(global, local, id_node->token);

            int arr_r  = tmp_counter++;
            printf("  %%%d = load i8**, i8*** %s%s\n", arr_r, pfx, id_node->token);

            int eptr_r = tmp_counter++;
            printf("  %%%d = getelementptr i8*, i8** %%%d, i32 %%%d\n", eptr_r, arr_r, real_idx_r);

            int elem_r = tmp_counter++;
            printf("  %%%d = load i8*, i8** %%%d\n", elem_r, eptr_r);

            int r = tmp_counter++;
            printf("  %%%d = call i32 @atoi(i8* %%%d)\n", r, elem_r);
            return r;
        }

        /* ---- args.length  (argc stored as %__<name>_len) ---- */

        case Length: {
            struct node *id_node = get_child(expr, 0);
            int r = tmp_counter++;
            printf("  %%%d = load i32, i32* %%__%s_len\n",
                   r, id_node->token);
            return r;
        }

        default:
            return -1;
    }
}

/* =========================================================================
 * StmtGen
 *   Returns 1 when the statement DEFINITELY ends on a terminator instruction
 *   (ret or unconditional br), so the caller knows whether to emit a fall-
 *   through branch.  Returns 0 otherwise.
 * ========================================================================= */
static int codegen_statement(struct node *stmt, SymTable *global,
                              SymTable *local) {
    if (!stmt) return 0;

    switch (stmt->category) {

        /* ---- Block ---- */

        case Block: {
            int term = 0;
            int i = 0;
            struct node *c;
            while ((c = get_child(stmt, i++)) != NULL) {
                term = codegen_statement(c, global, local);
            }
            return term;
        }

        /* ---- If / if-else ---- */

        case If: {
            struct node *cond_node = get_child(stmt, 0);
            struct node *then_node = get_child(stmt, 1);
            struct node *else_node = get_child(stmt, 2);  /* NULL if no else */

            int id     = tmp_counter++;
            int cond_r = codegen_expression(cond_node, global, local);
            int then_term, else_term = 0;

            if (else_node) {
                /* if-else */
                printf("  br i1 %%%d, label %%Lthen_%d, label %%Lelse_%d\n",
                       cond_r, id, id);
                printf("Lthen_%d:\n", id);
                then_term = codegen_statement(then_node, global, local);
                if (!then_term)
                    printf("  br label %%Lendif_%d\n", id);

                printf("Lelse_%d:\n", id);
                else_term = codegen_statement(else_node, global, local);
                if (!else_term)
                    printf("  br label %%Lendif_%d\n", id);

                /* Emit Lendif only when at least one branch can reach it */
                if (!then_term || !else_term)
                    printf("Lendif_%d:\n", id);

                return (then_term && else_term) ? 1 : 0;
            } else {
                /* if without else — false branch always falls to endif */
                printf("  br i1 %%%d, label %%Lthen_%d, label %%Lendif_%d\n",
                       cond_r, id, id);
                printf("Lthen_%d:\n", id);
                then_term = codegen_statement(then_node, global, local);
                if (!then_term)
                    printf("  br label %%Lendif_%d\n", id);
                printf("Lendif_%d:\n", id);
                return 0;  /* can always exit via false branch */
            }
        }

        /* ---- While ---- */

        case While: {
            struct node *cond_node = get_child(stmt, 0);
            struct node *body_node = get_child(stmt, 1);

            int id = tmp_counter++;
            printf("  br label %%Lwhile_%d\n", id);
            printf("Lwhile_%d:\n", id);
            int cond_r = codegen_expression(cond_node, global, local);
            printf("  br i1 %%%d, label %%Lbody_%d, label %%Lendwhile_%d\n",
                   cond_r, id, id);
            printf("Lbody_%d:\n", id);
            int body_term = codegen_statement(body_node, global, local);
            if (!body_term)
                printf("  br label %%Lwhile_%d\n", id);
            printf("Lendwhile_%d:\n", id);
            return 0;  /* execution can exit the loop */
        }

        /* ---- System.out.print ---- */

        case Print: {
            struct node *expr = get_child(stmt, 0);
            if (!expr) return 0;

            /* STRLIT argument: use printf("%s", literal_ptr) */
            if (expr->category == StrLit) {
                int idx    = register_strlit(expr->token);
                int str_r  = tmp_counter++;
                int call_r = tmp_counter++;
                printf("  %%%d = getelementptr inbounds [%d x i8], "
                       "[%d x i8]* @.str.lit.%d, i32 0, i32 0\n",
                       str_r, strlit_byte_count[idx],
                       strlit_byte_count[idx], idx);
                printf("  %%%d = call i32 (i8*, ...) @printf("
                       "i8* getelementptr inbounds "
                       "([3 x i8], [3 x i8]* @.str.string, i32 0, i32 0), "
                       "i8* %%%d)\n",
                       call_r, str_r);
                return 0;
            }

            if (expr->annot_type == T_StringArray) return 0;

            int val_r  = codegen_expression(expr, global, local);
            int call_r = tmp_counter++;

            if (expr->annot_type == T_Int) {
                printf("  %%%d = call i32 (i8*, ...) @printf("
                       "i8* getelementptr inbounds "
                       "([4 x i8], [4 x i8]* @.str.int, i32 0, i32 0), "
                       "i32 %%%d)\n",
                       call_r, val_r);
            } else if (expr->annot_type == T_Double) {
                printf("  %%%d = call i32 (i8*, ...) @printf("
                       "i8* getelementptr inbounds "
                       "([7 x i8], [7 x i8]* @.str.double, i32 0, i32 0), "
                       "double %%%d)\n",
                       call_r, val_r);
            } else if (expr->annot_type == T_Bool) {
                /*
                 * Boolean printing requires a branch: emit br before Ltrue/
                 * Lfalse, print the right string, then merge at Lendbool.
                 * Note: call_r was reserved before the branch only for int/
                 * double; for bool we use fresh registers inside each branch.
                 */
                int id  = tmp_counter++;
                printf("  br i1 %%%d, label %%L_true_%d, label %%L_false_%d\n",
                       val_r, id, id);

                printf("L_true_%d:\n", id);
                int cr1 = tmp_counter++;
                printf("  %%%d = call i32 (i8*, ...) @printf("
                       "i8* getelementptr inbounds "
                       "([6 x i8], [6 x i8]* @.str.true, i32 0, i32 0))\n",
                       cr1);
                printf("  br label %%L_endbool_%d\n", id);

                printf("L_false_%d:\n", id);
                int cr2 = tmp_counter++;
                printf("  %%%d = call i32 (i8*, ...) @printf("
                       "i8* getelementptr inbounds "
                       "([7 x i8], [7 x i8]* @.str.false, i32 0, i32 0))\n",
                       cr2);
                printf("  br label %%L_endbool_%d\n", id);

                printf("L_endbool_%d:\n", id);

                /* The pre-allocated call_r went unused for bool; that is
                 * intentional — it is subsumed by cr1/cr2 inside the branches.
                 * Suppress the unused variable warning by casting. */
                (void)call_r;
            }
            return 0;
        }

        /* ---- Return ---- */

        case Return: {
    struct node *expr = get_child(stmt, 0);
    if (expr) {
        int val_r = codegen_expression(expr, global, local);
        /* coerce al tipo de retorno declarado (int→double, etc.) */
        val_r = coerce_to(val_r, expr->annot_type, current_ret_type);
        printf("  ret %s %%%d\n", type_to_llvm(current_ret_type), val_r);
    } else {
        if (current_ret_type == T_Void) {
                    printf("  ret void\n");
                } else {
                    printf("  ret %s %s\n", 
                           type_to_llvm(current_ret_type), 
                           default_val_llvm(current_ret_type));
                }
    }
    return 1;
}

        /* ---- Expression statements ---- */

        case Assign:
        case Call:
        case ParseArgs:
            codegen_expression(stmt, global, local);
            return 0;

        case VarDecl:
            return 0;  /* handled in the alloca pre-pass */

        /* ---- Anything else: recurse through children ---- */

        default: {
            int term = 0;
            int i = 0;
            struct node *c;
            while ((c = get_child(stmt, i++)) != NULL) {
                term = codegen_statement(c, global, local);
            }
            return term;
        }
    }
}

/* =========================================================================
 * MethodGen
 * ========================================================================= */
static void codegen_method(struct node *method, SymTable *global) {
    struct node *header   = get_child(method, 0);
    struct node *body     = get_child(method, 1);
    
    if (!header) return;

    struct node *ret_type = get_child(header, 0);
    struct node *id_node  = get_child(header, 1);
    struct node *params   = get_child(header, 2);

    if (!id_node || !id_node->token) return;

    ParamType *params_list = build_params_list(params);
    char params_str[256];
    params_to_str(params_list, params_str, sizeof(params_str));
    
    char title[512];
    snprintf(title, sizeof(title), "Method %s%s", id_node->token, params_str);
    SymTable *local_table = search_table(global, title);
    int is_main = 0;
    if (strcmp(id_node->token, "main") == 0) {
        if (main_emitted == 0) {
            is_main = 1;
            main_emitted = 1;
        }
    }

    tmp_counter = 1;

    /* ---- 1. Function signature ---- */

    current_ret_type = is_main ? T_Int : type_from_node(ret_type); //main needs i32 as return
    BasicType param_types[64]; int npt = 0;
    { int pi = 0; struct node *p;
    while ((p = get_child(params, pi++)) != NULL) {
        struct node *pt = get_child(p, 0);
        if (pt) param_types[npt++] = type_from_node(pt);
    }
    }
    char mangled[128]; build_mangled_suffix(param_types, npt, mangled);



if (is_main) {
    /* main siempre con firma estándar C */
    printf("define i32 @main(i32 %%argc, i8** %%argv) {\n");
} else {
    printf("define %s @_%s%s(",
           type_to_llvm(type_from_node(ret_type)),
           id_node->token, mangled);

    /* Imprimir parámetros formales */
    int first = 1, pi = 0;
    struct node *p;
    while ((p = get_child(params, pi++)) != NULL) {
        struct node *p_type = get_child(p, 0);
        struct node *p_id   = get_child(p, 1);
        if (!p_id || !p_id->token) continue;
        if (!first) printf(", ");
        printf("%s %%_arg_%s",
               type_to_llvm(type_from_node(p_type)), p_id->token);
        first = 0;
    }
    printf(") {\n");
}
printf("entry:\n");
    /* ---- 2. Alloca + store for parameters ---- */
    if (is_main) {
        int i = 0;
        struct node *p;
        while ((p = get_child(params, i++)) != NULL) {
            struct node *p_type = get_child(p, 0);
            struct node *p_id   = get_child(p, 1);
            if (!p_id || !p_id->token) continue;
            
            if (type_from_node(p_type) == T_StringArray) {
                printf("  %%%s = alloca i8**\n", p_id->token);
                printf("  store i8** %%argv, i8*** %%%s\n", p_id->token);
                printf("  %%__%s_len = alloca i32\n", p_id->token);
                int r_argc = tmp_counter++;
                printf("  %%%d = sub i32 %%argc, 1\n", r_argc);
                printf("  store i32 %%%d, i32* %%__%s_len\n", r_argc, p_id->token);
            }
        }
    } else {
        int i = 0;
        struct node *p;
        while ((p = get_child(params, i++)) != NULL) {
            struct node *p_type = get_child(p, 0);
            struct node *p_id   = get_child(p, 1);
            if (!p_id || !p_id->token) continue;

            if (type_from_node(p_type) == T_StringArray) {
                printf("  %%%s = alloca i8**\n", p_id->token);
                printf("  store i8** %%_arg_%s, i8*** %%%s\n", p_id->token, p_id->token);
                
                printf("  %%__%s_len = alloca i32\n", p_id->token);
                printf("  store i32 0, i32* %%__%s_len\n", p_id->token);
                
            } else {
                const char *ll_t = type_to_llvm(type_from_node(p_type));
                printf("  %%%s = alloca %s\n", p_id->token, ll_t);
                printf("  store %s %%_arg_%s, %s* %%%s\n", ll_t, p_id->token, ll_t, p_id->token);
            }
        }
    }

    /* ---- 3. Alloca for local variables ---- */
    int i = 0;
    struct node *s;
    while ((s = get_child(body, i++)) != NULL) {
        if (s->category == VarDecl) {
            struct node *v_type = get_child(s, 0);
            struct node *v_id   = get_child(s, 1);
            if (v_id && v_id->token) {
                printf("  %%%s = alloca %s\n", v_id->token, type_to_llvm(type_from_node(v_type)));
            }
        }
    }
    printf("\n");

    /* ---- 4. Generate body statements ---- */
    int last_term = 0;
    i = 0;
    while ((s = get_child(body, i++)) != NULL) {
        if (s->category != VarDecl) {
            last_term = codegen_statement(s, global, local_table);
        }
    }

    /* ---- 5. Fallback return ---- */
    if (!last_term) {
        if (is_main) {
            printf("  ret i32 0\n");
        } else if (type_from_node(ret_type) == T_Void) {
            printf("  ret void\n");
        } else {
            printf("  ret %s %s\n", type_to_llvm(type_from_node(ret_type)), default_val_llvm(type_from_node(ret_type)));
        }
    }

    printf("}\n\n");
}

/* =========================================================================
 * ProgramGen  —  top-level entry point
 * ========================================================================= */
void codegen_program(struct node *program, SymTable *global_table) {

    /* 0. Pre-scan to collect all string literals before emitting anything */
    collect_strlits(program);

    /* 1. External C function declarations */
    printf("declare i32 @printf(i8*, ...)\n");
    printf("declare i32 @atoi(i8*)\n\n");

    /* 2. Static format strings
     *    Sizes (bytes, including \n and \0):
     *      %d\n\0      → 4   [4 x i8]
     *      %.16e\n\0   → 7   [7 x i8]   
     *      true\n\0    → 6   [6 x i8]
     *      false\n\0   → 7   [7 x i8]
     *      %s\0        → 3   [3 x i8]   
     */
    printf("@.str.int    = private unnamed_addr constant "
           "[4 x i8] c\"%%d\\0A\\00\"\n");
    printf("@.str.double = private unnamed_addr constant "
           "[7 x i8] c\"%%.16e\\0A\\00\"\n");
    printf("@.str.true   = private unnamed_addr constant "
           "[6 x i8] c\"true\\0A\\00\"\n");
    printf("@.str.false  = private unnamed_addr constant "
           "[7 x i8] c\"false\\0A\\00\"\n");
    printf("@.str.string = private unnamed_addr constant "
           "[3 x i8] c\"%%s\\00\"\n\n");

    /* 3. User string-literal globals */
    emit_strlits();

    /* 4. Global (field) variables */
    int i = 0;
    struct node *c;
    while ((c = get_child(program, i++)) != NULL) {
        if (c->category == FieldDecl) {
            struct node *type_node = get_child(c, 0);
            struct node *id_node   = get_child(c, 1);
            if (!id_node || !id_node->token) continue;
            
            BasicType t = type_from_node(type_node);
            printf("@_%s = global %s %s\n", id_node->token, type_to_llvm(t), default_val_llvm(t));
        }
    }
    printf("\n");

    /* 5. Methods */
    i = 0;
    while ((c = get_child(program, i++)) != NULL) {
        if (c->category == MethodDecl) {
            struct node *header = get_child(c, 0);
            if (!header) continue;
            
            struct node *mid = get_child(header, 1);
            if (mid && mid->annot_type != T_Undef) {
                codegen_method(c, global_table);
            }
        }
    }
    free_strlits();
}