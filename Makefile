main:
	gcc -c main.c
	gcc -c instruction.c
	gcc -c code.c
	gcc main.o instruction.o code.o -o main -lm

run: main
	./main
