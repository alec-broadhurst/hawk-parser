#include "scanner.h"
#include "parser.h"

int main(int argc, char* argv[]) {
    Scanner scanner;

    if (argc == 1) {
        scanner = createScanner("tests/custom.txt");
    } else if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    } else {
        scanner = createScanner(argv[1]);
    }

    parseProgram(&scanner);
    destroyScanner(&scanner);

    return 0;
}
