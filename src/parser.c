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
        if (scanner->currentToken != BEGIN) {
            printf("Error: Expected keyword 'begin'\n");
            exit(1);
        }
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
        nextToken(scanner);
        if (scanner->currentToken != EOF) {
            printf("Error: Expected EOF\n");
            exit(1);
        }
    }
    printf("Exit PROGRAM\n");
}

// DECL_SEC -> DECL | DECL DECL_SEC
// DECL_SEC -> DECL DECL_SEC_PRIME
// DECL_SEC_PRIME -> DECL DECL_SEC_PRIME | EPSILON
void parseDeclSec(Scanner* scanner) {
    parseDecl(scanner);
    parseDeclSecPrime(scanner);
}

void parseDeclSecPrime(Scanner* scanner) {
    if (scanner->currentToken == ID) {
        parseDecl(scanner);
        parseDeclSecPrime(scanner);
    }
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
    if (scanner->currentToken != SEMICOLON) {
        printf("Error: Missing ';'\n");
        exit(1);
    }
    nextToken(scanner); // consume token
    printf("Exit DECL\n");
}

void parseIdList(Scanner* scanner) {
    printf("Enter ID_LIST\n");
    if (scanner->currentToken != ID) {
        printf("Error: Expected ID\n");
        exit(1);
    }
    nextToken(scanner);

    if (scanner->currentToken == COMMA) {
        nextToken(scanner);
        parseIdList(scanner);
    }

    printf("Exit ID_LIST\n");
}

// STMT_SEC -> STMT | STMT STMT_SEC
// STMT_SEC -> STMT STMT_SEC'
// STMT_SEC' -> STMT STMT_SEC' | EPSILON
void parseStmtSec(Scanner* scanner) {
    printf("Enter STMT_SEC\n");
    parseStmt(scanner);
    parseStmtSecPrime(scanner);
    printf("Exit STMT_SEC\n");
}

void parseStmtSecPrime(Scanner* scanner) {
    if (scanner->currentToken == ID || scanner->currentToken == IF || scanner->currentToken == WHILE || scanner->currentToken == INPUT || scanner->currentToken == OUTPUT) {
        parseStmt(scanner);
        parseStmtSecPrime(scanner);
    }
}

void parseStmt(Scanner* scanner) {
    printf("Enter STMT\n");
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
            printf("Error: Expected statement\n");
            exit(1);
    }
    printf("Exit STMT\n");
}

void parseAssign(Scanner* scanner) {
    printf("Enter ASSIGN\n");
    if (scanner->currentToken != ID) {
        printf("Error: Expected ID\n");
        exit(1);
    }
    nextToken(scanner);
    if (scanner->currentToken != ASSIGN) {
        printf("Error: Missing assignment operator\n");
        exit(1);
    }
    nextToken(scanner);
    parseExpr(scanner);
    if (scanner->currentToken != SEMICOLON) {
        printf("Error: Missing ';'\n");
        exit(1);
    }
    nextToken(scanner);
    printf("Exit ASSIGN\n");
}

void parseIfStmt(Scanner* scanner) {
    printf("Enter IF_STMT\n");
    if (scanner->currentToken != IF) {
        printf("Error: Expected keyword 'if'\n");
        exit(1);
    }
    nextToken(scanner);
    parseComp(scanner);
    if (scanner->currentToken != THEN) {
        printf("Error: Expected keyword 'then'\n");
        exit(1);
    }
    nextToken(scanner);
    parseStmtSec(scanner);
    if (scanner->currentToken == ELSE) {
        nextToken(scanner);
        parseStmtSec(scanner);
    }
    if (scanner->currentToken != END) {
        printf("Error: Expected keyword 'end'\n");
        exit(1);
    }
    nextToken(scanner);
    if (scanner->currentToken != IF) {
        printf("Error: Expected keyword 'if'\n");
        exit(1);
    }
    nextToken(scanner);
    if (scanner->currentToken != SEMICOLON) {
        printf("Error: Missing ';'\n");
        exit(1);
    }
    nextToken(scanner);
}

