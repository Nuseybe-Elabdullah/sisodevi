# Basit ogrenci Makefile'i
CC = gcc

all: soru1 soru2

soru1: soru1.c
	$(CC) soru1.c -o soru1

soru2: soru2.c
	$(CC) soru2.c -o soru2

clean:
	rm -f soru1 soru2
