obj-m += hello_file.o

LINUX_SRC = /tmp/shubham/linux-4.10.1/

all:
	make -C $(LINUX_SRC) M=$(PWD) modules

clean:
	make -C $(LINUX_SRC) M=$(PWD) clean

