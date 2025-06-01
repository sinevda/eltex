all: pqueue clear

pqueue: main.o pqueue.o
	gcc main.o pqueue.o -o PQ

main.o: main.c
	gcc -c main.c

pqueue.o:
	gcc -c pqueue.c

clear:
	rm -f *.o

remove: clear
	rm PQ
