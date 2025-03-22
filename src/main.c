#include "scanner.h"
#include "parser.h"

int main() {
    Scanner scanner = createScanner("tests/custom.txt");
    parseProgram(&scanner);
    return 0;
}
