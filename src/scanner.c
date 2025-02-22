#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/scanner.h"

// function to create a scanner and open the source file
Scanner createScanner(const char* filename) {
    Scanner scanner = {0};
    scanner.fp = fopen(filename, "r");
    if (scanner.fp == NULL) {
            fprintf(stderr, "Error opening file: %s\n", filename);
            exit(1);
        }
    scanner.charClass = CLASS_UNKNOWN;
    scanner.currentToken = UNKNOWN;
    nextChar(&scanner);
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

// function to lookup operators and parentheses
TokenType lookup(Scanner* scanner) {
    TokenType token = UNKNOWN;
    switch(scanner->currentChar) {
        case '(':
            addChar(scanner);
            nextChar(scanner);
            token = LEFT_PAREN;
            break;

        case ')':
            addChar(scanner);
            nextChar(scanner);
            token = RIGHT_PAREN;
            break;

        case ';':
            addChar(scanner);
            nextChar(scanner);
            token = SEMICOLON;
            break;

        case ',':
            addChar(scanner);
            nextChar(scanner);
            token = COMMA;
            break;

        case ':':
            addChar(scanner);
            nextChar(scanner);
            if (scanner->currentChar == '=') {
                addChar(scanner);
                nextChar(scanner);
                token = ASSIGN;
            } else {
                token = COLON;
            }
            break;

        case '+':
            addChar(scanner);
            nextChar(scanner);
            token = PLUS;
            break;

        case '-':
            addChar(scanner);
            nextChar(scanner);
            token = MINUS;
            break;

        case '*':
            addChar(scanner);
            nextChar(scanner);
            token = MULTIPLY;
            break;

        case '/':
            addChar(scanner);
            nextChar(scanner);
            token = DIVIDE;
            break;

        case '<':
            addChar(scanner);
            nextChar(scanner);
            if (scanner->currentChar == '>') {
                addChar(scanner);
                nextChar(scanner);
                token = NOT_EQUALS;
            } else {
                token = LESS_THAN;
            }
            break;

        case '>':
            addChar(scanner);
            nextChar(scanner);
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
            addChar(scanner);
            nextChar(scanner);
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
            token = lookup(scanner);
            break;
        case CLASS_EOF:
            token = EOF;
            break;
    }
    scanner->currentToken = token;
}
