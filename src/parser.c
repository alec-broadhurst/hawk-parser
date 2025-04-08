#include <stdlib.h>
#include <stdio.h>

#include "scanner.h"
#include "parser.h"
#include "hashmap.h"
#include "error.h"

static void parseDeclSec(Scanner* scanner);
static void parseDecl(Scanner* scanner);
static void parseIdList(Scanner* scanner, int isDecl);
static void parseStmtSec(Scanner* scanner);
static void parseStmt(Scanner* scanner);
static void parseAssign(Scanner* scanner);
static void parseIfStmt(Scanner* scanner);
static void parseWhileStmt(Scanner* scanner);
static void parseInput(Scanner* scanner);
static void parseOutput(Scanner* scanner);
static void parseExpr(Scanner* scanner);
static void parseFactor(Scanner* scanner);
static void parseOperand(Scanner* scanner);
static void parseComp(Scanner* scanner);
static void parseType(Scanner* scanner);
static void parseFuncall(Scanner* scanner);
static inline int isKeyword(TokenType token);

static void parseDeclSec(Scanner* scanner) {
    printf("DECL_SEC\n");
    parseDecl(scanner);
    if (scanner->currentToken == ID) {
        parseDeclSec(scanner);
    }
}

static void parseDecl(Scanner* scanner) {
    printf("DECL\n");
    parseIdList(scanner, 1);
    if (scanner->currentToken != COLON) {
        fprintf(stderr, EXPECTED_SYMBOL, scanner->tokenLineNumber, ':');
        exit(1);
    }
    nextToken(scanner);
    parseType(scanner);
    if (scanner->currentToken != SEMICOLON) {
        fprintf(stderr, EXPECTED_SYMBOL, scanner->tokenLineNumber, ';');
        exit(1);
    }
    nextToken(scanner);
}

static void parseIdList(Scanner* scanner, int isDecl) {
    printf("ID_LIST\n");
    if (scanner->currentToken != ID) {
        if (isKeyword(scanner->currentToken)) {
            fprintf(stderr, KEYWORD_IDENTIFIER_CONFLICT, scanner->tokenLineNumber, scanner->lexeme);
            exit(1);
        }
        fprintf(stderr, EXPECTED_IDENTIFIER, scanner->tokenLineNumber, scanner->lexeme);
        exit(1);
    }
    if (isDecl) {
        if (variableLookup(&scanner->varTable, scanner->lexeme)) {
            fprintf(stderr, VARIABLE_REDECLARATION, scanner->tokenLineNumber, scanner->lexeme);
            exit(1);
        }
        insert(&scanner->varTable, scanner->lexeme);
    } else {
        if (!variableLookup(&scanner->varTable, scanner->lexeme)) {
            fprintf(stderr, VARIABLE_NOT_DECLARED, scanner->tokenLineNumber, scanner->lexeme);
            exit(1);
        }
    }
    nextToken(scanner);

    if (scanner->currentToken == COMMA) {
        nextToken(scanner);
        parseIdList(scanner, isDecl);
    }
}

static void parseStmtSec(Scanner* scanner) {
    printf("STMT_SEC\n");
    parseStmt(scanner);

    if (scanner->currentToken == ID || scanner->currentToken == IF || scanner->currentToken == WHILE || scanner->currentToken == INPUT || scanner->currentToken == OUTPUT) {
        parseStmtSec(scanner);
    }
}

static void parseStmt(Scanner* scanner) {
    printf("STMT\n");
    switch(scanner->currentToken) {
        case ID:
            parseAssign(scanner);
            break;
        case IF:
            parseIfStmt(scanner);
            break;
        case WHILE:
            parseWhileStmt(scanner);
            break;
        case INPUT:
            parseInput(scanner);
            break;
        case OUTPUT:
            parseOutput(scanner);
            break;
        default:
            fprintf(stderr, EXPECTED_STATEMENT, scanner->tokenLineNumber, scanner->lexeme);
            exit(1);
    }
}

static void parseAssign(Scanner* scanner) {
    printf("ASSIGN\n");
    if (scanner->currentToken != ID) {
        fprintf(stderr, EXPECTED_IDENTIFIER, scanner->tokenLineNumber, scanner->lexeme);
        exit(1);
    }
    if (!variableLookup(&scanner->varTable, scanner->lexeme)) {
        fprintf(stderr, VARIABLE_NOT_DECLARED, scanner->tokenLineNumber, scanner->lexeme);
        exit(1);
    }
    nextToken(scanner);
    if (scanner->currentToken != ASSIGN) {
        fprintf(stderr, EXPECTED_ASSIGNMENT, scanner->tokenLineNumber, scanner->lexeme);
        exit(1);
    }
    nextToken(scanner);
    parseExpr(scanner);
    if (scanner->currentToken != SEMICOLON) {
        fprintf(stderr, EXPECTED_SYMBOL, scanner->tokenLineNumber, ';');
        exit(1);
    }
    nextToken(scanner);
}

