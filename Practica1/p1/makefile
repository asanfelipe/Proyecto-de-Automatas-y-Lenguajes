################################################################################
CC 			= gcc
CFLAGS	= -g -Wall -pedantic -ansi
EJS 		= ej1 ej2 ej3 ej4 ej5 ej6 ej7
################################################################################
OBJECTS = generacion.o
################################################################################
#gcc -Wall -g -o ej1_asm generacion.c ej1.c
#./ej1_asm ej1.asm
#nasm -g -o ej1.o -f elf32 ej1.asm
#gcc -m32 -o ej1 ej1.o alfalib.o 
#./ej1

all: $(EJS) #clear

ej1: generacion.o
	$(CC) $(CFLAGS) -o ej1_asm generacion.o ej1.c
	./ej1_asm ej1.asm
	nasm -g -o ej1.o -f elf32 ej1.asm
	gcc -m32 -o ej1 ej1.o alfalib.o 

ej2: generacion.o
	$(CC) $(CFLAGS) -o ej2_asm generacion.o ej2.c
	./ej2_asm ej2.asm
	nasm -g -o ej2.o -f elf32 ej2.asm
	gcc -m32 -o ej2 ej2.o alfalib.o 

ej3: generacion.o
	$(CC) $(CFLAGS) -o ej3_asm generacion.o ej3.c
	./ej3_asm ej3.asm
	nasm -g -o ej3.o -f elf32 ej3.asm
	gcc -m32 -o ej3 ej3.o alfalib.o

ej4: generacion.o
	$(CC) $(CFLAGS) -o ej4_asm generacion.o ej4.c
	./ej4_asm ej4.asm
	nasm -g -o ej4.o -f elf32 ej4.asm
	gcc -m32 -o ej4 ej4.o alfalib.o 

ej5: generacion.o
	$(CC) $(CFLAGS) -o ej5_asm generacion.o ej5.c
	./ej5_asm ej5.asm
	nasm -g -o ej5.o -f elf32 ej5.asm
	gcc -m32 -o ej5 ej5.o alfalib.o 

ej6: generacion.o
	$(CC) $(CFLAGS) -o ej6_asm generacion.o ej6.c
	./ej6_asm ej6.asm
	nasm -g -o ej6.o -f elf32 ej6.asm
	gcc -m32 -o ej6 ej6.o alfalib.o 

ej7: generacion.o
	$(CC) $(CFLAGS) -o ej7_asm generacion.o ej7.c
	./ej7_asm ej7.asm
	nasm -g -o ej7.o -f elf32 ej7.asm
	gcc -m32 -o ej7 ej7.o alfalib.o 

generacion.o: generacion.c
	$(CC) $(CFLAGS) -c generacion.c

clear:
	rm -rf ej1.asm ej1_asm ej1.o ej2.o ej3.o ej4.o ej4.asm ej4_asm ej2.asm ej2_asm ej3.asm ej3_asm ej5.asm ej5_asm ej5.o ej6.asm ej6_asm ej6.o ej7.asm ej7_asm ej7.o

clean: clear
	rm -rf generacion.o $(EJS)


run: all
	@echo ">>>>>>Running test"
	./ej1
	./ej2
	./ej3
	./ej4
	./ej5
	./ej6
	./ej7

runv: all
	@echo ">>>>>>Running test with valgrind"
	valgrind --leak-check=full ./ej1
	valgrind --leak-check=full ./ej2
	valgrind --leak-check=full ./ej3
	valgrind --leak-check=full ./ej4
	valgrind --leak-check=full ./ej5
	valgrind --leak-check=full ./ej6
	valgrind --leak-check=full ./ej7


