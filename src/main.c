#include "scanner.h"
#include "parser.h"

int main() {
    Scanner scanner = createScanner("tests/test1.txt");
    parseProgram(&scanner);
    destroyScanner(&scanner);
    return 0;
}
