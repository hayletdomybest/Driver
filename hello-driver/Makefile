#------------------------------------------
#This is my Hello-Modules Makefile
#-----------------------------------------

obj-m := hello-driver.o

kernelDir=/lib/modules/$(shell uname -r)/build

all:
	make -C $(kernelDir) M=$(PWD) modules

clean:
	make -C $((kernelDir) M=$(PWD)/ clean





