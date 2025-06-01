all: remove shell

shell: functions
	gcc main.c -o shell/shell.out

functions: dir sum multiply max concat avg min
	
dir:
	mkdir shell

sum:
	gcc sum.c -o shell/sum

multiply:
	gcc multipl.c -o shell/multiply

max:
	gcc max.c -o shell/max

concat:
	gcc concat.c -o shell/concat

avg:
	gcc avg.c -o shell/avg

min:
	gcc min.c -o shell/min

remove:
	rm -rf shell