KERNEL_SRCS = kos/kmain.c \
       	      kos/mm/all.c \
       	      kos/mm/pages.c \
       	      kos/mm/liballoc_1_1.c \
       	      kos/mm/liballoc_hooks.c \
       	      kos/arch/all.c \
       	      kos/arch/segmentation.c \
       	      kos/arch/segmentation.S \
       	      kos/arch/interrupt.c \
       	      kos/arch/interrupt.S \
       	      kos/hal/module.c \
       	      kos/hal/res.c \
       	      kos/hal/irq.c \
       	      kos/hal/timer.c \
       	      kos/dev/i8253.c \
       	      kos/dev/i8259.c

KERNEL_OBJS = $(KERNEL_SRCS:=.o)
KERNEL_DEPS = $(KERNEL_SRCS:=.d)

iso/boot/kos.elf: CFLAGS  += -Iinclude -Ilibcore/include -Ilibarch/include -Ikboot/include -Ikos
iso/boot/kos.elf: LDFLAGS += -Tkos/link.ld -pie -Wl,--no-dynamic-linker
iso/boot/kos.elf: LIBS    += -lcore -larch
iso/boot/kos.elf: kos/link.ld libcore.a libarch.a $(KERNEL_OBJS)

ALL_OBJS += $(KERNEL_OBJS)
ALL_DEPS += $(KERNEL_DEPS)
ALL_ELFS += iso/boot/kos.elf

