# Relatório

## Secção i

### Transformações da Gramática EBNF para LALR(1)

A gramática original, fornecida em notação EBNF, foi transformada para uma gramática compatível com análise ascendente LALR(1) usando `yacc`. As principais transformações e decisões são descritas a seguir.

### Eliminação de Ambiguidade e Definição de Precedências

A gramática original das expressões é ambígua. Em vez de reescrever as produções para codificar precedência estruturalmente, optou-se por manter uma gramática mais plana para `SimpleExpr` e usar as diretivas `%left`, `%right` e `%nonassoc` do `yacc` para definir precedência e associatividade. A hierarquia definida, da menor para a maior precedência, é: atribuição (`ASSIGN`, associativa à direita), disjunção lógica (`OR`), conjunção lógica (`AND`), XOR, igualdade/diferença (`EQ`, `NE`), relacionais (`LT`, `GT`, `LE`, `GE`), deslocamentos (`LSHIFT`, `RSHIFT`), adição/subtração (`PLUS`, `MINUS`), multiplicação/divisão/módulo (`STAR`, `DIV`, `MOD`), e operadores unários (`NOT`, `UNARY`). Esta abordagem reproduz fielmente a precedência da linguagem Java.

O conflito clássico do `if-else` pendente foi resolvido declarando `IF_PREC` com `%nonassoc` a uma precedência inferior à de `ELSE`, e anotando a produção `IF ... Statement` sem `ELSE` com `%prec IF_PREC`, forçando o `yacc` a preferir o shift do `ELSE` ao reduce.

### Separação de Expr e SimpleExpr

A produção `Expr` foi dividida em duas: `Expr` trata apenas a atribuição (`Assignment`) e delega tudo o resto a `SimpleExpr`. Esta separação foi necessária para o yacc poder distinguir claramente entre uma atribuição e qualquer outra expressão, uma vez que ambas partilham o prefixo `IDENTIFIER`. A atribuição é declarada associativa à direita e com menor precedência que todos os operadores binários, mantendo a semântica correta da linguagem Java.

### Declarações Múltiplas numa Linha

As produções `FieldDecl` e `VarDecl` permitem declarar múltiplos identificadores numa única instrução (e.g., `int x, y, z;`). Para preservar a estrutura da AST — onde cada declaração corresponde a um nó separado `FieldDecl`/`VarDecl` — foi criada a função auxiliar `create_multiple_decls`, que constrói um nó `MethodBody` temporário (usado como contentor) com um nó de declaração por identificador. Este contentor é depois desempacotado com `unpack_nodes` no nó pai correto.

### Produções de Erro para Recuperação

Foram incluídas as seguintes produções de recuperação de erros sintáticos, conforme especificado:

- `FieldDecl → error SEMICOLON`
- `Statement → error SEMICOLON`
- `ParseArgs → PARSEINT LPAR error RPAR`
- `MethodInvocation → IDENTIFIER LPAR error RPAR`
- `SimpleExpr → LPAR error RPAR`

Estas produções permitem ao analisador recuperar de erros locais, continuar a análise e reportar múltiplos erros numa única invocação.

## Secção ii

### Estrutura de Dados da AST
 
Cada nó da AST é representado pela estrutura `node`, que contém: a categoria do nó (enum `category`), o texto original do token (`token`), a linha e coluna no ficheiro de entrada (`line`, `col`), o tipo anotado (`annot_type`), a assinatura de parâmetros para chamadas de métodos (`annot_params`), e um ponteiro para a lista de filhos.

Os filhos são armazenados numa lista ligada `node_list` com um nó sentinela na cabeça, o que simplifica a inserção em `addchild` e o acesso por índice em `get_child`. A impressão é feita recursivamente por `show` e `show_annotated`, sendo a anotação de tipo impressa como ` - tipo` após o nome do nó, usando `annot_params` para nós `Identifier` de chamadas de métodos e `annot_type` para os restantes.
 
### Construção da AST
 
Os nós são criados nas ações semânticas do `jucompiler.y` com `newnode`. Para declarações múltiplas numa linha (e.g. `int x, y;`), a função auxiliar `create_multiple_decls` cria um nó `MethodBody` temporário como contentor com um nó `FieldDecl`/`VarDecl` por identificador, que é depois desempacotado no nó pai com `unpack_nodes`. Nós supérfluos são evitados: um `Block` com zero filhos é substituído por `NULL` e com um único filho é substituído pelo próprio filho, exceto quando representa um `Statement` obrigatoriamente vazio.
 
### Estrutura de Dados da Tabela de Símbolos
 
Cada símbolo é representado pela estrutura `Symbol`, que contém: o nome, o tipo (`BasicType`), o classificação (`SymbolKind`: `SYM_FIELD`, `SYM_METHOD`, `SYM_PARAM`, `SYM_LOCAL`, `SYM_RETURN`), a lista de tipos dos parâmetros formais (`ParamType *params`, apenas para métodos), e um ponteiro para a tabela de símbolos do próprio método (`nested_table`). A tabela global contém todos os campos e métodos, a tabela de cada método, armazenada diretamente em `nested_table`, contém o pseudo-símbolo `return`, os parâmetros formais e as variáveis locais. 
 
### Algoritmo de Análise Semântica
 
A análise semântica é feita em duas passagens. A **passagem 1** (`check_semantics_pass1`) regista antecipadamente todos os campos e cabeçalhos de métodos na tabela global, criando desde logo a `nested_table` de cada método com o símbolo `return` e os parâmetros formais, o que permite chamadas a métodos declarados posteriormente. A **passagem 2** (`check_semantics_pass2`) percorre recursivamente a AST, regista as variáveis locais de cada método, e delega a verificação de tipos a `check_node`, que opera em pós-ordem para garantir que os tipos dos filhos etsão anotados antes do nó pai ser verificado. Os erros são recolhidos numa lista ligada e impressos no final, antes das tabelas e da AST anotada.