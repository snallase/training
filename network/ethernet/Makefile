obj-m     += ethd.o

KDIR ?= /lib/modules/`uname -r`/build

default:
	$(MAKE) -C $(KDIR) M=$$PWD

clean:
	
	$(MAKE) -C $(KDIR) M=$$PWD clean
