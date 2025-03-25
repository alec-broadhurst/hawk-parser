#include <stdlib.h>
#include <stdio.h>

#include "scanner.h"
#include "parser.h"
#include "error.h"

int isKeyword(TokenType token) {
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
    printf("Enter PROGRAM\n");
    nextToken(scanner); // get first token
    if (scanner->currentToken != PROGRAM) {
        fprintf(stderr, EXPECTED_KEYWORD, scanner->lineNumber, "program");
        exit(1);
    }
    nextToken(scanner);
    if (scanner->currentToken == BEGIN) {
        nextToken(scanner);
        parseStmtSec(scanner);
        if (scanner->currentToken != END) {
            fprintf(stderr, EXPECTED_KEYWORD, scanner->lineNumber, "end");
            exit(1);
        }
        nextToken(scanner);
        if (scanner->currentToken != SEMICOLON) {
            fprintf(stderr, EXPECTED_SYMBOL, scanner->lineNumber, ';');
            exit(1);
        }
    } else {
        parseDeclSec(scanner);
        if (scanner->currentToken != BEGIN) {
            fprintf(stderr, EXPECTED_KEYWORD, scanner->lineNumber, "begin");
            exit(1);
        }
        nextToken(scanner);
        parseStmtSec(scanner);
        if (scanner->currentToken != END) {
            fprintf(stderr, EXPECTED_KEYWORD, scanner->lineNumber, "end");
            exit(1);
        }
        nextToken(scanner);
        if (scanner->currentToken != SEMICOLON) {
            fprintf(stderr, EXPECTED_SYMBOL, scanner->lineNumber, ';');
            exit(1);
        }
        nextToken(scanner);
        if (scanner->currentToken != EOF) {
            fprintf(stderr, EXPECTED_SYMBOL, scanner->lineNumber, EOF);
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
    parseIdList(scanner, 1);
    if (scanner->currentToken != COLON) {
        fprintf(stderr, EXPECTED_SYMBOL, scanner->lineNumber, ':');
        exit(1);
    }
    nextToken(scanner);
    parseType(scanner);
    if (scanner->currentToken != SEMICOLON) {
        fprintf(stderr, EXPECTED_SYMBOL, scanner->lineNumber, ';');
        exit(1);
    }
    nextToken(scanner); // consume token
    printf("Exit DECL\n");
}

void parseIdList(Scanner* scanner, int isDecl) {
    printf("Enter ID_LIST\n");
    if (scanner->currentToken != ID) {
        if (isKeyword(scanner->currentToken)) {
            fprintf(stderr, KEYWORD_IDENTIFIER_CONFLICT, scanner->lineNumber, scanner->lexeme);
            exit(1);
        }
        fprintf(stderr, EXPECTED_IDENTIFIER, scanner->lineNumber, scanner->lexeme);
        exit(1);
    }
    if (isDecl) {
        addVariable(scanner);
    } else {
        if (!variableLookUp(scanner)) {
            fprintf(stderr, VARIABLE_NOT_DECLARED, scanner->lineNumber, scanner->lexeme);
            exit(1);
        }
    }
    nextToken(scanner);

    if (scanner->currentToken == COMMA) {
        nextToken(scanner);
        parseIdList(scanner, isDecl);
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
            fprintf(stderr, EXPECTED_STATEMENT, scanner->lineNumber, scanner->lexeme);
            exit(1);
    }
    printf("Exit STMT\n");
}

void parseAssign(Scanner* scanner) {
    printf("Enter ASSIGN\n");
    if (scanner->currentToken != ID) {
        fprintf(stderr, EXPECTED_IDENTIFIER, scanner->lineNumber, scanner->lexeme);
        exit(1);
    }
    nextToken(scanner);
    if (scanner->currentToken != ASSIGN) {
        fprintf(stderr, EXPECTED_ASSIGNMENT, scanner->lineNumber, scanner->lexeme);
        exit(1);
    }
    nextToken(scanner);
    parseExpr(scanner);
    if (scanner->currentToken != SEMICOLON) {
        fprintf(stderr, EXPECTED_SYMBOL, scanner->lineNumber, ';');
        exit(1);
    }
    nextToken(scanner);
    printf("Exit ASSIGN\n");
}

void parseIfStmt(Scanner* scanner) {
    printf("Enter IF_STMT\n");
    if (scanner->currentToken != IF) {
        fprintf(stderr, EXPECTED_KEYWORD, scanner->lineNumber, "if");
        exit(1);
    }
    nextToken(scanner);
    parseComp(scanner);
    if (scanner->currentToken != THEN) {
        fprintf(stderr, EXPECTED_KEYWORD, scanner->lineNumber, "then");
        exit(1);
    }
    nextToken(scanner);
    parseStmtSec(scanner);
    if (scanner->currentToken == ELSE) {
        nextToken(scanner);
        parseStmtSec(scanner);
    }
    if (scanner->currentToken != END) {
        fprintf(stderr, EXPECTED_KEYWORD, scanner->lineNumber, "end");
        exit(1);
    }
    nextToken(scanner);
    if (scanner->currentToken != IF) {
        fprintf(stderr, EXPECTED_KEYWORD, scanner->lineNumber, "if");
        exit(1);
    }
    nextToken(scanner);
    if (scanner->currentToken != SEMICOLON) {
        fprintf(stderr, EXPECTED_SYMBOL, scanner->lineNumber, ';');
        exit(1);
    }
    nextToken(scanner);
}

void parseWhileStmt(Scanner* scanner) {
    printf("Enter WHILE_STMT\n");
    if (scanner->currentToken != WHILE) {
        fprintf(stderr, EXPECTED_KEYWORD, scanner->lineNumber, "while");
        exit(1);
    }
    nextToken(scanner);
    parseComp(scanner);
    if (scanner->currentToken != LOOP) {
        fprintf(stderr, EXPECTED_KEYWORD, scanner->lineNumber, "loop");
        exit(1);
    }
    nextToken(scanner);
    parseStmtSec(scanner);
    if (scanner->currentToken != END) {
        fprintf(stderr, EXPECTED_KEYWORD, scanner->lineNumber, "end");
        exit(1);
    }
    nextToken(scanner);
    if (scanner->currentToken != LOOP) {
        fprintf(stderr, EXPECTED_KEYWORD, scanner->lineNumber, "loop");
        exit(1);
    }
    nextToken(scanner);
    if (scanner->currentToken != SEMICOLON) {
        fprintf(stderr, EXPECTED_SYMBOL, scanner->lineNumber, ';');
        exit(1);
    }
    nextToken(scanner);
    printf("Exit WHILE_STMT\n");
}

void parseInput(Scanner* scanner) {
    printf("Enter INPUT\n");
    if (scanner->currentToken != INPUT) {
        fprintf(stderr, EXPECTED_KEYWORD, scanner->lineNumber, "input");
        exit(1);
    }
    nextToken(scanner);
    parseIdList(scanner, 0);
    if (scanner->currentToken != SEMICOLON) {
        fprintf(stderr, EXPECTED_SYMBOL, scanner->lineNumber, ';');
        exit(1);
    }
    nextToken(scanner);
    printf("Exit INPUT\n");
}

void parseOutput(Scanner* scanner) {
    printf("Enter OUTPUT\n");
    if (scanner->currentToken != OUTPUT) {
        fprintf(stderr, EXPECTED_KEYWORD, scanner->lineNumber, "output");
        exit(1);
    }
    nextToken(scanner);
    if (scanner->currentToken == NUM) {
        nextToken(scanner);
    }
    if (scanner->currentToken == ID) {
        parseIdList(scanner, 0);
    }
    if (scanner->currentToken != SEMICOLON) {
        fprintf(stderr, EXPECTED_SYMBOL, scanner->lineNumber, ';');
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
            fprintf(stderr, EXPECTED_SYMBOL, scanner->lineNumber, ')');
            exit(1);
        }
        nextToken(scanner);
    } else if (scanner->currentToken == CALL) {
        nextToken(scanner);
        parseFuncall(scanner);
    } else {
        fprintf(stderr, EXPECTED_OPERAND, scanner->lineNumber, scanner->lexeme);
        exit(1);
    }
    printf("Exit OPERAND\n");
}

void parseComp(Scanner* scanner) {
    printf("Enter COMP\n");
    if (scanner->currentToken != LEFT_PAREN) {
        fprintf(stderr, EXPECTED_SYMBOL, scanner->lineNumber, '(');
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
            fprintf(stderr, EXPECTED_COMPARISON, scanner->lineNumber, scanner->lexeme);
            exit(1);
    }
    parseOperand(scanner);
    if (scanner->currentToken != RIGHT_PAREN) {
        fprintf(stderr, EXPECTED_SYMBOL, scanner->lineNumber, ')');
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
            fprintf(stderr, EXPECTED_TYPE, scanner->lineNumber, scanner->lexeme);
            exit(1);
    }
    //printf("Exit TYPE\n");
}

void parseFuncall(Scanner* scanner) {
    printf("Enter FUNCALL\n");
    if (scanner->currentToken != ID) {
        fprintf(stderr, EXPECTED_IDENTIFIER, scanner->lineNumber, scanner->lexeme);
        exit(1);
    }
    nextToken(scanner);
    if (scanner->currentToken != LEFT_PAREN) {
        fprintf(stderr, EXPECTED_SYMBOL, scanner->lineNumber, '(');
        exit(1);
    }
    nextToken(scanner);
    parseIdList(scanner, 0);
    if (scanner->currentToken != RIGHT_PAREN) {
        fprintf(stderr, EXPECTED_SYMBOL, scanner->lineNumber, ')');
        exit(1);
    }
    nextToken(scanner);
    if (scanner->currentToken != SEMICOLON) {
        fprintf(stderr, EXPECTED_SYMBOL, scanner->lineNumber, ';');
        exit(1);
    }
    nextToken(scanner);
}
