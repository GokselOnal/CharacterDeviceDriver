obj-m += char_driver_project.o
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	gcc userProgram.c -o program

mkprogram:
	gcc userProgram.c -o program

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm program
