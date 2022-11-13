BOOT_OBJS = boot/src/entry.o \
	    boot/src/multiboot2.o \
	    boot/src/debug.o \
	    boot/src/bmain.o \
	    boot/src/mm.o \
	    boot/src/fs.o \
	    boot/src/service.o

boot.elf: CFLAGS  += -Ilibcore/include -Ilibmm/include -Ilibx86/include -I boot/include -Iboot
boot.elf: LDFLAGS += -T boot/link.ld -L.
boot.elf: LIBS    += -nostdlib -lx86 -lmm -lcore -lgcc

boot.elf: boot/link.ld $(BOOT_OBJS) libcore.a libmm.a libx86.a
	$(CC) $(LDFLAGS) -o $@ $(BOOT_OBJS) $(LIBS)

ALL_OBJS += $(BOOT_OBJS)
ALL_ELFS += boot.elf
