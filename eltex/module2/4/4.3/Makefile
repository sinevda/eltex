all: PhoneBook clear

PhoneBook: main.o BTree.o
	gcc main.o BTree.o -o PhoneBook

main.o: main.c
	gcc main.c -c

BTree.o: BTree.c
	gcc BTree.c -c

clear:
	rm -f *.o

remove: clear
	rm PhoneBook