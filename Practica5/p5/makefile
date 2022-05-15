CC = gcc
CFLAGS = -g -Wall -pedantic
OBJECTS = lex.yy.o y.tab.o alfa.o hash_table.o generacion.o symbols_table.o


all: $(OBJECTS)
	$(CC) $(CFLAGS) -o alfa $(OBJECTS) 
alfa.o: alfa.c y.tab.h
	$(CC) $(CFLAGS) -c alfa.c
generacion.o: generacion.c generacion.h
	$(CC) $(CFLAGS) -c generacion.c
hash_table.o: hash_table.c hash_table.h
	$(CC) $(CFLAGS) -c hash_table.c
symbols_table.o: symbols_table.c symbols_table.h
	$(CC) $(CFLAGS) -c symbols_table.c
lex.yy.o: lex.yy.c y.tab.h
	$(CC) $(CFLAGS) -c -w lex.yy.c
y.tab.o: y.tab.c
	$(CC) $(CFLAGS) -c y.tab.c
lex.yy.c: alfa.l
	$ flex alfa.l
y.tab.h: alfa.y
	$ bison -d -y -v alfa.y

clean:
	rm -rf alfa lex.yy.c y.tab.c y.tab.h y.output alfa.tab.c alfa.tab.h compilados salida.asm salida.o 
	rm $(OBJECTS)

test_alfa: 
	mkdir compilados
	for file in aritmeticas1 aritmeticas2 comparaciones error_ej1 error_ej2 \
	funciones1 funciones2 funciones3 funciones4 if1 if2 if3 logicas1 logicas2 vectores1 vectores2 vectores3 vectores4 while ; do \
	./alfa archivos_alfa/$$file.alf salida.asm ; \
	nasm -f elf32 -g salida.asm ; \
	gcc -o $$file -m32 salida.o alfalib.o ; \
	mv $$file compilados ; \
	done; \

test_errsem:
	for file in error_se1 error_se2 error_se3 error_se4 error_se5 error_se6 error_se7 error_se8 error_se9 \
	error_se10 error_se11 error_se12 error_se13 error_se14 error_se15; do \
	./alfa err_sem/$$file.alf salida.asm ; \
	done; \
