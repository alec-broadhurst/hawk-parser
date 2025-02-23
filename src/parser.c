#include <stdio.h>
#include "../include/scanner.h"
#include "../include/parser.h"


void parseProgram(Scanner* scanner) {
    printf("Enter PROGRAM\n");
    if (scanner->currentToken != PROGRAM) {
        printf("Error: Expected keyword 'program'\n");
        return;
    }
    nextToken(scanner);
    if (scanner->currentToken == BEGIN) {
        nextToken(scanner);
        parseStmtSec(scanner);
        if (scanner->currentToken != END) {
            printf("Error: Expected keyword 'end'\n");
            return;
        }
        nextToken(scanner);
        if (scanner->currentToken != SEMICOLON) {
            printf("Error: Missing ';'\n");
            return;
        }
    } else {
        parseDeclSec(scanner);
        nextToken(scanner);
        parseStmtSec(scanner);
        if (scanner->currentToken != END) {
            printf("Error: Expected keyword 'end'\n");
            return;
        }
    }
}
void parseDeclSec(Scanner* scanner) {}
void parseDecl(Scanner* scanner) {}
void parseIdList(Scanner* scanner) {}
void parseId(Scanner* scanner) {}
void parseStmtSec(Scanner* scanner) {}
void parseStmt(Scanner* scanner) {}
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
