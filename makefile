default: build

build:
	gcc -o snake main.c snake.c

run: build
	./snake 16 8