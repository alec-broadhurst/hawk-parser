#ifndef ERROR_H
#define ERROR_H

#define VARIABLE_REDECLARATION "\x1b[31mLine %d: Variable %s already declared\n"
#define VARIABLE_NOT_DECLARED "\x1b[31mLine %d: Variable %s not declared\n"
#define ILLEGAL_SYMBOL "\x1b[31mLine %d: Illegal symbol %c\n"
#define ILLEGAL_IDENTIFIER "\x1b[31mLine %d: Illegal identifier %s: Identifiers must start with a letter or underscore, and can only contain letters, digits, and underscores\n"
#define ILLEGAL_NUMBER "\x1b[31mLine %d: Illegal number %s: Numbers must be at most 10 digits and contain at most one decimal point\n"
#define EXPECTED_KEYWORD "\x1b[31mLine %d: Expected keyword %s\n"
#define EXPECTED_SYMBOL "\x1b[31mLine %d: Expected symbol %c\n"
#define MISSING_SEMICOLON "\x1b[31mLine %d: Expected ';' after '%s'\n"
#define EXPECTED_IDENTIFIER "\x1b[31mLine %d: Expected identifier, got %s\n"
#define EXPECTED_STATEMENT "\x1b[31mLine %d: Expected statement, got %s\n"
#define EXPECTED_OPERAND "\x1b[31mLine %d: Expected operand, got %s\n"
#define EXPECTED_COMPARISON "\x1b[31mLine %d: Expected comparison operator, got %s\n"
#define EXPECTED_TYPE "\x1b[31mLine %d: Expected type, got %s\n"
#define EXPECTED_ASSIGNMENT "\x1b[31mLine %d: Expected assignment operator, got %s\n"
#define KEYWORD_IDENTIFIER_CONFLICT "\x1b[31mLine %d: %s is a keyword, cannot be used as an identifier\n"
#define EXPECTED_ID_NUM "\x1b[31mLine %d: Expected identifier or number, got %s\n"

#endif
