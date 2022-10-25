BOOT_OBJS = boot/loader.o \
	    boot/debug.o \
	    boot/bmain.o \
	    boot/mmap.o \
	    boot/modules.o \
	    boot/pages.o \
	    boot/boot_params.o

boot.elf: CFLAGS  += -Icore/include -Iboot
boot.elf: LDFLAGS += -T boot/link.ld -L.
boot.elf: LIBS    += -nostdlib -lcore -lgcc

boot.elf: boot/link.ld $(BOOT_OBJS) libcore.a
	$(CC) $(LDFLAGS) -o $@ $(BOOT_OBJS) $(LIBS)
