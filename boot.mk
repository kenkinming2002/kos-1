BOOT_OBJS = boot/src/entry.o \
	    boot/src/multiboot2.o \
	    boot/src/debug.o \
	    boot/src/bmain.o \
	    boot/src/kboot/all.o \
	    boot/src/kboot/alloc.o \
	    boot/src/kboot/info.o \
	    boot/src/kboot/mmap.o \
	    boot/src/kernel.o

boot.elf: CFLAGS  += -Ilibcore/include -Iboot/include -Iboot/src
boot.elf: LDFLAGS += -T boot/link.ld
boot.elf: LIBS    += -lcore
boot.elf: boot/link.ld libcore.a $(BOOT_OBJS)

ALL_OBJS += $(BOOT_OBJS)
ALL_ELFS += boot.elf
