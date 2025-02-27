#include "../include/scanner.h"
#include "../include/parser.h"

int main() {
    Scanner scanner = createScanner("tests/input2.txt");
    parseProgram(&scanner);
    return 0;
}
