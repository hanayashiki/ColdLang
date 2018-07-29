# syntax 0.1

## entity

entity are atoms in `expr`

+ entity ->	atom sub_entity
+ sub_entity -> None | `.` entity
+ atom -> [identifier] | func_call | literal | func_def_and_optional_call | `(` expr `)` | `true` | `false` | `none`
+ func_call -> identifier `(` comma_exprs `)` | identifier `()` | 
+ literal -> [string] | [float] | [integer]
+ comma_exprs -> expr {, expr}

## expression

`expr` defines right values, the name `expr_i` is used to define 
different levels of priorities.

+ expr -> expr_1 `?` expr `:` expr
+ expr_1 -> expr_2 {`||` expr_2}
+ expr_2 -> expr_3 {`&&` expr_3}
+ expr_3 -> expr_4 {(`==`|`!=`) expr_4}
+ expr_4 -> expr_5 {(`>=`|`<=`|`>`|`<`) expr_5}
+ expr_5 -> term {(`+`|`-`) term}
+ term -> factor {(`*`|`/`|`%`) factor}
+ factor -> entity | entity (`++`|`--`) | `+` entity | `-` entity

## statement

`statement` defines lines of statements. For simplicity of initial implement, there are no `if`, `while` and so on.

+ statement -> entity | entity `=` expression
+ statement -> `retv` expr
+ statement -> `ret`
+ statement_block -> statement {'\n' statement_block}
+ statement -> `if` expr `{` statement_block `}` optional_elif_list optional_else
+ optional_elif_list -> `elif` expr `{` statement_block `}` optional_elif_list | empty
+ optional_else -> `else` `{` statement_block `}` | empty
+ statement -> `while` expr `{` statement_block `}`
+ statement -> `break`

## function

define function, and opt to call it at once.

+ func_def_and_optional_call -> func_def | (None | `(` comma_exprs `)`)
+ func_def -> `fn` comma_identifiers `{` statement_block `}`
