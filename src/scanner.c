#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "scanner.h"
#include "error.h"

// function to create a scanner and open the source file
Scanner createScanner(const char* filename) {
    Scanner scanner = {0};
    scanner.fp = fopen(filename, "r");
    if (scanner.fp == NULL) {
            fprintf(stderr, "Error opening file: %s\n", filename);
            exit(1);
    }

    scanner.currentToken = UNKNOWN;
    scanner.lineNumber = 1;
    scanner.varTable.varCount = 0;

    nextChar(&scanner);
    return scanner;
}

int variableLookUp(Scanner* scanner) {
    VarTable* table = &scanner->varTable;
    for (int i = 0; i < table->varCount; i++) {
        if (strcmp(table->variables[i].name, scanner->lexeme) == 0) {
            return 1; // found
        }
    }
    return 0; // not found
}

void addVariable(Scanner* scanner) {
    VarTable* table = &scanner->varTable;
    if (!variableLookUp(scanner)) {
        strcpy(table->variables[table->varCount].name, scanner->lexeme);
        table->varCount++;
    } else {
        fprintf(stderr, "Line %d: Variable %s already declared\n", scanner->lineNumber, scanner->lexeme);
        exit(1);
    }
}

// function to get the next character and determine its class
void nextChar(Scanner* scanner) {
    // get the next character from the file, tracking line number
    do {
        scanner->currentChar = getc(scanner->fp);
        if (scanner->currentChar == '\n') {
            scanner->lineNumber++;
        }
    } while (scanner->currentChar == '\n');

    // determine the character class
    if (scanner->currentChar != EOF) {
        if (isalpha(scanner->currentChar) || scanner->currentChar == '_') {
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

// function to lookup operators and parentheses
TokenType lookup(Scanner* scanner) {
    TokenType token = UNKNOWN;
    switch(scanner->currentChar) {
        case '(':
            token = LEFT_PAREN;
            break;

        case ')':
            token = RIGHT_PAREN;
            break;

        case ';':
            token = SEMICOLON;
            break;

        case ',':
            token = COMMA;
            break;

        case ':':
            addChar(scanner);
            nextChar(scanner);
            if (scanner->currentChar == '=') {
                token = ASSIGN;
            } else {
                token = COLON;
            }
            break;

        case '=':
            token = EQUALS;
            break;

        case '+':
            token = PLUS;
            break;

        case '-':
            token = MINUS;
            break;

        case '*':
            token = MULTIPLY;
            break;

        case '/':
            token = DIVIDE;
            break;

        case '<':
            addChar(scanner);
            nextChar(scanner);
            if (scanner->currentChar == '>') {
                token = NOT_EQUALS;
            } else {
                token = LESS_THAN;
            }
            break;

        case '>':
            token = GREATER_THAN;
            break;

        default:
            addChar(scanner);
            nextChar(scanner);
            token = UNKNOWN;
            break;
    }
    return token;
}

// function to collect the next token
void nextToken(Scanner* scanner) {
    // reset lexeme length for next token
    scanner->lexemeLength = 0;
    skipWhitespace(scanner);
    TokenType token = UNKNOWN;

    // determine the token based on the character class
    switch(scanner->charClass) {
        case CLASS_LETTER:
            while (scanner->charClass == CLASS_LETTER || scanner->charClass == CLASS_DIGIT) {
                addChar(scanner);
                nextChar(scanner);
            }
            // check if the lexeme is a reserved word
            if (strcmp(scanner->lexeme, "program") == 0) {
                token = PROGRAM;
            } else if (strcmp(scanner->lexeme, "begin") == 0) {
                token = BEGIN;
            } else if (strcmp(scanner->lexeme, "end") == 0) {
                token = END;
            } else if (strcmp(scanner->lexeme, "if") == 0) {
                token = IF;
            } else if (strcmp(scanner->lexeme, "then") == 0) {
                token = THEN;
            } else if (strcmp(scanner->lexeme, "else") == 0) {
                token = ELSE;
            } else if (strcmp(scanner->lexeme, "input") == 0) {
                token = INPUT;
            } else if (strcmp(scanner->lexeme, "output") == 0) {
                token = OUTPUT;
            } else if (strcmp(scanner->lexeme, "int") == 0) {
                token = INT;
            } else if (strcmp(scanner->lexeme, "float") == 0) {
                token = FLOAT;
            } else if (strcmp(scanner->lexeme, "double") == 0) {
                token = DOUBLE;
            } else if (strcmp(scanner->lexeme, "while") == 0) {
                token = WHILE;
            } else if (strcmp(scanner->lexeme, "loop") == 0) {
                token = LOOP;
            } else if (strcmp(scanner->lexeme, "call") == 0) {
                token = CALL;
            } else {
                token = ID;
            }
            break;

        case CLASS_DIGIT: {
            int isFloat = 0;
            int numLength = 0;
            while (scanner->charClass == CLASS_DIGIT && numLength < 11) {
                addChar(scanner);
                nextChar(scanner);
                numLength++;
                if (scanner->currentChar == '.' && !isFloat) {
                    isFloat = 1;
                    addChar(scanner);
                    nextChar(scanner);
                } else if (scanner->currentChar == '.' && isFloat) {
                    fprintf(stderr, ILLEGAL_SYMBOL, scanner->lineNumber, scanner->currentChar);
                    exit(1);
                } else if (numLength > 10) {
                    fprintf(stderr, ILLEGAL_NUMBER, scanner->lineNumber, scanner->lexeme);
                    exit(1);
                } else if (scanner->charClass == CLASS_LETTER) {
                    fprintf(stderr, ILLEGAL_IDENTIFIER, scanner->lineNumber);
                    exit(1);
                }
            }
            token = NUM;
            break;
        }

        case CLASS_UNKNOWN:
            token = lookup(scanner);
            addChar(scanner);
            nextChar(scanner);
            break;

        case CLASS_EOF:
            token = EOF;
            scanner->lexeme[0] = 'E';
            scanner->lexeme[1] = 'O';
            scanner->lexeme[2] = 'F';
            scanner->lexeme[3] = '\0';
            break;
    }
    scanner->currentToken = token;
    // printf("Token is: %d\tLexeme is: %s\n", scanner->currentToken, scanner->lexeme);
}
