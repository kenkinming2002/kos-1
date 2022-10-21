BOOT_OBJS = boot/loader.o boot/bmain.o

boot.elf: boot/link.ld $(BOOT_OBJS)
	$(CC) $(LDFLAGS) -T boot/link.ld -o $@ $(BOOT_OBJS) -nostdlib -lgcc