static void parseIfStmt(Scanner* scanner) {
    printf("IF_STMT\n");
    if (scanner->currentToken != IF) {
        fprintf(stderr, EXPECTED_KEYWORD, scanner->tokenLineNumber, "if");
        exit(1);
    }
    nextToken(scanner);
    parseComp(scanner);
    if (scanner->currentToken != THEN) {
        fprintf(stderr, EXPECTED_KEYWORD, scanner->tokenLineNumber, "then");
        exit(1);
    }
    nextToken(scanner);
    parseStmtSec(scanner);
    if (scanner->currentToken == ELSE) {
        nextToken(scanner);
        parseStmtSec(scanner);
    }
    if (scanner->currentToken != END) {
        fprintf(stderr, EXPECTED_KEYWORD, scanner->tokenLineNumber, "end");
        exit(1);
    }
    nextToken(scanner);
    if (scanner->currentToken != IF) {
        fprintf(stderr, EXPECTED_KEYWORD, scanner->tokenLineNumber, "if");
        exit(1);
    }
    nextToken(scanner);
    if (scanner->currentToken != SEMICOLON) {
        fprintf(stderr, EXPECTED_SYMBOL, scanner->tokenLineNumber, ';');
        exit(1);
    }
    nextToken(scanner);
}

static void parseWhileStmt(Scanner* scanner) {
    printf("WHILE_STMT\n");
    if (scanner->currentToken != WHILE) {
        fprintf(stderr, EXPECTED_KEYWORD, scanner->tokenLineNumber, "while");
        exit(1);
    }
    nextToken(scanner);
    parseComp(scanner);
    if (scanner->currentToken != LOOP) {
        fprintf(stderr, EXPECTED_KEYWORD, scanner->tokenLineNumber, "loop");
        exit(1);
    }
    nextToken(scanner);
    parseStmtSec(scanner);
    if (scanner->currentToken != END) {
        fprintf(stderr, EXPECTED_KEYWORD, scanner->tokenLineNumber, "end");
        exit(1);
    }
    nextToken(scanner);
    if (scanner->currentToken != LOOP) {
        fprintf(stderr, EXPECTED_KEYWORD, scanner->tokenLineNumber, "loop");
        exit(1);
    }
    nextToken(scanner);
    if (scanner->currentToken != SEMICOLON) {
        fprintf(stderr, EXPECTED_SYMBOL, scanner->tokenLineNumber, ';');
        exit(1);
    }
    nextToken(scanner);
}

static void parseInput(Scanner* scanner) {
    printf("INPUT\n");
    if (scanner->currentToken != INPUT) {
        fprintf(stderr, EXPECTED_KEYWORD, scanner->tokenLineNumber, "input");
        exit(1);
    }
    nextToken(scanner);
    parseIdList(scanner, 0);
    if (scanner->currentToken != SEMICOLON) {
        fprintf(stderr, EXPECTED_SYMBOL, scanner->tokenLineNumber, ';');
        exit(1);
    }
    nextToken(scanner);
}

static void parseOutput(Scanner* scanner) {
    printf("OUTPUT\n");
    if (scanner->currentToken != OUTPUT) {
        fprintf(stderr, EXPECTED_KEYWORD, scanner->tokenLineNumber, "output");
        exit(1);
    }
    nextToken(scanner);
    if (scanner->currentToken == NUM) {
        nextToken(scanner);
    } else if (scanner->currentToken == ID) {
        parseIdList(scanner, 0);
    } else {
        fprintf(stderr, EXPECTED_ID_NUM, scanner->tokenLineNumber, scanner->lexeme);
        exit(1);
    }
    if (scanner->currentToken != SEMICOLON) {
        fprintf(stderr, EXPECTED_SYMBOL, scanner->tokenLineNumber, ';');
        exit(1);
    }
    nextToken(scanner);
}

static void parseExpr(Scanner* scanner) {
    printf("EXPR\n");
    parseFactor(scanner);
    if (scanner->currentToken == PLUS || scanner->currentToken == MINUS) {
        nextToken(scanner);
        parseExpr(scanner);
    }
}

static void parseFactor(Scanner* scanner) {
    printf("FACTOR\n");
    parseOperand(scanner);
    if (scanner->currentToken == MULTIPLY || scanner->currentToken == DIVIDE) {
        nextToken(scanner);
        parseFactor(scanner);
    }
}

