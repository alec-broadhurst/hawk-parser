# Hawk Parser
This project implements a scanner and a recursive descent parser for a small programming language named **Hawk**. It was developed as a term project for my **Programming Language Concepts** class during Spring 2025.

## What This Project Does
The parser reads programs written in the Hawk language, performs lexical analysis (scanning) and syntactical analysis (parsing), and produces an output reflecting the left-hand side of the production rules applied during parsing. It also detects errors such as illegal symbols, illegal identifiers, illegal numbers, and other parse errors. When an error is encountered, a descriptive error message (including the line number) is produced and the program exits immediately.

## Language Grammar
This is the grammar this project is built to parse:
1. **PROGRAM** → program **DECL_SEC** begin **STMT_SEC** end; | program begin **STMT_SEC** end;
2. **DECL_SEC** → **DECL** | **DECL DECL_SEC**
3. **DECL** → **ID_LIST** : **TYPE** ;
4. **ID_LIST** → **ID** | **ID , ID_LIST**
5. **ID** → (_ | a | b | ... | z | A | ... | Z) (_ | a | b | ... | z | A | ... | Z | 0 | 1 | ... | 9)*
6. **STMT_SEC** → **STMT** | **STMT STMT_SEC**
7. **STMT** → **ASSIGN** | **IFSTMT** | **WHILESTMT** | **INPUT** | **OUTPUT**
8. **ASSIGN** → **ID** := **EXPR** ;
9. **IFSTMT** → if **COMP** then **STMT_SEC** end if ; | if **COMP** then **STMT_SEC** else **STMT_SEC** end if ;
10. **WHILESTMT** → while **COMP** loop **STMT_SEC** end loop ;
11. **INPUT** → input **ID_LIST**;
12. **OUTPUT** → output **ID_LIST** | output **NUM**;
13. **EXPR** → **FACTOR** | **FACTOR + EXPR** | **FACTOR - EXPR**
14. **FACTOR** → **OPERAND** | **OPERAND * FACTOR** | **OPERAND / FACTOR**
15. **OPERAND** → **NUM** | **ID** | ( **EXPR** ) | **FUNCALL**
16. **NUM** → (0 | 1 | ... | 9)+[.(0 | 1 | ... | 9)+]
17. **COMP** → ( **OPERAND = OPERAND** ) | ( **OPERAND <> OPERAND** ) | ( **OPERAND > OPERAND** ) | ( **OPERAND < OPERAND** )
18. **TYPE** → int | float | double
19. **FUNCALL** → call **ID( ID_LIST )** ;

## Lexical Elements
- **Reserved Words**: `program`, `begin`, `end`, `if`, `then`, `else`, `input`, `output`, `int`, `float`, `double`, `while`, `loop`
- **Operators**: assignment (`:=`), less than (`<`), greater than (`>`), equals (`=`), not equals (`<>`), plus (`+`), minus (`-`), multiply (`*`), divide (`/`) and parentheses
- **Delimiters**: `;` is used to terminate statements and the `,` is used when declaring variables
- **Identifiers**: Must start with a letter or an underscore, followed by any combination of letters, digits, or underscores.
- **Numbers**: Either integer numbers (up to 10 digits) or floating point numbers (up to 10 digits).

## Error Handling
The parser is designed to catch and report the following errors:
- **Illegal Symbol**: Encountering a character that is not part of the language.
- **Illegal Identifier**: Improperly formatted identifiers.
- **Illegal Number**: Numbers exceeding the allowed digit count or malformed numbers.
- **Parse Errors**: Missing or unexpected tokens during parsing.
- **Redeclaration Errors**: Multiple declarations of the same variable.
- **Undeclared Identifier**: Use of a variable before declaration.

Each error message includes the specific line number and a clear description of the issue.

## How to Run
Run `make` to compile and run the project.
