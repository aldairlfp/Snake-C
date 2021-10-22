default: build

build:
	gcc -o snake snake.c main.c

run: build
	./snake 16 8