BOOT_OBJS = kboot/src/entry.S.o \
	    kboot/src/trampoline.S.o \
	    kboot/src/multiboot2.S.o \
	    kboot/src/debug.c.o \
	    kboot/src/bmain.c.o \
	    kboot/src/kboot/all.c.o \
	    kboot/src/kboot/alloc.c.o \
	    kboot/src/kboot/info.c.o \
	    kboot/src/kboot/mmap.c.o \
	    kboot/src/file.c.o \
	    kboot/src/kernel.c.o \
	    kboot/src/initrd.c.o

iso/boot/kboot.elf: CFLAGS  += -Iinclude -Ilibarch/include -Ilibcore/include -Ikboot/include -Ikboot/src
iso/boot/kboot.elf: LDFLAGS += -Tkboot/link.ld
iso/boot/kboot.elf: LIBS    += -larch -lcore
iso/boot/kboot.elf: kboot/link.ld libarch.a libcore.a $(BOOT_OBJS)

ALL_OBJS += $(BOOT_OBJS)
ALL_ELFS += iso/boot/kboot.elf
