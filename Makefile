CC = gcc

EX  =  bin/soru1 \
       bin/soru2

all: $(EX) 

clean: 
	rm -f bin/*

bin/soru1: src/soru1.c
	$(CC) -o bin/soru1 src/soru1.c

bin/soru2: src/soru2.c
	$(CC) -o bin/soru2 src/soru2.c