void parseWhileStmt(Scanner* scanner) {
    printf("Enter WHILE_STMT\n");
    if (scanner->currentToken != WHILE) {
        printf("Error: Expected keyword 'while'\n");
        exit(1);
    }
    nextToken(scanner);
    parseComp(scanner);
    if (scanner->currentToken != LOOP) {
        printf("Error: Expected keyword 'loop'\n");
        exit(1);
    }
    nextToken(scanner);
    parseStmtSec(scanner);
    if (scanner->currentToken != END) {
        printf("Error: Expected keyword 'end'\n");
        exit(1);
    }
    nextToken(scanner);
    if (scanner->currentToken != LOOP) {
        printf("Error: Expected keyword 'loop'\n");
        exit(1);
    }
    nextToken(scanner);
    if (scanner->currentToken != SEMICOLON) {
        printf("Error: Missing ';'\n");
        exit(1);
    }
    nextToken(scanner);
    printf("Exit WHILE_STMT\n");
}

void parseInput(Scanner* scanner) {
    printf("Enter INPUT\n");
    if (scanner->currentToken != INPUT) {
        printf("Error: Expected keyword 'input'\n");
        exit(1);
    }
    nextToken(scanner);
    parseIdList(scanner);
    if (scanner->currentToken != SEMICOLON) {
        printf("Error: Missing ';'\n");
        exit(1);
    }
    nextToken(scanner);
    printf("Exit INPUT\n");
}

void parseOutput(Scanner* scanner) {
    printf("Enter OUTPUT\n");
    if (scanner->currentToken != OUTPUT) {
        printf("Error: Expected keyword 'output'\n");
        exit(1);
    }
    nextToken(scanner);
    if (scanner->currentToken == NUM) {
        nextToken(scanner);
    }
    if (scanner->currentToken == ID) {
        parseIdList(scanner);
    }
    if (scanner->currentToken != SEMICOLON) {
        printf("Error: Missing ';'\n");
        exit(1);
    }
    nextToken(scanner);
    printf("Exit OUTPUT\n");
}

void parseExpr(Scanner* scanner) {
    printf("Enter EXPR\n");
    parseFactor(scanner);
    if (scanner->currentToken == PLUS || scanner->currentToken == MINUS) {
        nextToken(scanner);
        parseExpr(scanner);
    }
    //nextToken(scanner);
    printf("Exit EXPR\n");
}

void parseFactor(Scanner* scanner) {
    printf("Enter FACTOR\n");
    parseOperand(scanner);
    if (scanner->currentToken == MULTIPLY || scanner->currentToken == DIVIDE) {
        nextToken(scanner);
        parseFactor(scanner);
    }
    //nextToken(scanner);
    printf("Exit FACTOR\n");
}

void parseOperand(Scanner* scanner) {
    printf("Enter OPERAND\n");
    if (scanner->currentToken == NUM || scanner->currentToken == ID) {
        nextToken(scanner);
    } else if (scanner->currentToken == LEFT_PAREN) {
        nextToken(scanner);
        parseExpr(scanner);
        if (scanner->currentToken != RIGHT_PAREN) {
            printf("Error: Missing ')'\n");
            exit(1);
        }
        nextToken(scanner);
    } else if (scanner->currentToken == CALL) {
        nextToken(scanner);
        parseFuncall(scanner);
    } else {
        printf("Error: Expected operand\n");
        exit(1);
    }
    printf("Exit OPERAND\n");
}

void parseComp(Scanner* scanner) {
    printf("Enter COMP\n");
    if (scanner->currentToken != LEFT_PAREN) {
        printf("Error: Missing '('\n");
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
            printf("Error: Expected comparison operator\n");
            exit(1);
    }
    parseOperand(scanner);
    if (scanner->currentToken != RIGHT_PAREN) {
        printf("Error: Missing ')'\n");
        exit(1);
    }
    nextToken(scanner);
    printf("Exit COMP\n");
}

void parseType(Scanner* scanner) {
    //printf("Enter TYPE\n");
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
            printf("Error: Expected type\n");
            exit(1);
    }
    //printf("Exit TYPE\n");
}

void parseFuncall(Scanner* scanner) {
    printf("Enter FUNCALL\n");
    if (scanner->currentToken != ID) {
        printf("Error: Expected ID\n");
        exit(1);
    }
    nextToken(scanner);
    if (scanner->currentToken != LEFT_PAREN) {
        printf("Error: Missing '('\n");
        exit(1);
    }
    nextToken(scanner);
    parseIdList(scanner);
    if (scanner->currentToken != RIGHT_PAREN) {
        printf("Error: Missing ')'\n");
        exit(1);
    }
    nextToken(scanner);
    if (scanner->currentToken != SEMICOLON) {
        printf("Error: Missing ';'\n");
        exit(1);
    }
    nextToken(scanner);
}
