#ifndef ERROR_H
#define ERROR_H

#define VARIABLE_REDECLARATION "Error on line %d: Variable %s already declared\n"
#define VARIABLE_NOT_DECLARED "Error on line %d: Variable %s not declared\n"
#define ILLEGAL_SYMBOL "Error on line %d: Illegal symbol %c\n"
#define ILLEGAL_IDENTIFIER "Error on line %d: Illegal identifier %s\n"
#define ILLEGAL_NUMBER "Error on line %d: Illegal number %s, max length is 10 digits\n"
#define EXPECTED_KEYWORD "Error on line %d: Expected keyword %s\n"
#define EXPECTED_SYMBOL "Error on line %d: Expected symbol %c\n"
#define EXPECTED_IDENTIFIER "Error on line %d: Expected identifier, got %s\n"
#define EXPECTED_STATEMENT "Error on line %d: Expected statement, got %s\n"
#define EXPECTED_OPERAND "Error on line %d: Expected operand, got %s\n"
#define EXPECTED_COMPARISON "Error on line %d: Expected comparison operator, got %s\n"
#define EXPECTED_TYPE "Error on line %d: Expected type, got %s\n"
#define EXPECTED_ASSIGNMENT "Error on line %d: Expected assignment operator, got %s\n"

#endif
