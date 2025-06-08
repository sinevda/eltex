obj-m += netlink.o

all: client module

clean: clean_client clean_module


client:
	gcc client.c -o client

module:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules


clean_client:
	rm client

clean_module:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean