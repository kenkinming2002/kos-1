BOOT_OBJS = boot/entry.o \
	    boot/multiboot2.o \
	    boot/debug.o \
	    boot/bmain.o \
	    boot/mm.o \
	    boot/info/modules.o \
	    boot/boot_params.o

boot.elf: CFLAGS  += -Ilibcore/include -Ilibmm/include -Ilibx86/include -Iboot
boot.elf: LDFLAGS += -T boot/link.ld -L.
boot.elf: LIBS    += -nostdlib -lx86 -lmm -lcore -lgcc

boot.elf: boot/link.ld $(BOOT_OBJS) libcore.a libmm.a libx86.a
	$(CC) $(LDFLAGS) -o $@ $(BOOT_OBJS) $(LIBS)

ALL_OBJS += $(BOOT_OBJS)
ALL_ELFS += boot.elf
