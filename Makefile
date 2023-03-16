run:
	gcc main.c instruction.c code.c -o main -lm
	gcc -c main.c
	gcc -c instruction.c
	gcc -c code.c
	gcc main.o instruction.o code.o -o main -lm
