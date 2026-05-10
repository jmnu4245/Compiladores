# Report

## Section i

### Transformations from EBNF Grammar to LALR(1)

The original grammar, provided in EBNF notation, was transformed into a grammar compatible with LALR(1) bottom-up parsing using `yacc`. The main transformations and design decisions are described below.

### Disambiguation and Precedence Rules

The original expression grammar is ambiguous. Rather than rewriting productions to encode precedence structurally, a flat grammar for `SimpleExpr` was kept and the `%left`, `%right`, and `%nonassoc` directives of `yacc` were used to define operator precedence and associativity. The hierarchy defined, from lowest to highest precedence, is: assignment (`ASSIGN`, right-associative), logical disjunction (`OR`), logical conjunction (`AND`), XOR, equality/inequality (`EQ`, `NE`), relational operators (`LT`, `GT`, `LE`, `GE`), shifts (`LSHIFT`, `RSHIFT`), addition/subtraction (`PLUS`, `MINUS`), multiplication/division/modulo (`STAR`, `DIV`, `MOD`), and unary operators (`NOT`, `UNARY`). This approach faithfully reproduces Java's operator precedence.

The classic dangling `else` conflict was resolved by declaring `IF_PREC` with `%nonassoc` at a lower precedence than `ELSE`, and annotating the `IF ... Statement` production without `ELSE` with `%prec IF_PREC`, forcing `yacc` to prefer shifting `ELSE` over reducing.

### Separation of Expr and SimpleExpr

The `Expr` production was split into two: `Expr` handles only assignment (`Assignment`) and delegates everything else to `SimpleExpr`. This separation was necessary for `yacc` to clearly distinguish between an assignment and any other expression, since both share the prefix `IDENTIFIER`. Assignment is declared right-associative and with lower precedence than all binary operators, preserving the correct semantics of the Java language.

### Multiple Declarations on a Single Line

The `FieldDecl` and `VarDecl` productions allow declaring multiple identifiers in a single statement (e.g., `int x, y, z;`). To preserve the AST structure — where each declaration corresponds to a separate `FieldDecl`/`VarDecl` node — an auxiliary function `create_multiple_decls` was created, which builds a temporary `MethodBody` node as a container with one declaration node per identifier. This container is then unpacked into the correct parent node using `unpack_nodes`.

### Error Recovery Productions

The following syntax error recovery productions were included, as specified:

- `FieldDecl → error SEMICOLON`
- `Statement → error SEMICOLON`
- `ParseArgs → PARSEINT LPAR error RPAR`
- `MethodInvocation → IDENTIFIER LPAR error RPAR`
- `SimpleExpr → LPAR error RPAR`

These productions allow the parser to recover from local errors, continue analysis, and report multiple errors in a single invocation.

## Section ii

### AST Data Structure and Construction
 
Each AST node is represented by the `node` structure, containing the category, the original token text, the line and column, the annotated type (`annot_type`), the parameter signature for method calls (`annot_params`), and a pointer to a `node_list` children linked list with a sentinel head. Nodes are created in `jucompiler.y` semantic actions using `newnode`. Superfluous `Block` nodes are eliminated: zero children become `NULL`, one child is replaced by that child itself. For multiple declarations on a single line, `create_multiple_decls` builds a temporary container unpacked into the parent with `unpack_nodes`. The full tree is released by `free_tree`; in syntax error cases, partially built subtrees are freed before the parser returns. Printing is done recursively by `show` and `show_annotated`, using `annot_params` for method call `Identifier` nodes and `annot_type` for all others.
 
### Symbol Table Data Structure
 
Each symbol is represented by the `Symbol` structure, containing the name, type (`BasicType`), classification (`SymbolKind`), formal parameter types (`ParamType *params`, methods only), and a pointer to the method's nested table (`nested_table`). Five kinds are defined: `SYM_FIELD`, `SYM_METHOD`, `SYM_PARAM`, `SYM_LOCAL`, and `SYM_RETURN`. The last stores the enclosing method's expected return type as a pseudo-symbol, avoiding special-case lookups when validating `return` statements. The global table holds all fields and methods; each method's `nested_table` holds `return`, formal parameters, and local variables.
 
### Semantic Analysis Algorithm
 
Semantic analysis runs in two passes. **Pass 1** eagerly registers all fields and method headers, immediately building each method's `nested_table`, enabling forward calls. **Pass 2** recursively traverses the AST, registers local variables, and delegates expression type-checking to `check_node`, which operates in post-order so children are annotated before their parent.
 
Type compatibility follows Java rules: `int` promotes implicitly to `double`; arithmetic operators require numeric operands; `&&` and `||` require `boolean`; `^` requires `int`; `==` and `!=` accept two operands of the same type or any two numeric types. When an operation receives invalid operands, the result is set to `undef`, which propagates up the tree to suppress cascading errors on parent nodes. Method overload resolution prioritises exact matches; if none exists, a unique compatible match is selected; multiple compatible matches yield an ambiguity error; no match yields a cannot-find error — in both cases, the `Call` and `Identifier` nodes are annotated with `undef`. Errors are collected in a linked list and printed before the symbol tables and annotated AST.