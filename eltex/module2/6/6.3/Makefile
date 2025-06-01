all: calc clear

calc: libs main.o
	gcc main.o -o Calc

libs: libscat sum.so diff.so divis.so multipl.so 

libscat:
	mkdir -p libs

main.o: main.c
	gcc main.c -c

sum.so: sum.c
	gcc -fPIC sum.c -c
	gcc -shared sum.o -o libs/sum.so

divis.so: divis.c
	gcc -fPIC divis.c -c
	gcc -shared divis.o -o libs/divis.so
	
diff.so: diff.c
	gcc -fPIC diff.c -c
	gcc -shared diff.o -o libs/diff.so

multipl.so: multipl.c
	gcc -fPIC multipl.c -c
	gcc -shared multipl.o -o libs/multipl.so


clear:
	rm -f *.o

remove:
	rm -r libs
	rm Calc