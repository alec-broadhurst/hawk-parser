#ifndef SCANNER_H
#define SCANNER_H

#include <stdio.h>

// Enum for TokenType and CharClass
typedef enum {
    UNKNOWN,

    // Reserved words
    PROGRAM, BEGIN, END, IF, THEN,
    ELSE, INPUT, OUTPUT, INT, FLOAT,
    DOUBLE, WHILE, LOOP, CALL,

    // Identifiers and literals
    ID, NUM,

    // Operators
    ASSIGN, LESS_THAN, GREATER_THAN,
    EQUALS, NOT_EQUALS, PLUS,
    MINUS, MULTIPLY, DIVIDE,

    // Punctuation
    LEFT_PAREN, RIGHT_PAREN,
    SEMICOLON, COMMA, COLON,
} TokenType;

typedef enum {
    CLASS_UNKNOWN,
    CLASS_LETTER,
    CLASS_DIGIT,
    CLASS_EOF,
} CharClass;

// Scanner struct definition
typedef struct {
    FILE *fp;
    char currentChar;
    CharClass charClass;
    char lexeme[100];
    int lexemeLength;
    TokenType currentToken;
    int lineNumber;
} Scanner;

// Function declarations
Scanner createScanner(const char* filename);
void nextChar(Scanner* scanner);
void addChar(Scanner* scanner);
void skipWhitespace(Scanner* scanner);
TokenType lookup(Scanner* scanner);
void nextToken(Scanner* scanner);

#endif
