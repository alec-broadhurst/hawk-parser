#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include "scanner.h"

void parseProgram(Scanner* scanner);
void parseDeclSec(Scanner* scanner);
void parseDeclSecPrime(Scanner* scanner);
void parseDecl(Scanner* scanner);
void parseIdList(Scanner* scanner);
void parseId(Scanner* scanner);
void parseStmtSec(Scanner* scanner);
void parseStmtSecPrime(Scanner* scanner);
void parseStmt(Scanner* scanner);
void parseAssign(Scanner* scanner);
void parseIfStmt(Scanner* scanner);
void parseWhileStmt(Scanner* scanner);
void parseInput(Scanner* scanner);
void parseOutput(Scanner* scanner);
void parseExpr(Scanner* scanner);
void parseFactor(Scanner* scanner);
void parseOperand(Scanner* scanner);
void parseNum(Scanner* scanner);
void parseComp(Scanner* scanner);
void parseType(Scanner* scanner);
void parseFuncall(Scanner* scanner);

#endif
