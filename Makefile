MODULES_NAME = cw2048b

EXTRA_CFLAGS += -I$(src)/head

# If KERNELRELEASE is defined, we've been invoked from the
# kernel build system and can use its language.
# obj-m := $(MODULES_NAME).o
obj-m := module.o
module-objs := source/cw2048b.o source/cw_low.o source/cm_low.o \
	 source/cm_read.o source/cm_data.o source/cm_true.o \
	 source/cm_setuser.o source/cm_srduser.o source/cm_swruser.o \
	 source/cm_write.o source/cw_encry.o source/authen_pw_test.o
# Otherwise we were called directly from the command
# line; invoke the kernel build system.
KERNELDIR ?= $(HOME)/work/rock/rock3188/branch/4.2.2_r1/4.2.2_r1_kl003_new/kernel/
# KERNELDIR ?= /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd) 
default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules


clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean
