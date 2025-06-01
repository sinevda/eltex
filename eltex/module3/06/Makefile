all: compilation queuefile

compilation: first second

queuefile:
	touch queuef
	chmod 0770 queuef

first:
	gcc first.c -o first

second:
	gcc second.c -o second
	
remove:
	rm -rf first second queuef
