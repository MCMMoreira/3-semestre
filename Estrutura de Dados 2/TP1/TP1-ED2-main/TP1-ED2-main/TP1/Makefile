all: tp.o funcoes.o asi.o arvorebin.o arvoreb.o arvorebest.o
	@gcc tp.o funcoes.o asi.o arvorebin.o arvoreb.o arvorebest.o -o exe
	@rm -r tp.o funcoes.o asi.o arvorebin.o arvoreb.o arvorebest.o
tp.o: tp.c
	@gcc tp.c -c -Wall
funcoes.o: funcoes.c
	@gcc funcoes.c -c -Wall
asi.o: asi.c
	@gcc asi.c -c -Wall
arvorebin.o: arvorebin.c
	@gcc arvorebin.c -c -Wall
arvoreb.o: arvoreb.c
	@gcc arvoreb.c -c -Wall
arvorebest.o: arvorebest.c
	@gcc arvorebest.c -c -Wall

val:
	@gcc -g -o arvorebest.c arvoreb.c arvorebin.c asi.c funcoes.c tp.c -Wall
valrun: val
	@valgrind --leak-check=full --track-origins=yes -s ./exeval
run:
	@./exe


