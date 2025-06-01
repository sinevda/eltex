all: compile queuefile

compile: server client

server:
	gcc main.c -o server

client:
	gcc client.c -o client

queuefile:
	touch .queuef

remove:
	rm -rf .queue server client