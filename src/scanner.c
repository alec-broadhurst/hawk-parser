#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "scanner.h"
#include "error.h"
#include "hashmap.h"

// function to create a scanner and open the source file
Scanner createScanner(const char* filename) {
    Scanner scanner;
    scanner.fp = fopen(filename, "r");
    if (scanner.fp == NULL) {
            fprintf(stderr, "Error opening file: %s\n", filename);
            exit(1);
    }

    fseek(scanner.fp, 0, SEEK_END);
    long file_size = ftell(scanner.fp);
    rewind(scanner.fp);

    scanner.buffer = (char*)malloc(file_size + 1);
    scanner.bufferPtr = scanner.buffer;

    fread(scanner.buffer, file_size, 1, scanner.fp);
    scanner.buffer[file_size] = '\0';
    fclose(scanner.fp);

    scanner.currentToken = UNKNOWN;
    scanner.lineNumber = 1;
    scanner.tokenLineNumber = 1;
    scanner.varTable = newHashMap();

    nextChar(&scanner);
    return scanner;
}

// function to get the next character and determine its class
void nextChar(Scanner* scanner) {
    // load the next character from the buffer into currentChar
    scanner->currentChar = *scanner->bufferPtr;
    if (scanner->currentChar == '\n') {
        scanner->tokenLineNumber = scanner->lineNumber;
        scanner->lineNumber++;
    }
    scanner->bufferPtr++;

    // determine the character class
    if (scanner->currentChar != '\0') {
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

static inline int isOperator(char c) {
    switch (c) {
        case '(': case ')': case ';': case ',': case ':':
        case '=': case '+': case '-': case '*': case '/':
        case '<': case '>':
            return 1;
        default:
            return 0;
    }
}

static inline int isLexemeBreaker(Scanner* scanner) {
    return isspace(scanner->currentChar) || scanner->currentChar == '\0' || isOperator(scanner->currentChar);
}

static inline void consumeLexeme(Scanner* scanner) {
    while (!isLexemeBreaker(scanner)) {
        addChar(scanner);
        nextChar(scanner);
    }
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
    scanner->tokenLineNumber = scanner->lineNumber;

    // determine the token based on the character class
    switch(scanner->charClass) {
        case CLASS_LETTER:
            while (scanner->charClass == CLASS_LETTER || scanner->charClass == CLASS_DIGIT || scanner->currentChar == '_') {
                addChar(scanner);
                nextChar(scanner);
            }

            if (!isLexemeBreaker(scanner)) {
                consumeLexeme(scanner);
                fprintf(stderr, ILLEGAL_IDENTIFIER, scanner->lineNumber, scanner->lexeme);
                exit(1);
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
                    consumeLexeme(scanner);
                    fprintf(stderr, ILLEGAL_NUMBER, scanner->lineNumber, scanner->lexeme);
                    exit(1);
                } else if (numLength > 10) {
                    fprintf(stderr, ILLEGAL_NUMBER, scanner->lineNumber, scanner->lexeme);
                    exit(1);
                } else if (scanner->charClass == CLASS_LETTER) {
                    consumeLexeme(scanner);
                    fprintf(stderr, ILLEGAL_IDENTIFIER, scanner->lineNumber, scanner->lexeme);
                    exit(1);
                }
            }
            token = NUM;
            break;
        }

        case CLASS_UNKNOWN:
            token = lookup(scanner);
            if (token == UNKNOWN) {
                fprintf(stderr, ILLEGAL_SYMBOL, scanner->lineNumber, scanner->currentChar);
                exit(1);
            }
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
}

void destroyScanner(Scanner *scanner) {
    free(scanner->buffer);
    freeHashMap(&scanner->varTable);
}
