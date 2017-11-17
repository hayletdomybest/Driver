#------------------------------------------
#This is my Hello-Modules Makefile
#-----------------------------------------

obj-m := main.o

kernelDir=/lib/modules/4.9.0-4-rpi/build

all:
	make -C $(kernelDir) M=$(PWD) modules

clean:
	rm -rf .*.cmd *.o *.mod.c *.ko .tmp_versions Module.symvers modules.order





