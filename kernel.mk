KERNEL_OBJS = kernel/debug.o \
       	      kernel/kmain.o \
       	      kernel/loader.o
       	      #kernel/mm/pages.o

kernel.elf: CFLAGS  += -Ilibcore/include -Ilibpage/include -Ikernel
kernel.elf: LDFLAGS += -T kernel/link.ld -L.
kernel.elf: LIBS    += -nostdlib -lpage -lcore -lgcc

kernel.elf: kernel/link.ld $(KERNEL_OBJS) libcore.a libpage.a
	$(CC) $(LDFLAGS) -o $@ $(KERNEL_OBJS) $(LIBS)

