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
    // nextToken(scanner);
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
    nextToken(scanner); // consume ID, get next token
}

void parseStmtSec(Scanner* scanner) {
    printf("Enter STMT_SEC\n");
    parseStmt(scanner);
    if (scanner->currentToken != END) {
        nextToken(scanner);
        parseStmtSec(scanner);
    }
    printf("Exit STMT_SEC\n");
}

void parseStmt(Scanner* scanner) {
    printf("Enter STMT\n");
    nextToken(scanner);
    if (scanner->currentToken == ID) {
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

void parseAssign(Scanner* scanner) {
    parseId(scanner);
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
}

void parseIfStmt(Scanner* scanner) {
    printf("Enter IF_STMT");
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
}

void parseInput(Scanner* scanner) {
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
}

void parseOutput(Scanner* scanner) {
    if (scanner->currentToken != OUTPUT) {
        printf("Error: Expected keyword 'output'\n");
        exit(1);
    }
    nextToken(scanner);
    if (scanner->currentToken == NUM) {
        nextToken(scanner);
    } else {
        parseIdList(scanner);
    }
    if (scanner->currentToken != SEMICOLON) {
        printf("Error: Missing ';'\n");
        exit(1);
    }
    nextToken(scanner);
}

void parseExpr(Scanner* scanner) {
    parseFactor(scanner);
    if (scanner->currentToken == PLUS || scanner->currentToken == MINUS) {
        nextToken(scanner);
        parseExpr(scanner);
    }
    nextToken(scanner);
}

void parseFactor(Scanner* scanner) {
    parseOperand(scanner);
    if (scanner->currentToken == MULTIPLY || scanner->currentToken == DIVIDE) {
        nextToken(scanner);
        parseFactor(scanner);
    }
}

void parseOperand(Scanner* scanner) {
    if (scanner->currentToken == NUM) {
        parseNum(scanner);
    } else if (scanner->currentToken == ID) {
        parseId(scanner);
    } else if (scanner->currentToken == LEFT_PAREN) {
        nextToken(scanner);
        parseExpr(scanner);
        if (scanner->currentToken != RIGHT_PAREN) {
            printf("Error: Missing ')'\n");
            exit(1);
        }
    } else {
        printf("Error: Expected operand\n");
        exit(1);
    }
}

void parseNum(Scanner* scanner) {}
void parseComp(Scanner* scanner) {}

void parseType(Scanner* scanner) {
    printf("Enter TYPE\n");
    if (scanner->currentToken == INT) {
        nextToken(scanner);
    } else if (scanner->currentToken == FLOAT) {
        nextToken(scanner);
    } else if (scanner->currentToken == DOUBLE) {
        nextToken(scanner);
    } else {
        printf("Error: Expected type\n");
        exit(1);
    }
    printf("Exit TYPE\n");
}

void parseFuncall(Scanner* scanner) {}
