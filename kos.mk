KERNEL_OBJS = kos/debug.o \
       	      kos/kmain.o \
       	      kos/mm/all.o \
       	      kos/mm/pages.o \
       	      kos/mm/liballoc_1_1.o \
       	      kos/mm/liballoc_hooks.o \
       	      kos/arch/all.o \
       	      kos/arch/gdt.o \
       	      kos/arch/idt.o \
       	      kos/arch/segs.o \
       	      kos/arch/isrs.o \
       	      kos/hal/all.o \
       	      kos/hal/res.o \
       	      kos/hal/irq/all.o \
       	      kos/hal/irq/slot.o \
       	      kos/hal/irq/bus.o \
       	      kos/hal/irq/root.o \
       	      kos/hal/timer.o \
	      kos/dev/all.o \
       	      kos/dev/exceptions.o \
       	      kos/dev/i8253.o \
       	      kos/dev/i8259.o

iso/boot/kos.elf: CFLAGS  += -Iinclude -Ilibarch/include -Ilibcore/include -Ikboot/include -Ikos
iso/boot/kos.elf: LDFLAGS += -Tkos/link.ld -pie -Wl,--no-dynamic-linker
iso/boot/kos.elf: LIBS    += -larch -lcore
iso/boot/kos.elf: kos/link.ld libarch.a libcore.a $(KERNEL_OBJS)

ALL_OBJS += $(KERNEL_OBJS)
ALL_ELFS += iso/boot/kos.elf

