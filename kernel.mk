KERNEL_OBJS = kernel/debug.o \
       	      kernel/kmain.o \
       	      kernel/mm.o \
       	      kernel/mm/pages.o \
       	      kernel/mm/liballoc_1_1.o \
       	      kernel/mm/liballoc_hooks.o \
       	      kernel/hal/exceptions.o \
       	      kernel/hal.o \
       	      kernel/hal/ports.o \
       	      kernel/hal/gdt.o \
       	      kernel/hal/idt.o \
       	      kernel/hal/segs.o \
       	      kernel/hal/isrs.o \
       	      kernel/hal/irqs.o \
       	      kernel/pic/isa.o \
       	      kernel/pic/pic8259.o \
       	      kernel/timer/pit.o

kernel.elf: CFLAGS  += -Ilibcore/include -Iboot/include -Ikernel
kernel.elf: LDFLAGS += -T kernel/link.ld -pie -Wl,--no-dynamic-linker
kernel.elf: LIBS    += -lcore
kernel.elf: kernel/link.ld libcore.a $(KERNEL_OBJS)

ALL_OBJS += $(KERNEL_OBJS)
ALL_ELFS += kernel.elf

