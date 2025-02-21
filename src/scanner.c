#include <ctype.h>
#include <stdio.h>

typedef enum {
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
    CLASS_LETTER,
    CLASS_DIGIT,
    CLASS_UNKNOWN,
    CLASS_EOF,
} CharClass;

typedef struct {
    FILE *fp;
    char currentChar;
    CharClass charClass;
    char lexeme[100];
    int lexemeLength;
    TokenType currentToken;
} Scanner;

// function to create a scanner and open the source file
Scanner createScanner(const char* filename) {
    Scanner scanner;
    scanner.fp = fopen(filename, "r");
    return scanner;
}

// function to get the next character
void nextChar(Scanner* scanner) {
    // get the next character from the file
    scanner->currentChar = getc(scanner->fp);

    // determine the character class
    if (scanner->currentChar != EOF) {
        if (isalpha(scanner->currentChar)) {
            scanner->charClass = CLASS_LETTER;
        } else if (isdigit(scanner->currentChar)) {
            scanner->charClass = CLASS_DIGIT;
        } else {
            scanner->charClass = CLASS_UNKNOWN;
        }
    } else {
        scanner->charClass = CLASS_EOF;
    }
}

// function to add the current character to the lexeme
void addChar(Scanner* scanner) {
    scanner->lexeme[scanner->lexemeLength++] = scanner->currentChar;
    scanner->lexeme[scanner->lexemeLength] = '\0';
}

// function to skip whitespace
void skipWhitespace(Scanner* scanner) {
    while (isspace(scanner->currentChar)) {
        nextChar(scanner);
    }
}

void lookup(Scanner* scanner) {
    switch(scanner->currentChar) {
        case '(':
            addChar(scanner);
            nextChar(scanner);
            scanner->currentToken = LEFT_PAREN;
            break;

        case ')':
            addChar(scanner);
            nextChar(scanner);
            scanner->currentToken = RIGHT_PAREN;
            break;

        case ';':
            addChar(scanner);
            nextChar(scanner);
            scanner->currentToken = SEMICOLON;
            break;

        case ',':
            addChar(scanner);
            nextChar(scanner);
            scanner->currentToken = COMMA;
            break;

        case ':':
            addChar(scanner);
            nextChar(scanner);
            if (scanner->currentChar == '=') {
                addChar(scanner);
                nextChar(scanner);
                scanner->currentToken = ASSIGN;
            } else {
                scanner->currentToken = COLON;
            }
            break;

        case '+':
            addChar(scanner);
            nextChar(scanner);
            scanner->currentToken = PLUS;
            break;

        case '-':
            addChar(scanner);
            nextChar(scanner);
            scanner->currentToken = MINUS;
            break;

        case '*':
            addChar(scanner);
            nextChar(scanner);
            scanner->currentToken = MULTIPLY;
            break;

        case '/':
            addChar(scanner);
            nextChar(scanner);
            scanner->currentToken = DIVIDE;
            break;

        case '<':
            addChar(scanner);
            nextChar(scanner);
            if (scanner->currentChar == '>') {
                addChar(scanner);
                nextChar(scanner);
                scanner->currentToken = NOT_EQUALS;
            } else {
                scanner->currentToken = LESS_THAN;
            }
            break;

        case '>':
            addChar(scanner);
            nextChar(scanner);
            scanner->currentToken = GREATER_THAN;
            break;
    }
}

// function to collect the next token
TokenType nextToken(Scanner* scanner) {
    // reset lexeme length for next token
    scanner->lexemeLength = 0;
    skipWhitespace(scanner);
    TokenType token;

    // determine the token based on the character class
    switch(scanner->charClass) {
        case CLASS_LETTER:
            addChar(scanner);
            nextChar(scanner);
            while (scanner->charClass == CLASS_LETTER || scanner->charClass == CLASS_DIGIT) {
                addChar(scanner);
                nextChar(scanner);
            }
            token = ID;
            break;
        case CLASS_DIGIT:
            addChar(scanner);
            nextChar(scanner);
            while (scanner->charClass == CLASS_DIGIT) {
                addChar(scanner);
                nextChar(scanner);
            }
            token = NUM;
            break;
        case CLASS_UNKNOWN:
            addChar(scanner);
            nextChar(scanner);
            // add lookup function for parens and operators
            break;
        case CLASS_EOF:
            scanner->currentToken = EOF;
            break;
    }
    return token;
}
