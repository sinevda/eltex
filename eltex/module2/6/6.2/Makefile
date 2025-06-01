all: phonebook

phonebook: main.o phonelib.so
	gcc main.o -o PhoneBook

main.o: main.c
	gcc -c main.c

phonelib.so: list.c
	gcc -fPIC list.c -c
	gcc -shared list.o -o phonelib.so
