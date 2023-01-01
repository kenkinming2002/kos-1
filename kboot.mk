BOOT_OBJS = kboot/src/entry.o \
	    kboot/src/multiboot2.o \
	    kboot/src/debug.o \
	    kboot/src/bmain.o \
	    kboot/src/kboot/all.o \
	    kboot/src/kboot/alloc.o \
	    kboot/src/kboot/info.o \
	    kboot/src/kboot/mmap.o \
	    kboot/src/kernel.o

kboot.elf: CFLAGS  += -Ikcore/include -Ikboot/include -Ikboot/src
kboot.elf: LDFLAGS += -Tkboot/link.ld
kboot.elf: LIBS    += -lkcore
kboot.elf: kboot/link.ld libkcore.a $(BOOT_OBJS)

ALL_OBJS += $(BOOT_OBJS)
ALL_ELFS += kboot.elf
