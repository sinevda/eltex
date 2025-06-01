all: phonebook

phonebook: main.o list.o
	gcc main.o list.o -o PhoneBook

main.o: main.c
	gcc -c main.c

list.o: list.c
	gcc -c list.c
