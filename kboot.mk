BOOT_OBJS = kboot/src/entry.o \
	    kboot/src/multiboot2.o \
	    kboot/src/debug.o \
	    kboot/src/bmain.o \
	    kboot/src/kboot/all.o \
	    kboot/src/kboot/alloc.o \
	    kboot/src/kboot/info.o \
	    kboot/src/kboot/mmap.o \
	    kboot/src/file.o \
	    kboot/src/kernel.o \
	    kboot/src/initrd.o

iso/boot/kboot.elf: CFLAGS  += -Iinclude -Ikcore/include -Ikboot/include -Ikboot/src
iso/boot/kboot.elf: LDFLAGS += -Tkboot/link.ld
iso/boot/kboot.elf: LIBS    += -lkcore
iso/boot/kboot.elf: kboot/link.ld libkcore.a $(BOOT_OBJS)

ALL_OBJS += $(BOOT_OBJS)
ALL_ELFS += iso/boot/kboot.elf
