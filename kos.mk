KERNEL_OBJS = kos/debug.o \
       	      kos/kmain.o \
       	      kos/mm/all.o \
       	      kos/mm/pages.o \
       	      kos/mm/liballoc_1_1.o \
       	      kos/mm/liballoc_hooks.o \
       	      kos/pal/all.o \
       	      kos/pal/gdt.o \
       	      kos/pal/idt.o \
       	      kos/pal/segs.o \
       	      kos/pal/isrs.o \
       	      kos/hal/all.o \
       	      kos/hal/access.o \
       	      kos/hal/res.o \
       	      kos/hal/locks.o \
       	      kos/hal/irq/all.o \
       	      kos/hal/irq/slot.o \
       	      kos/hal/irq/bus.o \
       	      kos/hal/irq/root.o \
       	      kos/hal/timer.o \
	      kos/dev/all.o \
       	      kos/dev/exceptions.o \
       	      kos/dev/i8253.o \
       	      kos/dev/i8259.o

iso/boot/kos.elf: CFLAGS  += -Ikcore/include -Ikboot/include -Ikos
iso/boot/kos.elf: LDFLAGS += -Tkos/link.ld -pie -Wl,--no-dynamic-linker
iso/boot/kos.elf: LIBS    += -lkcore
iso/boot/kos.elf: kos/link.ld libkcore.a $(KERNEL_OBJS)

ALL_OBJS += $(KERNEL_OBJS)
ALL_ELFS += iso/boot/kos.elf

