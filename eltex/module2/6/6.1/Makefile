all: phonebook

phonebook: main.o libph.a
	gcc main.o -o PhoneBook -L. -lph

main.o: main.c
	gcc -c main.c

libph.a: list.c
	gcc list.c -c
	ar r libph.a list.o
