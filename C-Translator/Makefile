CC=gcc
FLAGS=-std=c99
all: interpret

interpret: interpret.c lexer.o generator.o
	$(CC) $(FLAGS) -lm interpret.c lexer.o generator.o -o interpret

lexer.o: lexer.c
	$(CC) $(FLAGS) lexer.c -c

generator.o: generator.c
	$(CC) $(FLAGS) generator.c -c

clean:
	rm -f interpret generator.o lexer.o