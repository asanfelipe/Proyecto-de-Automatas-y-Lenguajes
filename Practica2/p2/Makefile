CC = gcc -Wall -o 

all : lex.yy.c pruebaMorfo.o

lex.yy.c: alfa.l
	flex alfa.l

pruebaMorfo.o: 
	$(CC) pruebaMorfo pruebaMorfo.c tokens.h lex.yy.c

clean : 
	rm -f *.o pruebaMorfo lex.yy.c