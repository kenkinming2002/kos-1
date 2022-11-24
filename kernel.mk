KERNEL_OBJS = kernel/debug.o \
       	      kernel/kmain.o \
       	      kernel/mm.o

kernel.elf: CFLAGS  += -Ilibcore/include -Ilibmm/include -Iboot/include -Ikernel
kernel.elf: LDFLAGS += -T kernel/link.ld -L.
kernel.elf: LIBS    += -nostdlib -lmm -lcore -lgcc

kernel.elf: kernel/link.ld $(KERNEL_OBJS) libcore.a libmm.a
	$(CC) $(LDFLAGS) -o $@ $(KERNEL_OBJS) $(LIBS)

ALL_OBJS += $(KERNEL_OBJS)
ALL_ELFS += kernel.elf

