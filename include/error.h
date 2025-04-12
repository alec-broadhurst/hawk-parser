#ifndef ERROR_H
#define ERROR_H

#define VARIABLE_REDECLARATION "Line %d: Variable %s already declared\n"
#define VARIABLE_NOT_DECLARED "Line %d: Variable %s not declared\n"
#define ILLEGAL_SYMBOL "Line %d: Illegal symbol %c\n"
#define ILLEGAL_IDENTIFIER "Line %d: Illegal identifier %s: Identifiers must start with a letter or underscore, and can only contain letters, digits, and underscores\n"
#define ILLEGAL_NUMBER "Line %d: Illegal number %s: Numbers must be at most 10 digits and contain at most one decimal point\n"
#define EXPECTED_KEYWORD "Line %d: Expected keyword %s\n"
#define EXPECTED_SYMBOL "Line %d: Expected symbol %c\n"
#define MISSING_SEMICOLON "Line %d: Expected ';' after '%s'\n"
#define EXPECTED_IDENTIFIER "Line %d: Expected identifier, got %s\n"
#define EXPECTED_STATEMENT "Line %d: Expected statement, got %s\n"
#define EXPECTED_OPERAND "Line %d: Expected operand, got %s\n"
#define EXPECTED_COMPARISON "Line %d: Expected comparison operator, got %s\n"
#define EXPECTED_TYPE "Line %d: Expected type, got %s\n"
#define EXPECTED_ASSIGNMENT "Line %d: Expected assignment operator, got %s\n"
#define KEYWORD_IDENTIFIER_CONFLICT "Line %d: %s is a keyword, cannot be used as an identifier\n"
#define EXPECTED_ID_NUM "Line %d: Expected identifier or number, got %s\n"

#endif
