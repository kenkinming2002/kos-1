KERNEL_OBJS = kernel/debug.o \
       	      kernel/kmain.o \
       	      kernel/mm/all.o \
       	      kernel/mm/pages.o \
       	      kernel/mm/liballoc_1_1.o \
       	      kernel/mm/liballoc_hooks.o \
       	      kernel/pal/all.o \
       	      kernel/pal/gdt.o \
       	      kernel/pal/idt.o \
       	      kernel/pal/segs.o \
       	      kernel/pal/isrs.o \
       	      kernel/hal/all.o \
       	      kernel/hal/access.o \
       	      kernel/hal/res.o \
       	      kernel/hal/locks.o \
       	      kernel/hal/irq/all.o \
       	      kernel/hal/irq/slot.o \
       	      kernel/hal/irq/bus.o \
       	      kernel/hal/timer.o \
	      kernel/dev/all.o \
       	      kernel/dev/exceptions.o \
       	      kernel/dev/i8253.o \
       	      kernel/dev/i8259.o

kernel.elf: CFLAGS  += -Ilibcore/include -Iboot/include -Ikernel
kernel.elf: LDFLAGS += -T kernel/link.ld -pie -Wl,--no-dynamic-linker
kernel.elf: LIBS    += -lcore
kernel.elf: kernel/link.ld libcore.a $(KERNEL_OBJS)

ALL_OBJS += $(KERNEL_OBJS)
ALL_ELFS += kernel.elf

