KERNEL_OBJS = kernel/debug.o \
       	      kernel/kmain.o \
       	      kernel/mm.o \
       	      kernel/mm/pages.o \
       	      kernel/mm/liballoc_1_1.o \
       	      kernel/mm/liballoc_hooks.o \
       	      kernel/pal.o \
       	      kernel/pal/gdt.o \
       	      kernel/pal/idt.o \
       	      kernel/pal/segs.o \
       	      kernel/pal/isrs.o \
       	      kernel/hal.o \
       	      kernel/hal/irqs.o \
       	      kernel/hal/ports.o \
	      kernel/irqs.o \
       	      kernel/irqs/exceptions.o \
       	      kernel/irqs/pic8259.o \
       	      kernel/irqs/isa.o \
       	      kernel/timer/pit.o

kernel.elf: CFLAGS  += -Ilibcore/include -Iboot/include -Ikernel
kernel.elf: LDFLAGS += -T kernel/link.ld -pie -Wl,--no-dynamic-linker
kernel.elf: LIBS    += -lcore
kernel.elf: kernel/link.ld libcore.a $(KERNEL_OBJS)

ALL_OBJS += $(KERNEL_OBJS)
ALL_ELFS += kernel.elf

