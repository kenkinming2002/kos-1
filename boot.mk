BOOT_OBJS = boot/loader.o \
	    boot/debug.o \
	    boot/bmain.o \
	    boot/mmap.o \
	    boot/modules.o \
	    boot/pages.o \
	    boot/boot_params.o

boot.elf: CFLAGS  += -Ilibcore/include -Ilibpage/include -Ilibx86 -Iboot
boot.elf: LDFLAGS += -T boot/link.ld -L.
boot.elf: LIBS    += -nostdlib -lx86 -lpage -lcore -lgcc

boot.elf: boot/link.ld $(BOOT_OBJS) libcore.a libpage.a libx86.a
	$(CC) $(LDFLAGS) -o $@ $(BOOT_OBJS) $(LIBS)
