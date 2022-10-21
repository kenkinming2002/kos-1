KERNEL_OBJS = kernel/core/assert.o \
       	      kernel/core/bitmap.o \
       	      kernel/core/print.o \
       	      kernel/core/string.o \
       	      kernel/boot_params.o \
       	      kernel/debug.o \
       	      kernel/kmain.o \
       	      kernel/loader.o \
       	      kernel/mm/pages.o

kernel.elf: kernel/link.ld $(KERNEL_OBJS)
	$(CC) $(LDFLAGS) -T kernel/link.ld -o $@ $(KERNEL_OBJS) -nostdlib -lgcc

