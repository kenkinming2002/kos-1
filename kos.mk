KERNEL_OBJS = kos/debug.c.o \
       	      kos/kmain.c.o \
       	      kos/mm/all.c.o \
       	      kos/mm/pages.c.o \
       	      kos/mm/liballoc_1_1.c.o \
       	      kos/mm/liballoc_hooks.c.o \
       	      kos/arch/all.c.o \
       	      kos/arch/segmentation.c.o \
       	      kos/arch/segmentation.S.o \
       	      kos/arch/interrupt.c.o \
       	      kos/arch/interrupt.S.o \
       	      kos/hal/module.c.o \
       	      kos/hal/res.c.o \
       	      kos/hal/irq/slot.c.o \
       	      kos/hal/irq/bus.c.o \
       	      kos/hal/irq/root.c.o \
       	      kos/hal/timer.c.o \
	      kos/dev/all.c.o \
       	      kos/dev/exceptions.c.o \
       	      kos/dev/i8253.c.o \
       	      kos/dev/i8259.c.o

iso/boot/kos.elf: CFLAGS  += -Iinclude -Ilibarch/include -Ilibcore/include -Ikboot/include -Ikos
iso/boot/kos.elf: LDFLAGS += -Tkos/link.ld -pie -Wl,--no-dynamic-linker
iso/boot/kos.elf: LIBS    += -larch -lcore
iso/boot/kos.elf: kos/link.ld libarch.a libcore.a $(KERNEL_OBJS)

ALL_OBJS += $(KERNEL_OBJS)
ALL_ELFS += iso/boot/kos.elf

