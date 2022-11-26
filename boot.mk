BOOT_OBJS = boot/src/entry.o \
	    boot/src/multiboot2.o \
	    boot/src/debug.o \
	    boot/src/bmain.o \
	    boot/src/mm.o \
	    boot/src/fs.o \
	    boot/src/kernel.o \
	    boot/src/service.o

boot.elf: CFLAGS  += -Ilibcore/include -Iboot/include -Iboot
boot.elf: LDFLAGS += -T boot/link.ld
boot.elf: LIBS    += -lcore
boot.elf: boot/link.ld libcore.a $(BOOT_OBJS)

ALL_OBJS += $(BOOT_OBJS)
ALL_ELFS += boot.elf
