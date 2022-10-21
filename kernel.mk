KERNEL_OBJS = kernel/boot_params.o \
       	      kernel/debug.o \
       	      kernel/kmain.o \
       	      kernel/loader.o \
       	      kernel/mm/pages.o

kernel.elf: kernel/link.ld $(KERNEL_OBJS) libcore.a
	$(CC) $(LDFLAGS) -T kernel/link.ld -o $@ $(KERNEL_OBJS) -nostdlib -L. -lcore -lgcc

