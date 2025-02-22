#include <stdio.h>
#include "../include/scanner.h"

int main() {
    Scanner scanner = createScanner("tests/input1.txt");
    while (scanner.currentToken != EOF) {
        nextToken(&scanner);
        printf("Token: %d, Lexeme: %s\n", scanner.currentToken, scanner.lexeme);
    }
    return 0;
}
