#include <stdlib.h>
#include <stdio.h>
#include "../include/scanner.h"
#include "../include/parser.h"


void parseProgram(Scanner* scanner) {
    printf("Enter PROGRAM\n");
    nextToken(scanner); // get first token
    if (scanner->currentToken != PROGRAM) {
        printf("Error: Expected keyword 'program'\n");
        exit(1);
    }
    nextToken(scanner);
    if (scanner->currentToken == BEGIN) {
        nextToken(scanner);
        parseStmtSec(scanner);
        if (scanner->currentToken != END) {
            printf("Error: Expected keyword 'end'\n");
            exit(1);
        }
        nextToken(scanner);
        if (scanner->currentToken != SEMICOLON) {
            printf("Error: Missing ';'\n");
            exit(1);
        }
    } else {
        parseDeclSec(scanner);
        nextToken(scanner);
        parseStmtSec(scanner);
        if (scanner->currentToken != END) {
            printf("Error: Expected keyword 'end'\n");
            exit(1);
        }
    }
    printf("Exit PROGRAM\n");
}

void parseDeclSec(Scanner* scanner) {
    printf("Enter DECL_SEC\n");
    parseDecl(scanner);
    if (scanner->currentToken != BEGIN) {
        parseDeclSec(scanner);
    }
    nextToken(scanner);
    printf("Exit DECL_SEC\n");
}

void parseDecl(Scanner* scanner) {
    printf("Enter DECL\n");
    parseIdList(scanner);
    if (scanner->currentToken != COLON) {
        printf("Error: Missing ':'\n");
        exit(1);
    }
    nextToken(scanner);
    parseType(scanner);
    nextToken(scanner);
    if (scanner->currentToken != SEMICOLON) {
        printf("Error: Missing ';'\n");
        exit(1);
    }
    nextToken(scanner);
    printf("Exit DECL\n");
}
void parseIdList(Scanner* scanner) {
    printf("Enter ID_LIST\n");
    parseId(scanner);
    if (scanner->currentToken == COMMA) {
        nextToken(scanner);
        parseIdList(scanner);
    }
    printf("Exit ID_LIST\n");
}

void parseId(Scanner* scanner) {
    if (scanner->currentToken != ID) {
        printf("Error: Expected identifier\n");
        exit(1);
    }
    nextToken(scanner);
}

void parseStmtSec(Scanner* scanner) {
    printf("Enter STMT_SEC\n");
    parseStmt(scanner);
    if (scanner->currentToken == SEMICOLON) {
        nextToken(scanner);
        parseStmtSec(scanner);
    }
    printf("Exit STMT_SEC\n");
}

void parseStmt(Scanner* scanner) {
    printf("Enter STMT\n");
    if (scanner->currentToken == ASSIGN) {
        parseAssign(scanner);
    } else if (scanner->currentToken == IF) {
        parseIfStmt(scanner);
    } else if (scanner->currentToken == WHILE) {
        parseWhileStmt(scanner);
    } else if (scanner->currentToken == INPUT) {
        parseInput(scanner);
    } else if (scanner->currentToken == OUTPUT) {
        parseOutput(scanner);
    } else {
        printf("Error: Expected statement\n");
        exit(1);
    }
    printf("Exit STMT\n");
}
void parseAssign(Scanner* scanner) {}
void parseIfStmt(Scanner* scanner) {}
void parseWhileStmt(Scanner* scanner) {}
void parseInput(Scanner* scanner) {}
void parseOutput(Scanner* scanner) {}
void parseExpr(Scanner* scanner) {}
void parseFactor(Scanner* scanner) {}
void parseOperand(Scanner* scanner) {}
void parseNum(Scanner* scanner) {}
void parseComp(Scanner* scanner) {}
void parseType(Scanner* scanner) {}
void parseFuncall(Scanner* scanner) {}
