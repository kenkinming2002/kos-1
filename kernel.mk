KERNEL_OBJS = kernel/debug.o \
       	      kernel/kmain.o \
       	      #kernel/mm/pages.o

kernel.elf: CFLAGS  += -Ilibcore/include -Ilibmm/include -Ikernel -fPIE
kernel.elf: LDFLAGS += -T kernel/link.ld -L. -fPIE
kernel.elf: LIBS    += -nostdlib -lmm -lcore -lgcc

kernel.elf: kernel/link.ld $(KERNEL_OBJS) libcore.a libmm.a
	$(CC) $(LDFLAGS) -o $@ $(KERNEL_OBJS) $(LIBS)

ALL_OBJS += $(KERNEL_OBJS)
ALL_ELFS += kernel.elf

