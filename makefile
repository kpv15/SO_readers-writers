CC = gcc

all: main czytelnik pisarz

main: main.o api.o
	$(CC) main.o api.o -o main
	
pisarz: pisarz.o api.o
	$(CC) pisarz.o api.o -o pisarz
	
czytelnik: czytelnik.o api.o
	$(CC) czytelnik.o api.o -o czytelnik

api.o: api.c api.h
	$(CC) api.c -c -o api.o

main.o: main.c api.h
	$(CC) main.c -c -o main.o
	
pisarz.o: pisarz.c api.h
	$(CC) pisarz.c -c -o pisarz.o

czytelnik.o: czytelnik.c api.h
	$(CC) czytelnik.c -c -o czytelnik.o