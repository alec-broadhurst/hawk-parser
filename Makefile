default: clean build run

clean:
	rm -rf bin/parser

build: | bin
	clang src/scanner.c src/main.c -o bin/parser

bin:
	mkdir -p bin

run:
	./bin/parser
