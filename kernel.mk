KERNEL_OBJS = kernel/debug.o \
       	      kernel/kmain.o \
       	      kernel/mm/pages.o

kernel.elf: CFLAGS  += -Ilibcore/include -Iboot/include -Ikernel
kernel.elf: LDFLAGS += -T kernel/link.ld -L.
kernel.elf: LIBS    += -nostdlib -lcore -lgcc

kernel.elf: kernel/link.ld $(KERNEL_OBJS) libcore.a
	$(CC) $(LDFLAGS) -o $@ $(KERNEL_OBJS) $(LIBS)

ALL_OBJS += $(KERNEL_OBJS)
ALL_ELFS += kernel.elf

