#include "../include/scanner.h"
#include "../include/parser.h"
#include <stdio.h>

int main() {
    Scanner scanner = createScanner("tests/test.txt");
    parseProgram(&scanner);
    return 0;
}