static void parseOperand(Scanner* scanner) {
    printf("OPERAND\n");
    if (scanner->currentToken == NUM || scanner->currentToken == ID) {
        if (scanner->currentToken == ID) {
            if (!variableLookup(&scanner->varTable, scanner->lexeme)) {
                fprintf(stderr, VARIABLE_NOT_DECLARED, scanner->tokenLineNumber, scanner->lexeme);
                exit(1);
            }
        }
        nextToken(scanner);
    } else if (scanner->currentToken == LEFT_PAREN) {
        nextToken(scanner);
        parseExpr(scanner);
        if (scanner->currentToken != RIGHT_PAREN) {
            fprintf(stderr, EXPECTED_SYMBOL, scanner->tokenLineNumber, ')');
            exit(1);
        }
        nextToken(scanner);
    } else if (scanner->currentToken == CALL) {
        nextToken(scanner);
        parseFuncall(scanner);
    } else {
        fprintf(stderr, EXPECTED_OPERAND, scanner->tokenLineNumber, scanner->lexeme);
        exit(1);
    }
}

static void parseComp(Scanner* scanner) {
    printf("COMP\n");
    if (scanner->currentToken != LEFT_PAREN) {
        fprintf(stderr, EXPECTED_SYMBOL, scanner->tokenLineNumber, '(');
        exit(1);
    }
    nextToken(scanner);
    parseOperand(scanner);
    switch(scanner->currentToken) {
        case EQUALS:
            nextToken(scanner);
            break;
        case LESS_THAN:
            nextToken(scanner);
            break;
        case GREATER_THAN:
            nextToken(scanner);
            break;
        case NOT_EQUALS:
            nextToken(scanner);
            break;
        default:
            fprintf(stderr, EXPECTED_COMPARISON, scanner->tokenLineNumber, scanner->lexeme);
            exit(1);
    }
    parseOperand(scanner);
    if (scanner->currentToken != RIGHT_PAREN) {
        fprintf(stderr, EXPECTED_SYMBOL, scanner->tokenLineNumber, ')');
        exit(1);
    }
    nextToken(scanner);
}

static void parseType(Scanner* scanner) {
    switch (scanner->currentToken) {
        case INT:
            nextToken(scanner);
            break;
        case FLOAT:
            nextToken(scanner);
            break;
        case DOUBLE:
            nextToken(scanner);
            break;
        default:
            fprintf(stderr, EXPECTED_TYPE, scanner->tokenLineNumber, scanner->lexeme);
            exit(1);
    }
}

static void parseFuncall(Scanner* scanner) {
    printf("FUNCALL\n");
    if (scanner->currentToken != ID) {
        fprintf(stderr, EXPECTED_IDENTIFIER, scanner->tokenLineNumber, scanner->lexeme);
        exit(1);
    }
    nextToken(scanner);
    if (scanner->currentToken != LEFT_PAREN) {
        fprintf(stderr, EXPECTED_SYMBOL, scanner->tokenLineNumber, '(');
        exit(1);
    }
    nextToken(scanner);
    parseIdList(scanner, 0);
    if (scanner->currentToken != RIGHT_PAREN) {
        fprintf(stderr, EXPECTED_SYMBOL, scanner->tokenLineNumber, ')');
        exit(1);
    }
    nextToken(scanner);
}

static inline int isKeyword(TokenType token) {
    switch (token) {
    case PROGRAM:
    case BEGIN:
    case END:
    case IF:
    case THEN:
    case ELSE:
    case WHILE:
    case LOOP:
    case INPUT:
    case OUTPUT:
    case INT:
    case FLOAT:
    case DOUBLE:
    case CALL:
        return 1;
    default:
        return 0;
    }
}

void parseProgram(Scanner* scanner) {
    printf("PROGRAM\n");
    nextToken(scanner); // get first token
    if (scanner->currentToken != PROGRAM) {
        fprintf(stderr, EXPECTED_KEYWORD, scanner->tokenLineNumber, "program");
        exit(1);
    }
    nextToken(scanner);
    if (scanner->currentToken == BEGIN) {
        nextToken(scanner);
        parseStmtSec(scanner);
        if (scanner->currentToken != END) {
            fprintf(stderr, EXPECTED_KEYWORD, scanner->tokenLineNumber, "end");
            exit(1);
        }
        nextToken(scanner);
        if (scanner->currentToken != SEMICOLON) {
            fprintf(stderr, EXPECTED_SYMBOL, scanner->tokenLineNumber, ';');
            exit(1);
        }
    } else {
        parseDeclSec(scanner);
        if (scanner->currentToken != BEGIN) {
            fprintf(stderr, EXPECTED_KEYWORD, scanner->tokenLineNumber, "begin");
            exit(1);
        }
        nextToken(scanner);
        parseStmtSec(scanner);
        if (scanner->currentToken != END) {
            fprintf(stderr, EXPECTED_KEYWORD, scanner->tokenLineNumber, "end");
            exit(1);
        }
        nextToken(scanner);
        if (scanner->currentToken != SEMICOLON) {
            fprintf(stderr, EXPECTED_SYMBOL, scanner->tokenLineNumber, ';');
            exit(1);
        }
        nextToken(scanner);
        if (scanner->currentToken != EOF) {
            fprintf(stderr, EXPECTED_SYMBOL, scanner->tokenLineNumber, EOF);
            exit(1);
        }
    }
}
