CC=gcc
FLAGS=-std=c99
all: main

main: main.c lexer.o generator.o
	$(CC) $(FLAGS) -lm main.c lexer.o generator.o -o main

lexer.o: lexer.c
	$(CC) $(FLAGS) lexer.c -c

generator.o: generator.c
	$(CC) $(FLAGS) generator.c -c

clean:
	rm -f interpret generator.o lexer.o
