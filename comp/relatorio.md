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

### AST Data Structure

Each AST node is represented by the `node` structure, which contains: the node category (enum `category`), the original token text (`token`), the line and column in the input file (`line`, `col`), the annotated type (`annot_type`), the parameter signature for method calls (`annot_params`), and a pointer to the list of children.

Children are stored in a `node_list` linked list with a sentinel node at the head, which simplifies insertion in `addchild` and index-based access in `get_child`. Printing is done recursively by `show` and `show_annotated`, with the type annotation printed as ` - type` after the node name, using `annot_params` for `Identifier` nodes of method calls and `annot_type` for all others.

### AST Construction

Nodes are created in the semantic actions of `jucompiler.y` using `newnode`. For multiple declarations on a single line (e.g. `int x, y;`), the auxiliary function `create_multiple_decls` creates a temporary `MethodBody` node as a container with one `FieldDecl`/`VarDecl` node per identifier, which is then unpacked into the parent node using `unpack_nodes`. Superfluous nodes are avoided: a `Block` with zero children is replaced by `NULL` and with a single child is replaced by that child itself, except when it represents a mandatory empty `Statement`.

### Symbol Table Data Structure

Each symbol is represented by the `Symbol` structure, which contains the name, type (`BasicType`), classification (`SymbolKind`: `SYM_FIELD`, `SYM_METHOD`, `SYM_PARAM`, `SYM_LOCAL`, `SYM_RETURN`), the list of formal parameter types (`ParamType *params`, methods only), and a pointer to the method's own symbol table (`nested_table`). The global table contains all fields and methods; each method's table, stored directly in `nested_table`, contains the pseudo-symbol `return`, the formal parameters, and the local variables.

### Semantic Analysis Algorithm

Semantic analysis is performed in two passes. **Pass 1** (`check_semantics_pass1`) eagerly registers all global fields and method headers in the global table, immediately creating each method's `nested_table` with the `return` symbol and formal parameters, which enables calls to methods declared later in the file. **Pass 2** (`check_semantics_pass2`) recursively traverses the AST, registers each method's local variables, and delegates type checking to `check_node`, which operates in post-order to ensure that children's types are annotated before the parent node is checked. Errors are collected in a linked list and printed at the end, before the symbol tables and the annotated AST.

## Section iii

### Code Generation Architecture and Memory Model

For generating the LLVM Intermediate Representation, all local variables and method parameters receive their own alloca instruction right in the entry block of the method. Whenever we need to read or write to them, we use load and store instructions. This design delegates the complex task of register promotion to the LLVM optimizer, specifically the mem2reg pass. Evaluation of expressions generates sequential virtual registers managed by a global counter that resets at the start of each method.

### Method Emission and Name Mangling

Since LLVM does not natively understand Java-style method overloading, we implemented a name mangling system. We prefix functions with @_ and we append a suffix to the method name based on its parameter types, using identifiers like _i for integers or _d for doubles. The only exception is the main method, which is emitted with the standard LLVM signature of i32 @main(i32 %argc, i8 %argv) to serve as the correct program entry point. Additionally, we calculate %argc - 1 at the beginning and store it locally to support the args.length operation naturally.

### Type Coercion

When the semantic rules dictate some type widening, such as promoting an integer to a double, this conversion is managed by our code, coerce_to intercepts it and writes IR with register assignments and method arguments correctly typed. If an integer register must be consumed where a double is expected, we inject a sitofp instruction right before the target operation.
Furthermore, coerce_to includes defensive guards against T_Undef. While this acts as dead code under a correct semantic analysis, it prevents our program from crashing if it processes an AST containing prior semantic errors.

### Control Flow Translation

Translating control flow structures like if-else and while loops involved setting up different basic blocks and jumping between them with br instructions. Conditional statements evaluate their condition into a boolean register and emit a conditional branch to the respective blocks, which then unconditionally branch to a convergent end block. For logical AND and OR operators, we ensured proper short-circuiting by using dynamic branching and temporary stack allocations, preserving the strict evaluation semantics required by the language rather than relying on bitwise operations.

### Global State and Built-in Functions

Before generating method bodies, our generator performs a pre-pass over the AST to collect all string literals. We convert their escape sequences into LLVM-compatible hexadecimal formats and declare them at the top of the file as private constant arrays of bytes. Class fields are similarly declared globally with a specific prefix and initialized to their default values. 
To handle language built-ins like System.out.print and Integer.parseInt, we simply declare and call the standard C library functions printf and atoi, utilizing our pre-defined global format strings.
Decimal literals undergo string normalization before LLVM emission: if a literal starts with a dot (e.g., .5), a 0 is prepended; if it uses scientific notation without a decimal point (e.g., 1e5), a .0 is injected. Additionally, unary plus operators are treated simply as no-ops.